#include <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 25
#define MAX_OBJECTS 1024
#define MARKER_ROWS 9
#define MARKER_COUNT 18
using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];

Mat image, image_input, temp;
int image_height, image_width;
int framenum;
unsigned int offset;

unsigned int calculate_historam(){
	double max_intensity=0;
	double hist_value[256]; // 8-bit depth
	int i;
	
	offset = image_width*image_height - 1;
	histo:
	hist_value[image.data[offset]]+=1;
	if (image.data[offset]>max_intensity) max_intensity= image.data[offset];
	offset--;
	if (offset>0) goto histo;
	for (i=0; i<256; i++){
		printf("hist %d: %.0f\n",i,hist_value[i]);
		}
	printf("max: %.0f\n", max_intensity);
	}

unsigned int calculate_width(int thresholdval, int sep){
	double moment_x_temp, moment_y_temp, mass_temp, moment_x, moment_y;
	int x_min=image_width, x_max=0, y_min=image_height, y_max=0;
	
	offset = image_width*image_height - 1;
	width:
	if (image.data[offset] > thresholdval) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if ((offset % image_width) < sep){
			if (moment_y_temp>y_max) y_max= moment_y_temp;
			if (moment_y_temp<y_min) y_min= moment_y_temp;
			if (moment_x_temp>x_max) x_max= moment_x_temp;
			if (moment_x_temp<x_min) x_min= moment_x_temp;
			mass_temp++;
			moment_x += moment_x_temp;
			moment_y += moment_y_temp;
			}
		}
	offset--;
	if (offset>image_width) goto width;
	printf("marker: %lf %lf %d %d\n",moment_x/mass_temp, moment_y/mass_temp, x_max-x_min, y_max-y_min);
	}

unsigned int calculate_niner(int *margin){
	double moment_x[MARKER_COUNT], moment_y[MARKER_COUNT], mass[MARKER_COUNT];
	double moment_x_temp, moment_y_temp, mass_temp;
	
	offset = image_width*image_height - 1;
niner:
	if (image.data[offset] > THRESHOLD_GRAY) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		for (int i=0; i<MARKER_COUNT; i++){
			if (moment_x_temp<margin[i]){
				moment_x[i]+= moment_x_temp;
				moment_y[i]+= moment_y_temp;
				mass[i]++;
				break;
				}
			}
		}
	offset--;
	if (offset>image_width) goto niner;
	
	for(int i=0; i<MARKER_COUNT; i++){
		cout << setprecision(8) << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << ';';
		}
	cout << endl;
	return(0);
	} 

unsigned int calculate_rower(int separator, int *margin1, int *margin2){
	double moment_x[MARKER_COUNT], moment_y[MARKER_COUNT], mass[MARKER_COUNT];
	double moment_x_temp, moment_y_temp, mass_temp;
	
	image_height = image.rows;
	image_width = image.cols;

	offset = image_width*image_height - 1;
rower:
	if (image.data[offset] > THRESHOLD_GRAY) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if (moment_x_temp>separator){ // first row markers
			for (int i=0; i<9; i++){
				if (moment_y_temp>margin1[i]){
					moment_x[i]+= moment_x_temp;
					moment_y[i]+= moment_y_temp;
					mass[i]++;
					break;
					}
				}
			}
		else { // second row markers
			for (int i=9; i<18; i++){
				if (moment_y_temp>margin2[i-9]){
					moment_x[i]+= moment_x_temp;
					moment_y[i]+= moment_y_temp;
					mass[i]++;
					break;
					}
				}
			}	
		}
	offset--;
	if (offset>image_width) goto rower;
	
	for(int i=0; i<MARKER_COUNT; i++){
		cout << setprecision(8) << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << ';';
		}
	cout << endl;
	return(0);
	} 

unsigned int calculate_moment_gray(int sep) {
	//Mat moment_map = Mat(image_height, image_width, CV_8UC1);
	double moment_x_temp, moment_y_temp, mass_temp;
	int object_counts = 0, offset = 0;
	
// first pass
	offset = image_width*image_height - 1;
restofimage:
	if (image.data[offset] > THRESHOLD_GRAY) {
		if ((offset % image_width) > sep){
			mass_temp++;
			moment_x_temp += (offset % image_width);
			moment_y_temp += (offset / image_width);
			}
		}
	offset--;
	if (offset>image_width) goto restofimage;
	
	cout << setprecision(8) << moment_x_temp/mass_temp << ',' << moment_y_temp/mass_temp << ';';
	return(mass_temp);
}

int cvblob(){
	float moment_x[MAX_OBJECTS], moment_y[MAX_OBJECTS], mass[MAX_OBJECTS];
	float moment_x_temp, moment_y_temp, mass_temp;
	// Set up the detector with default parameters.
	//SimpleBlobDetector detector;
	SimpleBlobDetector::Params params;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByConvexity = true;
	params.filterByInertia = true;
	//params.maxThreshold = 220;
	//params.minThreshold = 100;
	SimpleBlobDetector detector(params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	//Mat invert; bitwise_not(image, invert); detector.detect(invert, keypoints);
	detector.detect(image, keypoints);

	int n=0; 
	for (std::vector<KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end(); ++it){
		//cout << it->pt.x  << ';' << it->pt.y << endl;
		moment_x[n]= it->pt.x;
		moment_y[n]= it->pt.y;
		mass[n]= it->size;
		n++;
		} //  std::cout << ' ' << *it;	
	
	// little sorting
	for (int i=n-1; i>=0; i--){
		for (int j=i-1; j>=0; j--){
			if (moment_x[i] > moment_x[j]){
				moment_x_temp= moment_x[i];
				moment_y_temp= moment_y[i];
				mass_temp= mass[i];
				moment_x[i]= moment_x[j];
				moment_y[i]= moment_y[j];
				mass[i]= mass[j];
				moment_x[j]= moment_x_temp;
				moment_y[j]= moment_y_temp;
				mass[j]= mass_temp;
				}
			}
		}
	
	// the log	
	for (int i=n-1; i>=0; i--){
		//logfile << moment_x[i] << ',' << moment_y[i] << ',' << mass[i] <<';';
		cout << moment_x[i] 	<< ',' << moment_y[i] << ';';
		}
	//logfile << n;	
	cout << endl;
	return(n);
}

// parse image sequence	
int main(int argc, char **argv){
	//logfile.open("/me/log.txt");
	sprintf(filename, "%s", argv[2]);
	image_input = imread(argv[2], 1);
	image_height = image_input.rows;
	image_width = image_input.cols;
	//printf("%s %d %d: ",filename, image_width, image_height);
	int mrgins[MARKER_COUNT]={2048};
	int separator= 370;
	int row2[9]={1500, 1234, 1060, 872, 688, 500, 378, 228, 10};
	int row1[9]={1616, 1372, 1137, 930, 744, 576, 420, 270, 50};
	
	if (image_height && image_width) {
		// in case of rotated camera, 3 lines
		cvtColor(image_input, temp, CV_BGR2GRAY);
		transpose(temp, image); 
		flip(image, image, 1);
		// or it is already in grayscale
		//cvtColor(image_input, image, CV_BGR2GRAY);
		//imshow("disp",image); waitKey(0);
		if (!strcmp(argv[1],"h")) calculate_historam();
		else if (!strcmp(argv[1],"g")) calculate_moment_gray(atoi(argv[3]));
		else if (!strcmp(argv[1],"c")) calculate_niner(mrgins);
		else if (!strcmp(argv[1],"d")) calculate_rower(separator, row1, row2);
		else if (!strcmp(argv[1],"w")) calculate_width(atoi(argv[3]), atoi(argv[4]));
		else if (!strcmp(argv[1],"v")) cvblob();
		else {
			cout << "wrong argument for argv[1]" << endl; 
			return(0);
			}
		//
	}
	else {
		cout << "fail to open " << filename << endl;
		return(1);
		}
	return(0);
}



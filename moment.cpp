#include <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 10
#define MAX_OBJECTS 1024
#define MARKER_ROWS 9
#define MARKER_COUNT 18
#define MARKER_COUNT_REF 20
#define PI 3.1415926

using namespace std;
using namespace cv;


ofstream logfile;
char filename[256];

Mat image, image_input, temp;
int image_height, image_width;
int framenum;
unsigned int offset;

unsigned int read_row(int row){
	offset = image_width*row - 1;
	readrow:
	printf("%d;",image.data[offset]);
	offset--;
	if (offset%image_width) goto readrow;
	}

unsigned int calculate_histogram(int threshold){
	double max_intensity=0;
	double hist_value[256]; // 8-bit depth image
	double cumulative_value, cumulative_count;
	int i;
	
	offset = image_width*image_height - 1;
	histo:
	hist_value[image.data[offset]]++;
	if (image.data[offset]>threshold){
		cumulative_count++;
		cumulative_value+=image.data[offset];
		}
	if (image.data[offset]>max_intensity) max_intensity= image.data[offset];
	offset--;
	if (offset>0) goto histo;
	for (i=0; i<256; i++){
		//printf("hist %d: %.0f\n",i,hist_value[i]);
		}
	cumulative_value /= cumulative_count;	
	printf("%s max: %.0f; avg: %.0f\n", filename, max_intensity, cumulative_value);
	}

unsigned int calculate_width(int thresholdval, int sep){
	double moment_x_temp, moment_y_temp, mass_temp, moment_x, moment_y;
	image_width= image.cols;
	image_height= image.rows;

	int x_min=image_width, x_max=0, y_min=image_height, y_max=0;
	
	mass_temp=0;
	moment_x= 0;
	moment_y= 0;
			
	printf("w%d,h%d\n",image_width, image_height);
	offset = image_width*image_height - 1;
	width:
	if (image.data[offset] > thresholdval) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if ((offset % image_width) > sep){
			//if (moment_y_temp>y_max) y_max= moment_y_temp;
			//if (moment_y_temp<y_min) y_min= moment_y_temp;
			//if (moment_x_temp>x_max) x_max= moment_x_temp;
			//if (moment_x_temp<x_min) x_min= moment_x_temp;
			mass_temp++;
			moment_x += moment_x_temp;
			moment_y += moment_y_temp;
			}
		}
		
	offset--;
	if (offset>image_width) goto width;
	printf("%.4lf,%.4lf\n",moment_x/mass_temp, moment_y/mass_temp);
	
	/*
	// minimum width/height
	x_max=image_width, x_min=0, y_max=image_height, y_min=0;
	offset = image_width*image_height - 1;
	width:
	if (image.data[offset] > thresholdval) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if ((offset % image_width) < sep){
			if (moment_y_temp<y_max) y_max= moment_y_temp;
			if (moment_y_temp>y_min) y_min= moment_y_temp;
			if (moment_x_temp>x_max) x_max= moment_x_temp;
			if (moment_x_temp<x_min) x_min= moment_x_temp;
			mass_temp++;
			moment_x += moment_x_temp;
			moment_y += moment_y_temp;
			}
		}
	//printf("min: %.0lf %lf %lf %d %d \n",mass_temp, moment_x/mass_temp, moment_y/mass_temp, x_max-x_min, y_max-y_min);
	*/
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

unsigned int calculate_rower(int separator, int *margin1, int *margin2, int thre){
	double *moment_x, *moment_y, *mass;
	double *cumulative_value, *cumulative_count; 
	unsigned char *max_value;
	moment_x = (double *) calloc(MARKER_COUNT,sizeof(double));
	moment_y = (double *) calloc(MARKER_COUNT,sizeof(double));
	mass = (double *) calloc(MARKER_COUNT,sizeof(double));
	double *x_min, *x_max, *y_min, *y_max;
	x_min = (double *) calloc(MARKER_COUNT,sizeof(double));
	y_min = (double *) calloc(MARKER_COUNT,sizeof(double));
	x_max = (double *) calloc(MARKER_COUNT,sizeof(double));
	y_max = (double *) calloc(MARKER_COUNT,sizeof(double));
	cumulative_count = (double *) (double *) calloc(MARKER_COUNT,sizeof(double));
	cumulative_value = (double *) (double *) calloc(MARKER_COUNT,sizeof(double));
	max_value = (unsigned char *) calloc(MARKER_COUNT,sizeof(char));
	
	int moment_x_temp, moment_y_temp, mass_temp;
	
	image_height = image.rows;
	image_width = image.cols;

	for (int i=0; i<MARKER_COUNT; i++){
		y_min[i]= image_height;
		x_min[i]= image_width;
		}
	
	for (offset = image_width*image_height - 1; offset>0; offset--){
	if (image.data[offset] > thre) {
		moment_x_temp = (offset % image_width);
		//moment_x_temp %= image_width;
		moment_y_temp = (offset / image_width);
		//moment_y_temp %= image_height;
		if (moment_x_temp>separator){ // first row markers
			for (int i=0; i<=8; i++){
				if (moment_y_temp>margin1[i]){
					mass[i]+=1.0;
					moment_x[i] += (double) moment_x_temp;
					moment_y[i] += (double) moment_y_temp;
					cumulative_count[i]++;
					cumulative_value[i]+=image.data[offset];
					if (image.data[offset] > max_value[i]) max_value[i]= image.data[offset];
					if (moment_x_temp>x_max[i]) x_max[i]= moment_x_temp;
					if (moment_x_temp<x_min[i]) x_min[i]= moment_x_temp;
					if (moment_y_temp>y_max[i]) y_max[i]= moment_y_temp;
					if ((moment_y_temp<y_min[i]) && (moment_y_temp>margin1[i])) y_min[i]= moment_y_temp;
					break;
					}
				}
			}
		else { // second row markers
			for (int i=9; i<=17; i++){
				if (moment_y_temp>margin2[i-9]){
					mass[i]+=1.0;
					moment_x[i]+= (double) moment_x_temp;
					moment_y[i]+= (double) moment_y_temp;
					cumulative_count[i]++;
					cumulative_value[i]+=image.data[offset];
					if (image.data[offset] > max_value[i]) max_value[i] = image.data[offset];
					if (moment_x_temp>x_max[i]) x_max[i]= moment_x_temp;
					if (moment_x_temp<x_min[i]) x_min[i]= moment_x_temp;
					if (moment_y_temp>y_max[i]) y_max[i]= moment_y_temp;
					if ((moment_y_temp<y_min[i]) && (moment_y_temp>margin2[i-9])) y_min[i]= moment_y_temp;
					break;
					}
				}
			}	
		}
	}
	
	for(int i=0; i<MARKER_COUNT; i++){
		//cumulative_value[i] /= cumulative_count[i]; // average intensity	
		//cout << (int) max_value[i] << ',' << cumulative_value[i] << ';';
		//cout << sqrt(4*mass[i]/PI) << ',' << x_max[i]-x_min[i] << ',' << y_max[i]-y_min[i] << ';';
		cout << setprecision(8) << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << ';';
		//cout << moment_x[i] << '\t' <<moment_y[i] << '\t' << mass[i] << endl;
		}
	cout << endl;
	return(0);
	} 

unsigned int calculate_ref(int upper, int lower, int mid, int *margin1, int *margin2){
	double moment_x[MARKER_COUNT_REF], moment_y[MARKER_COUNT_REF], mass[MARKER_COUNT_REF];
	double moment_x_temp, moment_y_temp, mass_temp;
	
	image_height = image.rows;
	image_width = image.cols;

	offset = image_width*image_height - 1;
ref:
	if (image.data[offset] > THRESHOLD_GRAY) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		
		// right reference
		if (moment_x_temp>upper){ 
			moment_x[18]+= moment_x_temp;
			moment_y[18]+= moment_y_temp;
			mass[18]++;
			}
		// left reference
		else if (moment_x_temp<lower){
			moment_x[19]+= moment_x_temp;
			moment_y[19]+= moment_y_temp;
			mass[19]++;
			} 
		//	
		else if (moment_x_temp>mid){ // first row markers
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
	if (offset>image_width) goto ref;
	
	for(int i=0; i<MARKER_COUNT_REF; i++){
		cout << setprecision(8) << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << ';';
		}
	cout << endl;
	return(0);
	} 

unsigned int calculate_refw(int thre, int upper, int lower, int mid, int *margin1, int *margin2){
	double moment_x[MARKER_COUNT_REF], moment_y[MARKER_COUNT_REF], mass[MARKER_COUNT_REF];
	double moment_x_temp, moment_y_temp, mass_temp;
	
	image_height = image.rows;
	image_width = image.cols;

	offset = image_width*image_height - 1;
rw:
	if (image.data[offset] > thre) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		
		// right reference
		if (moment_x_temp>upper){ 
			moment_x[18]+= moment_x_temp;
			moment_y[18]+= moment_y_temp;
			mass[18]++;
			}
		// left reference
		else if (moment_x_temp<lower){
			moment_x[19]+= moment_x_temp;
			moment_y[19]+= moment_y_temp;
			mass[19]++;
			} 
		//	
		else if (moment_x_temp>mid){ // first row markers
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
	if (offset>image_width) goto rw;
	
	for(int i=0; i<MARKER_COUNT_REF; i++){
		printf("%.0f;", mass[i]);
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

int cvblob(int sep){
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
	Mat invert; bitwise_not(image, invert); detector.detect(invert, keypoints);
	//detector.detect(image, keypoints);

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
		if (moment_x[i] < sep)	cout << moment_x[i] << ',' << moment_y[i] << ';';
		}
	//logfile << n;	
	cout << endl;
	return(n);
}

// parse image sequence	
int main(int argc, char **argv){
	//logfile.open("/me/log.txt");
	sprintf(filename, "%s", argv[2]);
	image_input = imread(filename, 1);
	image_height = image_input.rows;
	image_width = image_input.cols;
	//printf("%s %d %d: ",filename, image_width, image_height);
	int mrgins[MARKER_COUNT]={2048};
	
	int side1[9]={1614, 1380, 1130, 940, 760, 550, 392, 236, 75}; // right side
	int side2[9]={1590, 1320, 1106, 878, 690, 538, 390, 240, 68}; // left side
	int upper= 620;
	int lower= 130;
	
	int separator= 380;
	int row1[9]={1640, 1432, 1256, 1060, 880, 680, 528, 338, 250}; // right side
	int row2[9]={1656, 1427, 1200, 1004, 820, 664, 536, 360, 170}; // left side
	
	if (image_height && image_width) {
		// in case of rotated camera, 3 lines
		cvtColor(image_input, temp, CV_BGR2GRAY);
		transpose(temp, image);  // two lines, rotate 90 deg clockwise
		flip(image, image, 1);
		// or it is already in grayscale
		//cvtColor(image_input, image, CV_BGR2GRAY);
		//imshow("disp",image); waitKey(0);
		if (!strcmp(argv[1],"h")) calculate_histogram(atoi(argv[3]));
		else if (!strcmp(argv[1],"g")) calculate_moment_gray(atoi(argv[3]));
		else if (!strcmp(argv[1],"c")) calculate_niner(mrgins);
		else if (!strcmp(argv[1],"d")) calculate_rower(separator, row1, row2, atoi(argv[3]));
		else if (!strcmp(argv[1],"f")) calculate_ref(upper, lower, separator, side1, side2);
		else if (!strcmp(argv[1],"x")) calculate_refw(atoi(argv[3]), upper, lower, separator, side1, side2);
		else if (!strcmp(argv[1],"w")) calculate_width(atoi(argv[3]), atoi(argv[4]));
		else if (!strcmp(argv[1],"r")) read_row(atoi(argv[3]));
		else if (!strcmp(argv[1],"v")) cvblob(atoi(argv[3]));
		else {
			cout << "wrong argument for argv[1]" << endl; 
			return(1);
			}
		//
	}
	else {
		cout << "fail to open " << filename << endl;
		return(2);
		}
	return(0);
}

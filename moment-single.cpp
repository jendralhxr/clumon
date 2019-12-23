#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 20
#define MAX_OBJECTS 1024
#define MASS_MINIMUM 200
#define OBJECT_DISTANCE_MAX 120
#define MARKER_COUNT 9
using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];

Mat image, image_input, temp;
int image_height, image_width;
int framenum;
unsigned int offset;

unsigned int calculate_historam(){
	double hist_value[256]; // 8-bit depth
	offset = image_width*image_height - 1;
	histo:
	hist_value[image.data[offset]]+=1;
	offset--;
	if (offset>0) goto histo;
	
	for (int i=0; i<256; i++){
		printf("hist %d: %.0f\n",i,hist_value[i]);
		}
	}

unsigned int calculate_midline(int x1, int y1, int x2, int y2, int threshold){
	float edge1x=image_width, edge1y; // leftmost
	float edge2x=0, edge2y; // rightmost
	for (int y= y2; y>y1; y--){
		for (int x= x2; x>x1; x--){
			if (image.data[y*image_width+x] > threshold){
				if (x<edge1x){
					edge1x= x; edge1y=y;
					}
				if (x>edge2x){
					edge2x= x; edge2y=y;
					}	
				}
			} 
		}
	printf("center: (%f,%f %f,%f)\n", edge1x, edge1y, edge2x, edge2y);
	//printf("center: (%f,%f)\n",(float(edge1x)+float(edge2x))/float(2), (float(edge1y)+float(edge2y))/float(2));
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
			if ((moment_x_temp>margin[i])){
				moment_x[i]= moment_x[i]+ moment_x_temp;
				moment_y[i]= moment_y[i]+ moment_y_temp;
				mass[i]= mass[i]+ 1.0;
				break;
				}
			}
		}
	offset--;
	if (offset>image_width) goto niner;
	
	for(int i=MARKER_COUNT; i>=0; i--){
		printf("%.6lf,%.6lf;", moment_x[i]/mass[i], moment_y[i]/mass[i]);
		//cout << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << ';';
		//cout << moment_y[i] << ',' << moment_x[i] << ',' << mass[i] << ';';
		}
	cout << endl;
	return(0);
	} 

unsigned int calculate_moment_gray() {
	Mat moment_map = Mat(image_height, image_width, CV_8UC1);
	double moment_x_temp, moment_y_temp, mass_temp;
	int object_counts = 0, offset = 0;
	
// first pass
	offset = image_width*image_height - 1;
restofimage:
	if (image.data[offset] > THRESHOLD_GRAY) {
		mass_temp++;
		moment_x_temp += (offset % image_width);
		moment_y_temp += (offset / image_width);
		}
	offset--;
	if (offset>image_width) goto restofimage;
	
	cout << setprecision(8) << moment_x_temp/mass_temp << ',' << moment_y_temp/mass_temp << endl;
	return(mass_temp);
}

// parse image sequence	
int main(int argc, char **argv){
	//logfile.open("/me/log.txt");
	sprintf(filename, "%s", argv[2]);
	image_input = imread(argv[2], 1);
	image_height = image_input.rows;
	image_width = image_input.cols;
	//printf("%s %d %d: ",filename, image_width, image_height);
	
	int mrgins[9]={820, 819, 640, 639, 460, 459, 270, 269, 268};
	
	if (image_height && image_width) {
		cvtColor(image_input, image, CV_BGR2GRAY);
		//transpose(temp, image);  // two lines, rotate 90 deg clockwise
		//flip(image, image, 1);
		if (!strcmp(argv[1],"h")) calculate_historam();
		else if (!strcmp(argv[1],"c")) calculate_niner(mrgins);
		else if (!strcmp(argv[1],"m")) calculate_midline(atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
		else {
			cout << "check argv[1]" << endl; 
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



#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 20
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

unsigned int calculate_width(int thresholdval){
	double moment_x_temp, moment_y_temp;
	int x_min=image_width, x_max=0, y_min=image_height, y_max=0;
	
	offset = image_width*image_height - 1;
	width:
	if (image.data[offset] > thresholdval) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if (moment_y_temp>y_max) y_max= moment_y_temp;
		if (moment_y_temp<y_min) y_min= moment_y_temp;
		if (moment_x_temp>x_max) x_max= moment_x_temp;
		if (moment_x_temp<x_min) x_min= moment_x_temp;
		}
	offset--;
	if (offset>image_width) goto width;
	
	printf("marker: %d %d %d %d %d %d\n",x_max, x_min, y_max, y_min ,x_max-x_min, y_max-y_min);
		
	
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
		if (moment_x_temp<separator){ // first row markers
			for (int i=0; i<9; i++){
				if (moment_y_temp<margin1[i]){
					moment_x[i]+= moment_x_temp;
					moment_y[i]+= moment_y_temp;
					mass[i]++;
					break;
					}
				}
			}
		else { // second row markers
			for (int i=9; i<18; i++){
				if (moment_y_temp<margin2[i-9]){
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
	
	cout << setprecision(8) << moment_x_temp/mass_temp << ',' << moment_y_temp/mass_temp << ';';
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
	int mrgins[MARKER_COUNT]={2048};
	int separator= 282;
	int row1[9]={282, 464, 621, 804, 1002, 1208, 1417, 1647, 1936};
	int row2[9]={373, 536, 669, 827, 1013, 1195, 1423, 1660, 1941};
	
	if (image_height && image_width) {
		//cvtColor(image_input, temp, CV_BGR2GRAY);
		//transpose(temp, image); // in case of rotated camera
		cvtColor(image_input, image, CV_BGR2GRAY);
		//imshow("disp",image); waitKey(0);
		if (!strcmp(argv[1],"h")) calculate_historam();
		else if (!strcmp(argv[1],"c")) calculate_niner(mrgins);
		else if (!strcmp(argv[1],"d")) calculate_rower(separator, row1, row2);
		else if (!strcmp(argv[1],"w")) calculate_width(atoi(argv[3]));
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



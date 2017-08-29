#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 7
#define MAX_OBJECTS 1024
#define MASS_MINIMUM 200
#define OBJECT_DISTANCE_MAX 120
#define MARKER_ROWS 9
#define MARKER_COUNT 18
using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];

Mat image, image_input;
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
		cout << setprecision(8) << moment_x[i]/mass[i] << ',' << moment_y[i]/mass[i] << endl;
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
	int mrgins[MARKER_COUNT]={2048};
	int separator= 1350;
	int row1[9]={293, 450, 588, 750, 938, 1129, 1365, 1588, 1925};
	int row2[9]={263, 444, 609, 793, 995, 1206, 1404, 1627, 1950};
	
	if (image_height && image_width) {
		cvtColor(image_input, image, CV_BGR2GRAY);
		if (!strcmp(argv[1],"h")) calculate_historam();
		else if (!strcmp(argv[1],"c")) calculate_niner(mrgins);
		else if (!strcmp(argv[1],"d")) calculate_rower(separator, row1, row2);
		else {
			cout << "put either 'h' or 'c' for argv[1]" << endl; 
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



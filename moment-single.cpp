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
#define MARKER_COUNT 9
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

unsigned int calculate_niner(int m1, int m2, int m3, int m4, int m5,\
	int m6,int m7, int m8, int m9){
	double moment_x[MARKER_COUNT], moment_y[MARKER_COUNT], mass[MARKER_COUNT];
	double moment_x_temp, moment_y_temp, mass_temp;
	
	
	offset = image_width*image_height - 1;
niner:
	if (image.data[offset] > THRESHOLD_GRAY) {
		moment_x_temp = (offset % image_width);
		moment_y_temp = (offset / image_width);
		if (moment_x_temp<m1){
			moment_x[0]+= moment_x_temp;
			moment_y[0]+= moment_y_temp;
			mass[0]++;
			}
		else if (moment_x_temp<m2){
			moment_x[1]+= moment_x_temp;
			moment_y[1]+= moment_y_temp;
			mass[1]++;
			}
		else if (moment_x_temp<m3){
			moment_x[2]+= moment_x_temp;
			moment_y[2]+= moment_y_temp;
			mass[2]++;
			}
		else if (moment_x_temp<m4){
			moment_x[3]+= moment_x_temp;
			moment_y[3]+= moment_y_temp;
			mass[3]++;
			}
		else if (moment_x_temp<m5){
			moment_x[4]+= moment_x_temp;
			moment_y[4]+= moment_y_temp;
			mass[4]++;
			}
		else if (moment_x_temp<m6){
			moment_x[5]+= moment_x_temp;
			moment_y[5]+= moment_y_temp;
			mass[5]++;
			}
		else if (moment_x_temp<m7){
			moment_x[6]+= moment_x_temp;
			moment_y[6]+= moment_y_temp;
			mass[6]++;
			}
		else if (moment_x_temp<m8){
			moment_x[7]+= moment_x_temp;
			moment_y[7]+= moment_y_temp;
			mass[7]++;
			}
		else if (moment_x_temp<m9){
			moment_x[8]+= moment_x_temp;
			moment_y[8]+= moment_y_temp;
			mass[8]++;
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
	logfile.open("/me/log.txt");
	sprintf(filename, "%s", argv[1]);
	image_input = imread(argv[1], 1);
	image_height = image_input.rows;
	image_width = image_input.cols;
	//printf("%s %d %d: ",filename, image_width, image_height);
	if (image_height && image_width) {
		cvtColor(image_input, image, CV_BGR2GRAY);
		//calculate_moment_gray();
		//calculate_historam();
		calculate_niner(270, 510, 740, 1000, 1220, 1410, 1600, 1800, 2000);
	}
	else {
		cout << "fail to open " << filename << endl;
		return(1);
		}
	return(0);
}



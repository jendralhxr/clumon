#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 128
#define MAX_OBJECTS 256
#define MASS_MINIMUM 16
#define IMAGE_COUNT 1000
#define OBJECT_DISTANCE_MAX 32

using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];
int n;

Mat image, image_input;
int image_height, image_width;

unsigned int calculate_moment_gray() {
	Mat moment_map = Mat(image_height, image_width, CV_8UC1);
	double moment_x[MAX_OBJECTS], moment_y[MAX_OBJECTS], mass[MAX_OBJECTS];
	int object_counts = 0, offset = 0;
	unsigned char temp;

// first pass
	offset = image_width*image_height - 1;
restofimage:
	if (image.data[offset] > THRESHOLD_GRAY) {
		// horizontal neighbor
		if (moment_map.data[offset + 1]) moment_map.data[offset] = moment_map.data[offset + 1];
		// vertical neighbor, maybe unsafe
		else if (moment_map.data[offset + image_width]) moment_map.data[offset] = moment_map.data[offset + image_width];
		// new cluster
		else {
			object_counts++;
			moment_map.data[offset] = object_counts;
		}
		moment_x[moment_map.data[offset]] += offset % image_width;
		moment_y[moment_map.data[offset]] += offset / image_width;
		mass[moment_map.data[offset]] += 1;
	}
	else moment_map.data[offset] = 0; // may be optional, zeroing the map
	offset--;
	if (offset) goto restofimage;

/*
//second pass
	offset = 1;
secondpass:
	if (moment_map.data[offset]) {
		// horizontal neighbor
		temp = moment_map.data[offset];
		if (moment_map.data[offset - 1]>temp) {
			moment_map.data[offset] = moment_map.data[offset - 1];
			moment_x[moment_map.data[offset - 1]] += moment_x[temp];
			moment_x[temp] = 0;
			moment_y[moment_map.data[offset - 1]] += moment_y[temp];
			moment_y[temp] = 0;
			mass[moment_map.data[offset - 1]] += mass[temp];
			mass[temp] = 0;
		}

		// horizontal neighbor
		if (moment_map.data[offset + 1] > temp) {
			moment_map.data[offset] = moment_map.data[offset + 1];
			moment_x[moment_map.data[offset + 1]] += moment_x[temp];
			moment_x[temp] = 0;
			moment_y[moment_map.data[offset + 1]] += moment_y[temp];
			moment_y[temp] = 0;
			mass[moment_map.data[offset + 1]] += mass[temp];
			mass[temp] = 0;
		}
		
		// vertical neighbor, maybe unsafe
		if (offset > image_width) {
			if (moment_map.data[offset - image_width]>temp) {
				moment_map.data[offset] = moment_map.data[offset - image_width];
				moment_x[moment_map.data[offset - image_width]] += moment_x[temp];
				moment_x[temp] = 0;
				moment_y[moment_map.data[offset - image_width]] += moment_y[temp];
				moment_y[temp] = 0;
				mass[moment_map.data[offset - image_width]] += mass[temp];
				mass[temp] = 0;
			}
		}
	}
//	else moment_map.data[offset] = 0; // may be optional, zeroing the map
	offset++;
	if (offset < image_height*image_width) goto secondpass;
*/

// silly merge and sort goes here
// now we have centroid for meta-clusters
	for (int i=object_counts; i>0; i--){
		moment_x[i] /= mass[i];
		moment_y[i] /= mass[i];
		}

	
	for (int i=object_counts; i>0; i--){ // quadrature scan
		for (int j=object_counts; j>0; j--){
	//for (int i=object_counts; i>0; i--){ // for some reason, this wont work
 		//for (int j=i-i; j>0; j--){
			if ((fabs(moment_x[j]-moment_x[i])<OBJECT_DISTANCE_MAX) && \
				(fabs(moment_y[j]-moment_y[i])<OBJECT_DISTANCE_MAX) && \
				(mass[j]!=0) && (mass[i]!=0) && (j!=i)){
				//(mass[j]!=0) && (mass[i]!=0)){
				moment_x[i] = (moment_x[i]*mass[i]+moment_x[j]*mass[j])\
				/(mass[i]+mass[j]);
				moment_y[i] = (moment_y[i]*mass[i]+moment_y[j]*mass[j])\
				/(mass[i]+mass[j]);
				moment_x[j]= 0;
				moment_y[j]= 0;
				mass[j]= 0;
				}
			}
		}

	// log, number of objects and their positions
	for (int i = object_counts; i > 0; i--) {
		if ((mass[i] > MASS_MINIMUM) && (mass[i] != NAN)) \
			logfile << moment_x[i] << ',' << moment_y[i] << ',' << mass[i] << ';';
	}
	logfile << endl;

	offset = image_width*image_height - 1;
highlight:
	if (moment_map.data[offset]) moment_map.data[offset] = 100 + 2 * moment_map.data[offset];
	offset--;
	if (offset) goto highlight;
	//sprintf(filename, "D:\\zoomlenses\\2016_11_12-canon\\b%4.4d.png", n);
	//imwrite(filename, moment_map);
	return(object_counts);
}

/* Sobel-wannabe operation
   horizontal gradient:
   -5 0 +5
   -7 0 +7
   -5 0 +5

   vertical gradient:
   +5 +7 +5
    0  0  0
   -5 -7 -5
*/
int apply_gradient() {
	Mat grad_image = Mat(image_height, image_width, CV_8UC1);
	double grad_x, grad_y, grad;
	unsigned int tip_x = image_width, tip_y = image_height;
	unsigned int offset;

	offset = image_height*image_width - 1;
grad:
	if (offset%image_width == 0) grad_image.data[offset] = 200;
	else if (offset%image_width == image_width - 1) grad_image.data[offset] = 200;
	else if (offset/image_width == 0) grad_image.data[offset] = 200;
	else if (offset/image_width == image_height - 1) grad_image.data[offset] = 200;
	else {
	grad_x = \
	- 5 * (image.data[offset] - image.data[offset - 1 - image_width])\
	- 7 * (image.data[offset] - image.data[offset - 1])\
	- 5 * (image.data[offset] - image.data[offset - 1 + image_width])\
	+ 5 * (image.data[offset] - image.data[offset + 1 - image_width])\
	+ 7 * (image.data[offset] - image.data[offset + 1])\
	+ 5 * (image.data[offset] - image.data[offset + 1 - image_width]);
	grad_x /= 48; // 34 is actual value, apply some scaling
	grad_y= \
	- 5 * (image.data[offset] - image.data[offset + image_width - 1])\
	- 7 * (image.data[offset] - image.data[offset + image_width])\
	- 5 * (image.data[offset] - image.data[offset + image_width + 1])\
	+ 5 * (image.data[offset] - image.data[offset - image_width - 1])\
	+ 7 * (image.data[offset] - image.data[offset - image_width])\
	+ 5 * (image.data[offset] - image.data[offset - image_width + 1]);
	grad_y /= 48;
	grad = (grad_x + grad_y);
	grad_image.data[offset] = grad; // may need some adjustment
	}
	offset--;
	if (offset) goto grad;

	//saving
	logfile << tip_x << ',' << tip_y << endl;
	sprintf(filename, "D:\\20161125\\s%4.4d.png", n);
	imwrite(filename, grad_image);
	return(offset);
}

int apply_threshold() {
	Mat grad_image = Mat(image_height, image_width, CV_8UC1);
	double grad_x, grad_y, grad;
	unsigned int tip_x=image_width, tip_y=image_height;
	unsigned int offset;

	offset = image_height*image_width - 1;
grad:
	if (image.data[offset] > 10) {
		grad_image.data[offset] = 200;
		if (offset%image_width < tip_x) {
			tip_x = offset%image_width;
			tip_y = offset / image_width;
		}
	}
	offset--;
	if (offset) goto grad;
	
	//saving
	logfile << tip_x << ',' <<  tip_y << endl;
	sprintf(filename, "D:\\20161125\\s%4.4d.png", n);
	imwrite(filename, grad_image);
	return(offset);
}

int main()
{
	logfile.open("/me/ledball/logfile.txt");
	for (n=0; n<IMAGE_COUNT; n++){
		sprintf(filename, "/me/ledball/xiM%4.4d.png", n);
		image_input = imread(filename, 1);
		image_height = image_input.rows;
		image_width = image_input.cols;
		if (image_height || image_width) {
			cvtColor(image_input, image, CV_BGR2GRAY);
			//cvtColor(image_input, image, CV_BGR2HSV);
			//			imwrite(filename, image);
			cout << filename << ' ' << calculate_moment_gray() << endl;
			//apply_grad();
		}
		else cout << "fail to open " << filename << endl;
	}
	return 0;
}


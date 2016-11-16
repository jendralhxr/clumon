// moment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#define THRESHOLD_GRAY 100
#define THRESHOLD_VAL_UPPER 120
#define THRESHOLD_VAL_LOWER 100
#define MAX_OBJECTS 256
#define IMAGE_COUNT 100


using namespace std;
using namespace cv;

ofstream logfile;

Mat image, image_input;
unsigned int image_height, image_width;

unsigned int calculate_moment_gray() {
	Mat moment_map= Mat(image_height, image_width, CV_8UC1);
	double moment_x[MAX_OBJECTS], moment_y[MAX_OBJECTS], mass[MAX_OBJECTS];
	int object_counts=0, offset=0;
	// first line
	offset = image_width - 1;
firstline:
	if (image.data[offset] > THRESHOLD_GRAY) {
		// old label
		if (moment_map.data[offset + 1]) moment_map.data[offset] = moment_map.data[offset + 1];
		// new label
		else {
			object_counts++;
			moment_map.data[offset] = object_counts;
		}
		moment_x[object_counts] += offset % image_width;
		moment_y[object_counts] += 1;
		mass[object_counts] += 1;
	}
	else moment_map.data[offset] = 0; // may be optional, zeroing the map
	offset--;
	if (offset) goto firstline;

	// rest of image, sans first line
	offset = image_height*image_width - 1;
restofimage:
	if (image.data[offset] > THRESHOLD_GRAY) {
		// horizontal neighbor
		if (moment_map.data[offset + 1]) moment_map.data[offset] = moment_map.data[offset + 1];
		// vertical neighbor
		else if (moment_map.data[offset + image_width]) moment_map.data[offset] = moment_map.data[offset + image_width];
		// new object
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
	
	// log, number of objects and their positions
	logfile << object_counts << ';';
	for (int i = object_counts; i > 0; i--) {
		logfile << double(moment_x[i] /= mass[i]) << ',' << double(moment_y[i] /= mass[i]) << ',' << mass[i] << ';';
	}
	logfile << endl;
	return(object_counts);
}


int main()
{
	char filename[256];
	logfile.open("logfile.txt");
	for (int n=0; n< IMAGE_COUNT; n++) {
		sprintf_s(filename, "D:\\zoomlenses\\xiM%4.4d.png", n);
		image_input = imread(filename, 1);
		image_height = image_input.rows;
		image_width = image_input.cols;
		if (image_height || image_width) {
			cvtColor(image_input, image, CV_BGR2GRAY);
			//cvtColor(image_input, image, CV_BGR2HSV);
			sprintf_s(filename, "D:\\zoomlenses\\g%4.4d.png", n);
//			imwrite(filename, image);
			cout << filename << ' ' << calculate_moment_gray() << endl;
		}
		else cout << "fail to open " << filename << endl;
	}
	return 0;
}


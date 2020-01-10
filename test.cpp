// compile: g++ test.cpp  `pkg-config --libs opencv` -pthread -lpthread -std=gnu++11
//#define _GLIBCXX_USE_CXX11_ABI 0
//#define _GLIBCXX_USE_CXX17_ABI 0
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <math.h>
#include <stdlib.h>
#include "csv.h"

#define THRESHOLD_GRAY 15
#define DISTANCE 2 // px from marker edge
#define STEP 2
unsigned int offset;
unsigned int image_height, image_width, n_max;
unsigned int *centroid_x, *centroid_y, framenum;
char filename[256];

using namespace std;
using namespace cv;

Mat image, image_input, temp;

unsigned int calculate_moment(int markernumber, int x_mid, int y_mid){
	unsigned int edge_top, edge_bottom, edge_left, edge_right;
	double moment_x_temp, moment_y_temp, mass_temp;
	double centroid_x_final, centroid_y_final;
	int edge_band;
	
	// find edges
	// ytop
	edge_band= DISTANCE;
	for (edge_top= y_mid; edge_top>0; edge_top--){
		//cout << "top :" << edge_top << " " << x_mid  << " " << int (image.data[ edge_top*image_width + x_mid]) << " " <<edge_band<< endl;
		if (image.data[ edge_top*image_width + x_mid] < THRESHOLD_GRAY) {
			if (image.data[ edge_top*image_width + x_mid + STEP] > THRESHOLD_GRAY) x_mid+=STEP;
			else if (image.data[ edge_top*image_width + x_mid - STEP] > THRESHOLD_GRAY) x_mid-=STEP;
			else edge_band--;
			}
		else image.data[ edge_top*image_width + x_mid]= 255;
		if (edge_band==0) break;
		}
	// ybot
	edge_band= DISTANCE;
	for (edge_bottom= y_mid; edge_bottom<image_height; edge_bottom++){
		//cout << "bot :" << edge_bottom << " " << x_mid << " " << int (image.data[ edge_bottom*image_width + x_mid]) << " " <<edge_band<< endl;
		if (image.data[ edge_bottom*image_width + x_mid ] < THRESHOLD_GRAY) {
			if (image.data[ edge_bottom*image_width + x_mid + STEP] > THRESHOLD_GRAY) x_mid+=STEP;
			else if (image.data[ edge_bottom*image_width + x_mid - STEP] > THRESHOLD_GRAY) x_mid-=STEP;
			else edge_band--;
			}
		else image.data[ edge_bottom*image_width + x_mid ]= 255;
		if (edge_band==0) break;
		}
	// xleft
	edge_band= DISTANCE;
	for (edge_left= x_mid; edge_left>0; edge_left--){
		//cout << "left:" << y_mid << " " << edge_left << " " << int (image.data[ y_mid*image_width + edge_left]) << " " <<edge_band<< endl;
		if (image.data[ y_mid*image_width + edge_left ] < THRESHOLD_GRAY) {
			if (image.data[ (y_mid-STEP)*image_width + edge_left ] > THRESHOLD_GRAY) y_mid-=STEP;
			else if (image.data[ (y_mid+STEP)*image_width + edge_left ] > THRESHOLD_GRAY) y_mid+=STEP;
			else edge_band--;
			}
		else image.data[ y_mid*image_width + edge_left ]= 255;
		if (edge_band==0) break;
		}
	// xright
	edge_band= DISTANCE;
	for (edge_right= x_mid; edge_right<image_width; edge_right++){
		//cout << "right:" << y_mid << " " << edge_right << " " << int (image.data[ y_mid*image_width + edge_right]) << " " <<edge_band<< endl;
		if (image.data[ y_mid*image_width + edge_right ] < THRESHOLD_GRAY) {
			if (image.data[ (y_mid-STEP)*image_width + edge_right ] > THRESHOLD_GRAY) y_mid-=STEP;
			else if (image.data[ (y_mid+STEP)*image_width + edge_right ] > THRESHOLD_GRAY) y_mid+=STEP;
			else edge_band--;
			}
		else image.data[ y_mid*image_width + edge_right ]= 255;
		if (edge_band==0) break;
		}
	
	//cout << markernumber << "-l:\t" << edge_left << "\t" << x_mid << "\t" << edge_right << endl; 
	//cout << markernumber << "-v:\t" << edge_top << "\t" << y_mid  << "\t" << edge_bottom << endl; 
	// calculate moment of designated marker
	unsigned int x, y;
	mass_temp=0.0; moment_x_temp=0.0; moment_y_temp=0.0;
	for (y= edge_top; y<edge_bottom; y++){
		for (x= edge_left; x<edge_right; x++){
			if (image.data[ y*image_width + x ] > THRESHOLD_GRAY){
				mass_temp += 1.0;
				moment_x_temp += double(x);
				moment_y_temp += double(y);
				}
			}
		}
	
	// centroid position
	centroid_x_final= moment_x_temp/mass_temp;
	centroid_y_final= moment_y_temp/mass_temp;
	centroid_x[markernumber] = int (centroid_x_final);
	centroid_y[markernumber] = int (centroid_y_final);
	cout << setprecision(8) << centroid_x_final << ',' << centroid_y_final;
	return(mass_temp);
}


int main(int argc, char **argv) {
	io::CSVReader<3> in(argv[2]);
	in.read_header(io::ignore_extra_column, "marker", "x", "y");
	int a, b, c;

	// image handling
	sprintf(filename, "%s/xi%05d.tif", argv[1], framenum);
	image_input = imread(filename, 1);
	cvtColor(image_input, image, COLOR_BGR2GRAY);
	transpose(image, image);  // two lines, rotate 90 deg clockwise
	flip(image, image, 1);
	
	image_height = image.rows;
	image_width = image.cols;
	//cout << image_width << "x" << image_height << endl;
		
	// parse approximate centroid location	
	int n=0, n_max;
	while (in.read_row(a, b, c)) {
		//cout << int(a) << ": " <<b << "," << c << endl;
		n_max++;
		centroid_x= (unsigned int *) realloc(centroid_x, sizeof(unsigned int) *n_max);
		centroid_y= (unsigned int *) realloc(centroid_y, sizeof(unsigned int) *n_max);
		centroid_x[a]= b;
		centroid_y[a]= c;
		}
	
	for (framenum=0; framenum<atoi(argv[3]); framenum++){
		sprintf(filename, "%s/xi%05d.tif", argv[1], framenum);
		//printf("%s\n",filename);
		image_input = imread(filename, 1);
		cvtColor(image_input, image, COLOR_BGR2GRAY);
		transpose(image, image);  // two lines, rotate 90 deg clockwise
		flip(image, image, 1);
		cout << framenum << ";";
		for (n=0; n<n_max; n++) {
			calculate_moment(n, centroid_x[n], centroid_y[n]); 
			cout << ";";
			}
		cout << endl;
		
		imshow("wa", image);
		waitKey(0);
		}
	}       

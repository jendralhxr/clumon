// compile: g++ test.cpp  -pthread -lpthread `pkg-config --libs opencv`

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <math.h>
#include "csv.h"

#define THRESHOLD_GRAY 40
#define DISTANCE 3 // px from marker edge
unsigned int offset;
unsigned int image_height, image_width;

using namespace std;
using namespace cv;

Mat image, image_input, temp;

unsigned int calculate_moment(int markernumber, int x_mid, int y_mid){
	unsigned int edge_top, edge_bottom, edge_left, edge_right;
	double moment_x_temp, moment_y_temp, mass_temp;
	int edge_band;
	
	// find edges
	// ytop
	edge_band= DISTANCE;
	for (edge_top= y_mid; edge_top>0; edge_top--){
		//cout << "top :" << edge_top << " " << x_mid  << " " << int (image.data[ edge_top*image_width + x_mid]) << " " <<edge_band<< endl;
		if (image.data[ edge_top*image_width + x_mid] < THRESHOLD_GRAY) edge_band--;
		else image.data[ edge_top*image_width + x_mid]= 255;
		if (edge_band==0) break;
		}
	// ybot
	edge_band= DISTANCE;
	for (edge_bottom= y_mid; edge_bottom<image_height; edge_bottom++){
		//cout << "bot :" << edge_bottom << " " << x_mid << " " << int (image.data[ edge_bottom*image_width + x_mid]) << " " <<edge_band<< endl;
		if (image.data[ edge_bottom*image_width + x_mid ] < THRESHOLD_GRAY) edge_band--;
		else image.data[ edge_bottom*image_width + x_mid ]= 255;
		if (edge_band==0) break;
		}
	// xleft
	edge_band= DISTANCE;
	for (edge_left= x_mid; edge_left>0; edge_left--){
		//cout << "left:" << y_mid << " " << edge_left << " " << int (image.data[ y_mid*image_width + edge_left]) << " " <<edge_band<< endl;
		if (image.data[ y_mid*image_width + edge_left ] < THRESHOLD_GRAY) edge_band--;
		else image.data[ y_mid*image_width + edge_left ]= 255;
		if (edge_band==0) break;
		}
	// xright
	edge_band= DISTANCE;
	for (edge_right= x_mid; edge_right<image_width; edge_right++){
		//cout << "right:" << y_mid << " " << edge_right << " " << int (image.data[ y_mid*image_width + edge_right]) << " " <<edge_band<< endl;
		if (image.data[ y_mid*image_width + edge_right ] < THRESHOLD_GRAY) edge_band--;
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
	cout << setprecision(8) << moment_x_temp/mass_temp << ',' << moment_y_temp/mass_temp;
	return(mass_temp);
}


int main(int argc, char **argv) {
	io::CSVReader<3> in(argv[2]);
	in.read_header(io::ignore_extra_column, "marker", "x", "y");
	int a, b, c;

	// image handling
	//cvtColor(image_input, temp, CV_BGR2GRAY);
	image_input = imread(argv[1], 1);
	cvtColor(image_input, image, COLOR_BGR2GRAY);
	transpose(image, image);  // two lines, rotate 90 deg clockwise
	flip(image, image, 1);
	
	
	image_height = image.rows;
	image_width = image.cols;
	//cout << image_width << "x" << image_height << endl;
		
	// parse approximate centroid location	
	int n = 0;
	while (in.read_row(a, b, c)) {
		//cout << int(a) << ": " <<b << "," << c << endl;
		calculate_moment(a, b, c); 
		cout << ';'; 
		n++;
		}
	cout << endl;
	
	//imshow("wa", image);
	//waitKey(0);
	}       

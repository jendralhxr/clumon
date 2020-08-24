#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat input, img, result;

/// Function Headers
void blending(double cx, double cy);

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and template
  
  input = imread( argv[1], 1 );
  cvtColor(input, img, COLOR_BGR2GRAY);
  result= img.clone();
  
  blending(0.5, 0.8);
  
  imshow("in", img);
  imshow("out", result);
  //imwrite("in.png", img);
  //imwrite("out.png", result);
  
  
  waitKey(0);
  return 0;
}

void blending(double cx, double cy){
	double temp;
	for (int j=0; j<img.rows-1; j++){
		for (int i=0; i<img.cols-1; i++){
				temp= 	(1-cy)*(1-cx) * img.data[j*img.cols+i] +\
						(1-cy)*(  cx) * img.data[j*img.cols+i+1] +\
						(  cy)*(1-cx) * img.data[(j+1)*img.cols+i] +\
						(  cy)*(  cx) * img.data[(j+1)*img.cols+i+1];
				result.data[j*img.cols+i]= char(temp);
				
			}
		}
	}

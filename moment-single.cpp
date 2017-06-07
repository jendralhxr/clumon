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

using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];
int n;

VideoCapture video;
int framenum;
unsigned int offset;

Mat image, image_input;
int image_height, image_width;
Mat thre_image, thre_image_temp;
Mat grad_image;
Mat final_image;

unsigned int calculate_moment_gray() {
	Mat moment_map = Mat(image_height, image_width, CV_8UC1);
	double moment_x_temp, moment_y_temp, mass_temp;
	int object_counts = 0, offset = 0;
	unsigned char temp;
	double temp_double;

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
		calculate_moment_gray();
		//calculate_historam();
	}
	else {
		cout << "fail to open " << filename << endl;
		return(1);
		}
	return(0);
}
// */

 /*
// parse video
int main(int argc, char **argv){
	video.open(argv[1]);
	if (!video.isOpened()) return(-1);
	else {
		sprintf(filename, "%s.log.txt",argv[1]);
		logfile.open(filename);
		}
	Mat frame;
	Mat thre_image;
	
	for (framenum=0; framenum<atoi(argv[2]); framenum){
		if (video.read(frame)){
			if (framenum==0) {
				image_height= frame.rows;
				image_width= frame.cols;
				printf("image size %dx%d\n", image_width, image_height);				}
			framenum++;
			sprintf(filename,"val%04d.png",framenum);
			//imwrite(filename, frame);
			//printf("%d\n",framenum++);
			cv::cvtColor(frame, image, CV_BGR2GRAY);
			//apply_threshold();
			//printf(": %d\n",calculate_moment_gray());
			printf("%d: %d\n",framenum, cvblob());
			}
			else break;
		}
	printf("%d frames processed\n", framenum);
	return(0);
	}

 */

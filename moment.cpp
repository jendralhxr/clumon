#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#define THRESHOLD_GRAY 100
#define MAX_OBJECTS 1024
#define MASS_MINIMUM 20
#define IMAGE_COUNT 50
#define OBJECT_DISTANCE_MAX 40

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
	double moment_x[MAX_OBJECTS], moment_y[MAX_OBJECTS], mass[MAX_OBJECTS];
	double moment_x_temp, moment_y_temp, mass_temp;
	int object_counts = 0, offset = 0;
	unsigned char temp;
	double temp_double;

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
		moment_x[moment_map.data[offset]] += (offset % image_width);
		moment_y[moment_map.data[offset]] += (offset / image_width);
		mass[moment_map.data[offset]]+=1;
	}
	else moment_map.data[offset] = 0; // may be optional, zeroing the map
	offset--;
	if (offset>image_width) goto restofimage;
	sprintf(filename,"val%04d.png",framenum);
	//imwrite(filename, moment_map);
		
// silly merge and sort goes here
// now we have centroid for meta-clusters to mess with
	for (int i=object_counts; i>0; i--){
		moment_x[i] /= mass[i];
		moment_y[i] /= mass[i];
		//printf("x%lf y%lf m%lf \n",moment_x[i], moment_y[i], mass[i]);
		}
		
	// quadrature scan
	for (int i=object_counts; i>0; i--){
 		for (int j=i-1; j>0; j--){
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
				mass[i]+= mass[j];
				mass[j]= 0;
				}
			}
		}

// little sorting
	for (int i= object_counts; i>0; i--){
		for (int j=i-1; j>0; j--){
			if (moment_x[i] > moment_x[j]){
				moment_x_temp= moment_x[i];
				moment_y_temp= moment_y[i];
				mass_temp= mass[i];
				moment_x[i]= moment_x[j];
				moment_y[i]= moment_y[j];
				mass[i]= mass[j];
				moment_x[j]= moment_x_temp;
				moment_y[j]= moment_y_temp;
				mass[j]= mass_temp;
				}
			}
		}

int start_x=0, start_y=0, stop_x=0, stop_y=0;

	// log, number of objects and their positions
	for (int i = object_counts; i > 0; i--) {
		if ((mass[i] > MASS_MINIMUM) && !isnanf(moment_x[i]) && !isnanf(moment_y[i]) \
		&& (moment_y[i]<400)&& (moment_x[i]>900)){
//			logfile << moment_x[i] << ',' << moment_y[i] << ',' << mass[i] <<';';
			logfile << moment_x[i] << ',' << moment_y[i] << ';';
//			logfile << moment_x[i] << ';';
	
	/* drawing line
	
	if (start_x==0) start_x= int(moment_x[i]);
	else start_x= int(stop_x);
	if (start_y==0) start_y= int(moment_y[i]);
	else start_y= int(stop_y);
	stop_x= int(moment_x[i]);
	stop_y= int(moment_y[i]);
	
	line( moment_map, 
        cvPoint(start_x, start_y),
        cvPoint(stop_x, stop_y),
        Scalar(255,255,255), 2, 8 , 0);	*/
	}
	}
	logfile << endl;

	/*offset = image_width*image_height - 1;
highlight:
	if (moment_map.data[offset]) moment_map.data[offset] = 2 * moment_map.data[offset] + 100;
	offset--;
	if (offset) goto highlight;
	//sprintf(filename, "m%4.4d.png", n);
	//imwrite(filename, moment_map);
	return(object_counts);*/
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
	grad_image = Mat(image_height, image_width, CV_8UC1);
	double grad_x, grad_y, grad;
	unsigned int offset;

	offset = image_height*image_width - 1;
grad:
	if (offset%image_width == 0) grad_image.data[offset] = 00;
	else if (offset%image_width == image_width - 1) grad_image.data[offset] = 00;
	else if (offset/image_width == 0) grad_image.data[offset] = 00;
	else if (offset/image_width == image_height - 1) grad_image.data[offset] = 00;
	else {
	grad_x = \
	- 1 * (image.data[offset] - image.data[offset - 1 - image_width])\
	- 1 * (image.data[offset] - image.data[offset - 1])\
	- 1 * (image.data[offset] - image.data[offset - 1 + image_width])\
	+ 1 * (image.data[offset] - image.data[offset + 1 - image_width])\
	+ 1 * (image.data[offset] - image.data[offset + 1])\
	+ 1 * (image.data[offset] - image.data[offset + 1 + image_width]);
	grad_x = fabs(grad_x);
	- 1 * (image.data[offset] - image.data[offset + image_width - 1])\
	- 1 * (image.data[offset] - image.data[offset + image_width])\
	- 1 * (image.data[offset] - image.data[offset + image_width + 1])\
	+ 1 * (image.data[offset] - image.data[offset - image_width - 1])\
	+ 1 * (image.data[offset] - image.data[offset - image_width])\
	+ 1 * (image.data[offset] - image.data[offset - image_width + 1]);
	grad_y = fabs(grad_y);
	
	grad = 2*sqrt(grad_x*grad_x + grad_y*grad_y);
//	grad = grad_y;
	grad_image.data[offset] = grad; // may need some adjustment
	}
	offset--;
	if (offset) goto grad;

	//saving
	sprintf(filename, "/me/16meter/freerun/s%4.4d.png", n);
	//imwrite(filename, grad_image);
	return(offset);
}

int apply_canny(){
	int edgeThresh = 1;
	int lowThreshold= 9;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	Mat detected_edges;
	blur(image, detected_edges, Size(3,3) );
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	//grad_image= Scalar::all(0);
	
	sprintf(filename, "/me/16meter/freerun/s%4.4d.png", n);
	//imwrite(filename, detected_edges);
	detected_edges.copyTo(grad_image);
	return(0);
	}
	
int apply_threshold() {
	thre_image = Mat(image_height, image_width, CV_8UC1);

	offset = image_height*image_width - 1;
thre:
	//if (thre_image.data[offset] > 11)  thre_image.data[offset] *= 12;
	if (image.data[offset] > THRESHOLD_GRAY) thre_image.data[offset] = 200;
	else thre_image.data[offset] = 0;
	offset--;
	if (offset) goto thre;
	
	//saving
	sprintf(filename, "b%4.4d.png", framenum);
	thre_image.copyTo(thre_image_temp);
	//imwrite(filename, thre_image);
	return(offset);
}

int merge(){ // dumb pixel-wise edge detection
	final_image = Mat(image_height, image_width, CV_8UC1);
	double moment_x, moment_y, mass;
	unsigned int offset;
	unsigned neighbor;	
	double edge;
	
	offset = image_height*image_width - 1;
mask:
	if (thre_image_temp.data[offset] && (grad_image.data[offset] > 20)) {
		final_image.data[offset]= 200;
		moment_x += offset % image_width;
		moment_y += offset / image_width;
		mass += 1;
	}
	else final_image.data[offset]= 0;
	offset--;
	if (offset) goto mask;
	logfile << moment_x << ',' << moment_y <<  ',' << mass << endl;
	sprintf(filename, "/me/16meter/freerun/m%4.4d.png", n);
	//imwrite(filename, final_image);
	return(int(mass));
	}

int cvblob(){
	float moment_x[MAX_OBJECTS], moment_y[MAX_OBJECTS], mass[MAX_OBJECTS];
	float moment_x_temp, moment_y_temp, mass_temp;
	// Set up the detector with default parameters.
	SimpleBlobDetector detector;
	//SimpleBlobDetector::Params params;
	//params.filterByCircularity = false;
	//params.filterByConvexity = false;
	//params.filterByInertia = false;
	//SimpleBlobDetector detector(params);

	// Detect blobs.
	std::vector<KeyPoint> keypoints;
	Mat invert;
	bitwise_not(image, invert);
	detector.detect(invert, keypoints);
	//detector.detect(image, keypoints);

	int n=0; 
	for (std::vector<KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end(); ++it){
		//cout << it->pt.x  << ';' << it->pt.y << endl;
		moment_x[n]= it->pt.x;
		moment_y[n]= it->pt.y;
		mass[n]= it->size;
		n++;
		} //  std::cout << ' ' << *it;	
	
	// little sorting
	for (int i=n-1; i>=0; i--){
		for (int j=i-1; j>=0; j--){
			if (moment_x[i] > moment_x[j]){
				moment_x_temp= moment_x[i];
				moment_y_temp= moment_y[i];
				mass_temp= mass[i];
				moment_x[i]= moment_x[j];
				moment_y[i]= moment_y[j];
				mass[i]= mass[j];
				moment_x[j]= moment_x_temp;
				moment_y[j]= moment_y_temp;
				mass[j]= mass_temp;
				}
			}
		}
	
	// the log	
	for (int i=n-1; i>=0; i--){
		//logfile << moment_x[i] << ',' << moment_y[i] << ',' << mass[i] <<';';
		logfile << moment_x[i] << ',' << moment_y[i] << ';';
		}
	logfile << endl;
	return(n);
	}

// /*
// parse image sequence	
int main(int argc, char **argv){
	logfile.open("/me/log.txt");
	printf("num: %d ",atoi(argv[1]));
	
	for (framenum=0; framenum<atoi(argv[2]); framenum++){
		sprintf(filename, "%s/xi%4.4d.tif", argv[1], framenum);
		image_input = imread(filename, 1);
		image_height = image_input.rows;
		image_width = image_input.cols;
		printf("%d: ",framenum);
		//printf("%s %d %d: ",filename, image_width, image_height);
		if (image_height && image_width) {
			cvtColor(image_input, image, CV_BGR2GRAY);
			//cvtColor(image_input, image, CV_BGR2HSV);
			//		imwrite(filename, image);
			printf("%d\n",cvblob());
			//cout << filename << ' ' << calculate_moment_gray() << endl;
			//cout << filename << ' ' << apply_threshold() << endl;
			//cout << filename << ' ' << apply_gradient() << endl;
			//cout << filename << apply_canny() << endl;
			//cout << filename << ' ' << merge() << endl;
			
		}
		else cout << "fail to open " << filename << endl;
	}
	return 0;
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

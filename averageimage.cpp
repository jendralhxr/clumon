#include <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

using namespace std;
using namespace cv;

ofstream logfile;
char filename[256];

Mat image, temp;
int image_height, image_width;
int framenum;
unsigned int offset;
double **block;

using namespace cv;

// parse image sequence	
int main(int argc, char **argv){
	//logfile.open("/me/log.txt");
	int imagecount;
	imagecount= atoi(argv[3]) - atoi(argv[2]);
	if (imagecount<0) return(4);
	else printf("processing average of %d images\n", imagecount);
	
	sprintf(filename, "%s%05d.tif", argv[1], atoi(argv[2]));
	image = imread(filename, 1);
	temp = imread(filename, 1);
	image_height = image.rows;
	image_width = image.cols;
	printf("size: %d %d\n",image_width, image_height);
	
	int i, j;
	block =  (double**) malloc(sizeof(double *) * image_height);
	for (j=0; j<image_height; j++){
		block[j] = (double*) malloc(sizeof(double)* image_width);
		}
		
	for (int num=atoi(argv[2]); num<atoi(argv[3]); num++){
		sprintf(filename, "%s%05d.tif", argv[1], num);
		printf("%s\n",filename);
		image = imread(filename, 1);
		for (j=0; j<image_height; j++){
			for (i=0; i<image_width; i++){
				block[j][i]+= image.data[image_width*j + i]; 
				}
			}
		}
	
	for (j=0; j<image_height; j++){
		for (i=0; i<image_width; i++){
			temp.data[image_width*j + i]= (char) (block[j][i] / imagecount); 
			}
		}
	imwrite("average.tif", temp);	
		
	
	return(0);
}



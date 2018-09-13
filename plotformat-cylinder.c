#include <stdio.h>
#include <stdlib.h>
#define MULTIPLIER_RAD 10

FILE *input;
FILE *output;
FILE *output2;
int linecount;
char filename[128], filename2[128];
char linebuffer[256];

int parse9singleside(){
	float point[10];
	int t;
	for (t=0; t<linecount; t++){
		fgets(linebuffer, 256, input);
//		printf("%d: %s",t, linebuffer);
		sscanf(linebuffer,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",\
		&point[0],\
		&point[1], &point[2], &point[3],\
		&point[4], &point[5], &point[6],\
		&point[7], &point[8], &point[9]);
		
		fprintf(output,"# t=%d\n",t);
		// 0	72	122	172	222	272	322	372	414
		// gnuplot requires theta-z-radius
		// theta is in radian, z is in cm, radius is in mm (before  multiplier) 
		fprintf(output,"%f %f %f\n", point[0], -7.0, point[1]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 65.0, point[2]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 115.0, point[3]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 165.0, point[4]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 215.0, point[5]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 265.0, point[6]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 315.0, point[7]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 365.0, point[8]*MULTIPLIER_RAD);
		fprintf(output,"%f %f %f\n", point[0], 407.0, point[9]*MULTIPLIER_RAD);
		fprintf(output,"\n");
		}
		return(t);
}

int main(int argc, char **argv){
	input= fopen(argv[1], "r");
	printf("opening intput file %s\n",argv[1]);
	if (input==NULL){
		perror("error opening input file");
		return(1);
		}
	output= fopen(argv[2], "w");
	linecount= atoi(argv[3]);
	if (output==NULL){
		perror("error opening output file");
		return(1);
		}
	else printf("opening output file %s for %d lines\n",argv[2], linecount);
	
	parse9singleside();
	fclose(output);
	}

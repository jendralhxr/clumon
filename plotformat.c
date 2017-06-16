#include <stdio.h>
#define FRAMECOUNT 3
#define MULTIPLIER 80 // bear in mind abount the cm and mm :D
FILE *xy;

int main(int argc, char **argv){
	xy= fopen(argv[1], "r");
	
	float point[18];
	fscanf(xy,"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f\n",\
		&point[0], &point[1], &point[2], &point[3],\
		&point[4], &point[5], &point[6], &point[7],\
		&point[8], &point[9], &point[10], &point[11],\
		&point[12], &point[13], &point[14], &point[15],\
		&point[16], &point[17]);
	int i,t;
	for (t=0; t<FRAMECOUNT; t++){
		printf("# t=%d\n",t);
		printf("%f %f %f\n", 0.0,   MULTIPLIER*point[0], MULTIPLIER*point[1]);
		printf("%f %f %f\n", 0.0,   MULTIPLIER*point[0]+32, MULTIPLIER*point[1]);
		printf("%f %f %f\n", 50.0,  MULTIPLIER*point[2], MULTIPLIER*point[3]);
		printf("%f %f %f\n", 50.0,  MULTIPLIER*point[2]+32, MULTIPLIER*point[3]);
		printf("%f %f %f\n", 100.0, MULTIPLIER*point[4], MULTIPLIER*point[5]);
		printf("%f %f %f\n", 100.0, MULTIPLIER*point[4]+32, MULTIPLIER*point[5]);
		printf("%f %f %f\n", 150.0, MULTIPLIER*point[6], MULTIPLIER*point[7]);
		printf("%f %f %f\n", 150.0, MULTIPLIER*point[6]+32, MULTIPLIER*point[7]);
		printf("%f %f %f\n", 200.0, MULTIPLIER*point[8], MULTIPLIER*point[9]);
		printf("%f %f %f\n", 200.0, MULTIPLIER*point[8]+32, MULTIPLIER*point[9]);
		printf("%f %f %f\n", 250.0, MULTIPLIER*point[10], MULTIPLIER*point[11]);
		printf("%f %f %f\n", 250.0, MULTIPLIER*point[10]+32, MULTIPLIER*point[11]);
		printf("%f %f %f\n", 300.0, MULTIPLIER*point[12], MULTIPLIER*point[13]);
		printf("%f %f %f\n", 300.0, MULTIPLIER*point[12]+32, MULTIPLIER*point[13]);
		printf("%f %f %f\n", 350.0, MULTIPLIER*point[14], MULTIPLIER*point[15]);
		printf("%f %f %f\n", 350.0, MULTIPLIER*point[14]+32, MULTIPLIER*point[15]);
		printf("%f %f %f\n", 400.0, MULTIPLIER*point[16], MULTIPLIER*point[17]);
		printf("%f %f %f\n", 400.0, MULTIPLIER*point[16]+32, MULTIPLIER*point[17]);
		printf("\n");
		
		}
		
	}

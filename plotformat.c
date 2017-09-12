#define MULTIPLIER 80 // bear in mind abount the cm and mm :D
#define MULTIPLIER_LATERAL 5
#define MULTIPLIER_VERTICAL 10
#include <stdio.h>

FILE *input;
FILE *output;
int framecount;
char filename[128];

int parse18pointsinput(){
	float point[36];
	int i,t;
	for (t=0; t<framecount; t++){
		fscanf(input,"%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f\n",\
		&point[0], &point[1], &point[2], &point[3],\
		&point[4], &point[5], &point[6], &point[7],\
		&point[8], &point[9], &point[10], &point[11],\
		&point[12], &point[13], &point[14], &point[15],\
		&point[16], &point[17], &point[18], &point[19],\
		&point[20], &point[21], &point[22], &point[23],\
		&point[24], &point[25], &point[26], &point[27],\
		&point[28], &point[29], &point[30], &point[31],\
		&point[32], &point[33], &point[34], &point[35]);
		sprintf(filename, "deck%04d", t);
		output= fopen(filename, "w");
		fprintf(output,"# t=%d\n",t);
		fprintf(output,"%f %f %f\n", 0.0,   MULTIPLIER_LATERAL*point[0], 	MULTIPLIER_VERTICAL*point[1]);
		fprintf(output,"%f %f %f\n\n", 0.0,   MULTIPLIER_LATERAL*point[18]+32, MULTIPLIER_VERTICAL*point[19]);
		fprintf(output,"%f %f %f\n", 50.0,  MULTIPLIER_LATERAL*point[2], 	MULTIPLIER_VERTICAL*point[3]);
		fprintf(output,"%f %f %f\n\n", 50.0,  MULTIPLIER_LATERAL*point[20]+32, MULTIPLIER_VERTICAL*point[21]);
		fprintf(output,"%f %f %f\n", 100.0, MULTIPLIER_LATERAL*point[4], 	MULTIPLIER_VERTICAL*point[5]);
		fprintf(output,"%f %f %f\n\n", 100.0, MULTIPLIER_LATERAL*point[22]+32, MULTIPLIER_VERTICAL*point[23]);
		fprintf(output,"%f %f %f\n", 150.0, MULTIPLIER_LATERAL*point[6], 	MULTIPLIER_VERTICAL*point[7]);
		fprintf(output,"%f %f %f\n\n", 150.0, MULTIPLIER_LATERAL*point[24]+32, MULTIPLIER_VERTICAL*point[25]);
		fprintf(output,"%f %f %f\n", 200.0, MULTIPLIER_LATERAL*point[8], 	MULTIPLIER_VERTICAL*point[9]);
		fprintf(output,"%f %f %f\n\n", 200.0, MULTIPLIER_LATERAL*point[26]+32, MULTIPLIER_VERTICAL*point[27]);
		fprintf(output,"%f %f %f\n", 250.0, MULTIPLIER_LATERAL*point[10], 	MULTIPLIER_VERTICAL*point[11]);
		fprintf(output,"%f %f %f\n\n", 250.0, MULTIPLIER_LATERAL*point[28]+32, MULTIPLIER_VERTICAL*point[29]);
		fprintf(output,"%f %f %f\n", 300.0, MULTIPLIER_LATERAL*point[12], 	MULTIPLIER_VERTICAL*point[13]);
		fprintf(output,"%f %f %f\n\n", 300.0, MULTIPLIER_LATERAL*point[30]+32, MULTIPLIER_VERTICAL*point[31]);
		fprintf(output,"%f %f %f\n", 350.0, MULTIPLIER_LATERAL*point[14], 	MULTIPLIER_VERTICAL*point[15]);
		fprintf(output,"%f %f %f\n\n", 350.0, MULTIPLIER_LATERAL*point[32]+32, MULTIPLIER_VERTICAL*point[33]);
		fprintf(output,"%f %f %f\n", 400.0, MULTIPLIER_LATERAL*point[16], 	MULTIPLIER_VERTICAL*point[17]);
		fprintf(output,"%f %f %f\n\n", 400.0, MULTIPLIER_LATERAL*point[34]+32, MULTIPLIER_VERTICAL*point[35]);
		fprintf(output,"\n");
		fclose(output);
		}
}

int main(int argc, char **argv){
	input= fopen(argv[1], "r");
	framecount= atoi(argv[2]);
	parse18pointsinput();
	}

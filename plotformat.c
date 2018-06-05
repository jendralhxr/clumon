#define MULTIPLIER 80 // bear in mind abount the cm and mm :D
#define MULTIPLIER_LATERAL 10
#define MULTIPLIER_VERTICAL 10
#include <stdio.h>
#include <stdlib.h>

FILE *input;
FILE *output;
FILE *output2;
int framecount;
char filename[128], filename2[128];

int parse18dualside(){
	float point[36];
	int i,t;
	for (t=0; t<framecount; t++){
		fscanf(input,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",\
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
		// 0	72	122	172	222	272	322	372	414
		fprintf(output,"%f %f %f\n", -7.0,   MULTIPLIER_LATERAL*point[0], 	MULTIPLIER_VERTICAL*point[1]);
		fprintf(output,"%f %f %f\n\n", -7.0,   MULTIPLIER_LATERAL*point[18]+32, MULTIPLIER_VERTICAL*point[19]);
		fprintf(output,"%f %f %f\n", 65.0,  MULTIPLIER_LATERAL*point[2], 	MULTIPLIER_VERTICAL*point[3]);
		fprintf(output,"%f %f %f\n\n", 65.0,  MULTIPLIER_LATERAL*point[20]+32, MULTIPLIER_VERTICAL*point[21]);
		fprintf(output,"%f %f %f\n", 115.0, MULTIPLIER_LATERAL*point[4], 	MULTIPLIER_VERTICAL*point[5]);
		fprintf(output,"%f %f %f\n\n", 115.0, MULTIPLIER_LATERAL*point[22]+32, MULTIPLIER_VERTICAL*point[23]);
		fprintf(output,"%f %f %f\n", 165.0, MULTIPLIER_LATERAL*point[6], 	MULTIPLIER_VERTICAL*point[7]);
		fprintf(output,"%f %f %f\n\n", 165.0, MULTIPLIER_LATERAL*point[24]+32, MULTIPLIER_VERTICAL*point[25]);
		fprintf(output,"%f %f %f\n", 215.0, MULTIPLIER_LATERAL*point[8], 	MULTIPLIER_VERTICAL*point[9]);
		fprintf(output,"%f %f %f\n\n", 215.0, MULTIPLIER_LATERAL*point[26]+32, MULTIPLIER_VERTICAL*point[27]);
		fprintf(output,"%f %f %f\n", 265.0, MULTIPLIER_LATERAL*point[10], 	MULTIPLIER_VERTICAL*point[11]);
		fprintf(output,"%f %f %f\n\n", 265.0, MULTIPLIER_LATERAL*point[28]+32, MULTIPLIER_VERTICAL*point[29]);
		fprintf(output,"%f %f %f\n", 315.0, MULTIPLIER_LATERAL*point[12], 	MULTIPLIER_VERTICAL*point[13]);
		fprintf(output,"%f %f %f\n\n", 315.0, MULTIPLIER_LATERAL*point[30]+32, MULTIPLIER_VERTICAL*point[31]);
		fprintf(output,"%f %f %f\n", 365.0, MULTIPLIER_LATERAL*point[14], 	MULTIPLIER_VERTICAL*point[15]);
		fprintf(output,"%f %f %f\n\n", 365.0, MULTIPLIER_LATERAL*point[32]+32, MULTIPLIER_VERTICAL*point[33]);
		fprintf(output,"%f %f %f\n", 407.0, MULTIPLIER_LATERAL*point[16], 	MULTIPLIER_VERTICAL*point[17]);
		fprintf(output,"%f %f %f\n\n", 407.0, MULTIPLIER_LATERAL*point[34]+32, MULTIPLIER_VERTICAL*point[35]);
		fprintf(output,"\n");
		fclose(output);
		
		sprintf(filename2, "deck%04dn", t);
		output2= fopen(filename2, "w");
		fprintf(output2,"# t=%d\n",t);
		// 0	72	122	172	222	272	315	372	407
		fprintf(output2,"%f %f %f\n", -7.0,   MULTIPLIER_LATERAL*point[0], 	MULTIPLIER_VERTICAL*point[1]);
		fprintf(output2,"%f %f %f\n", 65.0,  MULTIPLIER_LATERAL*point[2], 	MULTIPLIER_VERTICAL*point[3]);
		fprintf(output2,"%f %f %f\n", 115.0, MULTIPLIER_LATERAL*point[4], 	MULTIPLIER_VERTICAL*point[5]);
		fprintf(output2,"%f %f %f\n", 165.0, MULTIPLIER_LATERAL*point[6], 	MULTIPLIER_VERTICAL*point[7]);
		fprintf(output2,"%f %f %f\n", 215.0, MULTIPLIER_LATERAL*point[8], 	MULTIPLIER_VERTICAL*point[9]);
		fprintf(output2,"%f %f %f\n", 265.0, MULTIPLIER_LATERAL*point[10], 	MULTIPLIER_VERTICAL*point[11]);
		fprintf(output2,"%f %f %f\n", 315.0, MULTIPLIER_LATERAL*point[12], 	MULTIPLIER_VERTICAL*point[13]);
		fprintf(output2,"%f %f %f\n", 365.0, MULTIPLIER_LATERAL*point[14], 	MULTIPLIER_VERTICAL*point[15]);
		fprintf(output2,"%f %f %f\n", 407.0, MULTIPLIER_LATERAL*point[16], 	MULTIPLIER_VERTICAL*point[17]);
		fprintf(output2,"\n");
		fclose(output2);
		}
}

int parse16singleside(){
	float point[32];
	int i,t;
	for (t=0; t<framecount; t++){
		fscanf(input,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",\
		&point[0], &point[1], &point[2], &point[3],\
		&point[4], &point[5], &point[6], &point[7],\
		&point[8], &point[9], &point[10], &point[11],\
		&point[12], &point[13], &point[14], &point[15],\
		&point[16], &point[17], &point[18], &point[19],\
		&point[20], &point[21], &point[22], &point[23],\
		&point[24], &point[25], &point[26], &point[27],\
		&point[28], &point[29], &point[30], &point[31]);
	
		sprintf(filename, "deck%04d", t);
		output= fopen(filename, "w");
		fprintf(output,"# t=%d\n",t);
		fprintf(output,"%f %f %f\n", 	0.0,   	MULTIPLIER_LATERAL*point[0], 		MULTIPLIER_VERTICAL*point[1]);
		fprintf(output,"%f %f %f\n\n", 	0.0,   	MULTIPLIER_LATERAL*point[0]+32,		MULTIPLIER_VERTICAL*point[1]);
		fprintf(output,"%f %f %f\n", 	26.7,	MULTIPLIER_LATERAL*point[2], 		MULTIPLIER_VERTICAL*point[3]);
		fprintf(output,"%f %f %f\n\n", 	26.7,   MULTIPLIER_LATERAL*point[2]+32,		MULTIPLIER_VERTICAL*point[3]);
		fprintf(output,"%f %f %f\n", 	53.3,  	MULTIPLIER_LATERAL*point[4], 		MULTIPLIER_VERTICAL*point[5]);
		fprintf(output,"%f %f %f\n\n", 	53.3,  	MULTIPLIER_LATERAL*point[4]+32, 	MULTIPLIER_VERTICAL*point[5]);
		fprintf(output,"%f %f %f\n", 	80.0,   MULTIPLIER_LATERAL*point[6], 		MULTIPLIER_VERTICAL*point[7]);
		fprintf(output,"%f %f %f\n\n", 	80.0,   MULTIPLIER_LATERAL*point[6]+32, 	MULTIPLIER_VERTICAL*point[7]);
		fprintf(output,"%f %f %f\n", 	106.7, 	MULTIPLIER_LATERAL*point[8], 		MULTIPLIER_VERTICAL*point[9]);
		fprintf(output,"%f %f %f\n\n", 	106.7, 	MULTIPLIER_LATERAL*point[8]+32, 	MULTIPLIER_VERTICAL*point[9]);
		fprintf(output,"%f %f %f\n", 	133.3, 	MULTIPLIER_LATERAL*point[10], 		MULTIPLIER_VERTICAL*point[11]);
		fprintf(output,"%f %f %f\n\n", 	133.3, 	MULTIPLIER_LATERAL*point[10]+32, 	MULTIPLIER_VERTICAL*point[11]);
		fprintf(output,"%f %f %f\n", 	160.0, 	MULTIPLIER_LATERAL*point[12], 		MULTIPLIER_VERTICAL*point[13]);
		fprintf(output,"%f %f %f\n\n", 	160.0, 	MULTIPLIER_LATERAL*point[12]+32, 	MULTIPLIER_VERTICAL*point[13]);
		fprintf(output,"%f %f %f\n", 	186.7, 	MULTIPLIER_LATERAL*point[14], 		MULTIPLIER_VERTICAL*point[15]);
		fprintf(output,"%f %f %f\n\n", 	186.7, 	MULTIPLIER_LATERAL*point[14]+32, 	MULTIPLIER_VERTICAL*point[15]);
		fprintf(output,"%f %f %f\n", 	213.3, 	MULTIPLIER_LATERAL*point[16], 		MULTIPLIER_VERTICAL*point[17]);
		fprintf(output,"%f %f %f\n\n", 	213.3, 	MULTIPLIER_LATERAL*point[16]+32, 	MULTIPLIER_VERTICAL*point[17]);
		fprintf(output,"%f %f %f\n", 	240.0, 	MULTIPLIER_LATERAL*point[18], 		MULTIPLIER_VERTICAL*point[19]);
		fprintf(output,"%f %f %f\n\n", 	240.0, 	MULTIPLIER_LATERAL*point[18]+32, 	MULTIPLIER_VERTICAL*point[19]);
		fprintf(output,"%f %f %f\n", 	266.7, 	MULTIPLIER_LATERAL*point[20], 		MULTIPLIER_VERTICAL*point[21]);
		fprintf(output,"%f %f %f\n\n", 	266.7, 	MULTIPLIER_LATERAL*point[20]+32, 	MULTIPLIER_VERTICAL*point[21]);
		fprintf(output,"%f %f %f\n", 	293.3, 	MULTIPLIER_LATERAL*point[22], 		MULTIPLIER_VERTICAL*point[23]);
		fprintf(output,"%f %f %f\n\n", 	293.3, 	MULTIPLIER_LATERAL*point[22]+32, 	MULTIPLIER_VERTICAL*point[23]);
		fprintf(output,"%f %f %f\n", 	320.0, 	MULTIPLIER_LATERAL*point[24], 		MULTIPLIER_VERTICAL*point[25]);
		fprintf(output,"%f %f %f\n\n", 	320.0, 	MULTIPLIER_LATERAL*point[24]+32, 	MULTIPLIER_VERTICAL*point[25]);
		fprintf(output,"%f %f %f\n", 	346.7, 	MULTIPLIER_LATERAL*point[26], 		MULTIPLIER_VERTICAL*point[27]);
		fprintf(output,"%f %f %f\n\n", 	346.7, 	MULTIPLIER_LATERAL*point[26]+32, 	MULTIPLIER_VERTICAL*point[27]);
		fprintf(output,"%f %f %f\n", 	373.3, 	MULTIPLIER_LATERAL*point[28], 		MULTIPLIER_VERTICAL*point[29]);
		fprintf(output,"%f %f %f\n\n", 	373.3, 	MULTIPLIER_LATERAL*point[28]+32, 	MULTIPLIER_VERTICAL*point[29]);
		fprintf(output,"%f %f %f\n", 	400.0, 	MULTIPLIER_LATERAL*point[30], 		MULTIPLIER_VERTICAL*point[31]);
		fprintf(output,"%f %f %f\n\n", 	400.0, 	MULTIPLIER_LATERAL*point[30]+32, 	MULTIPLIER_VERTICAL*point[31]);
		fprintf(output,"\n");
		fclose(output);
		}
}

int main(int argc, char **argv){
	input= fopen(argv[1], "r");
	framecount= atoi(argv[2]);
	parse18dualside();
	//parse16singleside();
	}

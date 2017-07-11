#include <stdio.h>
#include <stdlib.h>
#define COUNT 9

FILE *input;
FILE *output;
double data[COUNT];
int hour, min, sec, milisec;

int main(int argc, char **argv){
	input= fopen(argv[1], "r");
	if (!input) {
			printf("error opening input file\n");
			return(input);
		}
	output= fopen(argv[2], "w");
	if (!output) {
			printf("error opening output file\n");
			return(output);
		}
	
	int ln;
	printf("parsing %d lines\n", atoi(argv[3]));
	while(ln<atoi(argv[3])){ // read until EOF
		// 11:56:58.188,0.00,-0.04,-0.04,-0.04,-0.04,-0.03,-0.04,-0.04,-0.05
		
		fscanf(input,"%d:%d:%d.%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",\
			&hour, &min, &sec, &milisec,\
			&(data[0]),&(data[1]),&(data[2]),&(data[3]),&(data[4]),\
			&(data[5]),&(data[6]),&(data[7]),&(data[8]));
		int i;
		for (i=0; i<COUNT; i++){
			fprintf(output, "%lf ", data[i]);
			}
		ln++;
		}
	return(ln);
	}


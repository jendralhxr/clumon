#include <stdio.h>
#include <stdlib.h>
#define SELECTROW 0
FILE *text;

int parse9pointsyonly(){
	double val[3][9];
	int i, j, n;
	for (n=0; n<atoi(argv[2]); n++){
		// 0	-0.04	-0.04	-0.04	-0.04	-0.03	-0.04	-0.04	-0.04
		fscanf(text,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",\
			&(val[i][0]), &(val[i][1]), &(val[i][2]), &(val[i][3]), &(val[i][4]),\
			&(val[i][5]), &(val[i][6]), &(val[i][7]), &(val[i][8])); 
		i++;
	if(i==3){
		i=0;
		// moving average; for smoothing
		for (j=0; j<9; j++) val[SELECTROW][j] = (val[0][j] + val[1][j] + val[2][j]) /3;
		// select row (1 out of 3)
		j= SELECTROW;
		printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",\
			(val[j][0]), (val[j][1]), (val[j][2]), (val[j][3]), (val[j][4]),\
			(val[j][5]), (val[j][6]), (val[j][7]), (val[j][8]));
		}
	}
}


int main(int argc, char **argv){
	text= fopen(argv[1], "r");
	//parse9pointsyonly();
	return(atoi(argv[2]));
	}

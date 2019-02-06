#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMTOKEN 1024	

FILE *inFile;
FILE *outFile;

char buffer[2048];
double values[512];

char *str1, *str2, *token, **tokens;
char *saveptr1, *saveptr2;
int i, j, tokennum, tokenzero;

int main(int argc, char **argv){
	int line, linenumber=strtol(argv[3],NULL,10);
	printf("%s: parsed %d lines\n", argv[1], linenumber);
	
	inFile=fopen(argv[1],"r");
	if (inFile==NULL){
		printf("Input file could not be opened!\n");   
		return(1);
		}
	outFile=fopen(argv[2],"w");

	tokens = malloc(sizeof(char*)*NUMTOKEN);
	for (i=0; i<NUMTOKEN; i++) tokens[i] = malloc(sizeof(char)*16);
	
	for (line=0; line<linenumber; line++){
		// read a line
		fgets(buffer, 2048, inFile);
		buffer[strlen(buffer)-2]=NULL;
		//printf("%s",buffer);
	
		// parse a line
		// find first zero
		for (j = 0, str1 = buffer; ; j++, str1 = NULL){
			token = strtok_r(str1, ";\t", &saveptr1);
	        if (token == NULL) break;
	        strcpy(tokens[j],token);
			//printf("%d %s\tc%d\n",j, tokens[j], strcmp(tokens[j],"0.000000"));
			if ((strcmp(tokens[j],"0.000000")==0) && (j!=0) && (tokenzero==0)) tokenzero=j;
			}
		tokennum=j;
//		printf("first zero %d\n",tokenzero);
		
		fprintf(outFile,"%s;\t",tokens[0]);
		// end sweep
		for (j= tokenzero; j<tokennum; j++){
			fprintf(outFile,"%s\t",tokens[j]);
			}
		// front sweep	
		for (j= 1; j<tokenzero; j++){
			fprintf(outFile,"%s\t",tokens[j]);
			}
		fprintf(outFile,"\n");
		}
	
	fclose(inFile);
	fclose(outFile);
		
	}

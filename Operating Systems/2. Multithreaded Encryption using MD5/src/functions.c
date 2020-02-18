#include "functions.h"
#include <string.h>

// Opens our file and reads line till a random number is reached
void readlinefromfile(char *line){
//	char line[256];
	int len, n, a = rand()%100+1;
	FILE *fp;

	fp = fopen("input/text.txt", "r");
	if (fp == NULL) exit(EXIT_FAILURE);

	for( n = 0;  n < a; n++ )
		if(fgets(line, 256, fp ) == NULL)
			fp = fopen("text.txt", "r");
	len = strlen(line);

	if( line[len-2] == '\r'){
		line[len-3] = 0;
		line[len-2] = 0;
		line[len-1] = 0;
	}else if( line[len-1] == '\n'){
		line[len-1] = 0;
	}
	fclose(fp);
}

// Error printer for cmdl arguments
void printCmdlErr(int argc){
	if(argc > 2)
		printf("Please make sure you only pass the number of components as a command line argument and nothing else.\n");
	else
		printf("Please make sure you pass the number of components as a command line argument and nothing else.\n");
	exit(1);
}



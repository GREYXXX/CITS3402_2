#include "variables.h"

void readfile(char *arg)
{       
        int got1,got2;
        FILE *fp = fopen(arg,"r");
        if(fp == NULL) {
                perror("Unable to open file!");
                exit(1);
        }
       
        int *chunk = malloc(sizeof(int));
	got1 = fread(chunk, sizeof(int), 1, fp);
	p.vexnum = *chunk;

	int count = ((*chunk)*(*chunk));
	//printf("count is %d\n",count);
	int *buffer = malloc(sizeof(int)*(count));
	got2 = fread(buffer, sizeof(int), count, fp);
	//printf("chunk is %d\n",*chunk);
	p.matrix = (int*)malloc(p.vexnum * p.vexnum * sizeof(int*));
/*
	for(int k = 0;k < p.vexnum;k++){
		p.matrix[k] = (int*)malloc(p.vexnum * sizeof(int));
	}
*/
	int n = 0;
	for(int i = 0;i < p.vexnum * p.vexnum;i++){
		p.matrix[i] = buffer[n];
		n++;
		}
	}
/*			
	for (int i = 0; i < count; ++i) {
                printf("buffer[%d] = %d\n", i, buffer[i]);
    }
*/
        //printf("size is %d\n", buffer[count]);
	fclose(fp);
}

#include "variables.h"

graph readfile(char *arg)
{       
        int got1,got2;
	Graph g;
        FILE *fp = fopen(arg,"r");
        if(fp == NULL) {
                perror("Unable to open file!");
                exit(1);
        }
       
        int *chunk = malloc(sizeof(int));
        int k = 0;
        int l;
	got1 = fread(chunk, sizeof(int), 1, fp);
	g.vexnum = *chunk;

	int count = ((*chunk)*(*chunk));
	printf("count is %d\n",count);
	int *buffer = malloc(sizeof(int)*(count));
	got2 = fread(buffer, sizeof(int), count, fp);
	printf("chunk is %d\n",*chunk);
	g.matrix = int(**)malloc(g.vexnum * sizeof(int*));
	for(int k = 0;k < g.vexnum;k++){
		g.matrix[k] = (int*)malloc(g.vexnum * sizeof(int));
	}
	int n = 0;
	for(int i = 0;i < g.vexnum;i++){
		for(int j = 0;j < g.vexnum;j++){
			g.matrix[i][j] = buffer[n];
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
	free(chunk);
	free(buffer);
	return g;
	for(int k = 0;k < g.vexnum;k++){
                free(g.matrix[k]);
        }
	free(g.matrix);
}


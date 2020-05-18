#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
        int got1,got2;
        FILE *fp = fopen(argv[1],"r");
        if(fp == NULL) {
                perror("Unable to open file!");
                exit(1);
        }

        int *chunk = malloc(sizeof(int));
        got1 = fread(chunk, sizeof(int), 1, fp);
        int vexnum = *chunk;

        int count = ((*chunk)*(*chunk));
        //printf("count is %d\n",count);
        int *buffer = malloc(sizeof(int)*(count));
        got2 = fread(buffer, sizeof(int), count, fp);
        //printf("chunk is %d\n",*chunk);
        int **matrix = (int**)malloc(vexnum * sizeof(int*));
        for(int k = 0;k < vexnum;k++){
                matrix[k] = (int*)malloc(vexnum * sizeof(int));
        }
        int n = 0;
        for(int i = 0;i < vexnum;i++){
                for(int j = 0;j < vexnum;j++){
                        matrix[i][j] = buffer[n];
                        n++;
              }
        }
        for(int i = 0;i < vexnum;i++){
                for(int j = 0;j < vexnum;j++){
                        printf("%d\n",matrix[i][j]);

                }
        }
        fclose(fp);
        return 0;
}


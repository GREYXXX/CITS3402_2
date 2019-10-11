#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{       
        int got1,got2;
        FILE *fp = fopen(argv[1],"r");
        if(fp == NULL) {
                perror("Unable to open file!");
                exit(1);
        }
        //fseek(fp,0,SEEK_END);
        //int size = ftell(fp);
        int *chunk = malloc(sizeof(int));
        int k = 0;
        int l;
	got1 = fread(chunk, sizeof(int), 1, fp);

	int count = ((*chunk)*(*chunk));
	printf("count is %d\n",count);
	//printf("file is %d\n",size);
	int *buffer = malloc(sizeof(int)*(count));
	got2 = fread(buffer, sizeof(int), count, fp);
	//l = k;
	printf("chunk is %d\n",*chunk);
	for (int i = 0; i < count; ++i) {
                printf("buffer[%d] = %d\n", i, buffer[i]);
    }

        //printf("size is %d\n", buffer[count]);
	fclose(fp);
	return 0;
	free(chunk);
	free(buffer);
}


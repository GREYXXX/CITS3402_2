#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	int vexnum;
	int **matrix;
}Graph;

extern Graph readfile(char *);
extern int floyd(Graph);

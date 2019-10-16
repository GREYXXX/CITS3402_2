#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	int vexnum;	//Number of vertices
	int **matrix;
}Graph;

Graph p;

extern void readfile(char *);
extern int Floyd(Graph);

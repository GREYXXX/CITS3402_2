#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	int vexnum;
	int **matrix;
}Graph

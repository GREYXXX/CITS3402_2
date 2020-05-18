#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>
/*
typedef struct{
	int vexnum;	//Number of vertices
	int *matrix;
}Graph;

Graph p;
*/

//extern void readfile(char *);
extern int Floyd(int matrix[],int n, int rank, int p, MPI_Comm comm);
extern void find_k(int k, int p, int n);
extern void copy_row(int local_mat[], int n, int p, int k_row, int k)
extern void write_mat(int localmat[], int n, int rank, int p, MPI_Comm comm);


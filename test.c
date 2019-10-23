#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char** argv) {

	if(argc != 2) {
		printf("Usage is mpirun -n [number of processes] %s [filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int size,
	rank;
	
	int *vertexNum;
	clock_t start, end;

	MPI_File fh;
	MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	int rc = MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	if(rc) {
		if(rank == 0) {
			fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		}
		MPI_Finalize();
		exit(2);
	}

	vertexNum = malloc(sizeof(int));
	if(rank == 0) {
		MPI_File_read_at(fh, 0, vertexNum, 1, MPI_INT, MPI_STATUS_IGNORE);
		printf("Number of vertices is %d\n", *vertexNum);
	}
	MPI_Bcast(vertexNum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	int processRow = (*vertexNum) / size; // Number of rows that each process deals with
	int count = processRow * (*vertexNum); //Number of elements each process deals with
	int offset = (count * sizeof(int) * rank) + 4; //File position each process starts at

	int (*buffer)[*vertexNum] = malloc(sizeof *buffer * processRow);

	printf("Process %d ready with offset: %d and will process %d rows, No. vertices: %d\n", rank, offset, processRow, *vertexNum);

	//Each process reads the portion of the file that it is allocated
	MPI_File_read_at_all(fh, offset, &(buffer[0][0]), count, MPI_INT, MPI_STATUS_IGNORE);

	//printf("Process:%d First Number: %d\n", rank, buffer[0][0]);

	//Declare and initialise a contiguous array of pointers
	int (*matrix)[*vertexNum];
	matrix = malloc(sizeof *matrix * *vertexNum);

	MPI_Gather(&(buffer[0][0]), count, MPI_INT, &(matrix[0][0]), count, MPI_INT, 0, MPI_COMM_WORLD);

/*	if(rank == 0) {	
	for(int i = 0; i < *vertexNum; i++) { //ROW
		for(int j = 0; j < *vertexNum; j++) { //COLUMN
			printf("Matrix[%d][%d] is %d\n", i, j, matrix[i][j]);
		}
	}
	}*/


	for(int k = 0; k < *vertexNum; k++) { 	//k is both the intermediate vertex and the process id
		MPI_Bcast(&(matrix[0][0]), (*vertexNum)*(*vertexNum), MPI_INT, 0, MPI_COMM_WORLD);	
		int start = processRow* rank;
		int end = processRow * (rank + 1);
		//printf("K: %d Processor %s rank %d/%d will deal with row %d - %d\n",k,  processor_name, rank, size, start, end);

		for(int i = start; i < end; i++) { //ROW
			for(int j = 0; j < *vertexNum; j++) { //COLUMN
				//printf("Matrix[i][j] is Matrix[%d][%d] is %d\n", i, j, matrix[i][j]);
				//printf("Matrix[i][k] is Matrix[%d][%d] is %d\n", i, k, matrix[i][k]);
				//printf("Matrix[k][j] is Matrix[%d][%d] is %d\n", k, j, matrix[k][j]);
				if(matrix[i][j] > matrix[i][k] + matrix[k][j]) {
					matrix[i][j] = matrix[i][k] + matrix[k][j];
			
				}
			}
		}

		MPI_Gather(&(matrix[start][0]), count, MPI_INT, &(matrix[0][0]), count, MPI_INT, 0, MPI_COMM_WORLD);
	}

	if(rank == 0) {
		FILE *fp = fopen("output.out", "w");
		if(fp == NULL) {
			printf("Error!\n");
			exit(1);
		}

		fprintf(fp, "%d ", *vertexNum);
		for(int i = 0; i < *vertexNum; i++) {
			for(int j = 0; j < *vertexNum; j++) {
				fprintf(fp, "%d ", matrix[i][j]);
			}
		}
	}

	MPI_File_close(&fh);
        MPI_Finalize();

}

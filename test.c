#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char** argv) {

	int size,
	rank;
	
	int *vertexNum;
	clock_t start, end;

	MPI_File fh;
	//int buf[1000];
	MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	int rc = MPI_File_open(MPI_COMM_WORLD, "4.in", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	if(rc) {
		if(rank == 0) {
			fprintf(stderr, "%s: Could not open file %s\n", argv[0], "16.in");
		}
		MPI_Finalize();
		exit(2);
	}

	vertexNum = malloc(sizeof(int));
	if(rank == 0) {
		MPI_File_read_at(fh, 0, vertexNum, 1, MPI_INT, MPI_STATUS_IGNORE);
		//int *buffer = malloc(sizeof(int) * 5);	
		//MPI_File_read_at(*fh, 4, buffer, 5, MPI_INT, MPI_STATUS_IGNORE);
		//printf("Integers are: %d, %d, %d, %d, %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
		printf("Number of vertices is %d\n", *vertexNum);
	}
	MPI_Bcast(vertexNum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	int processRow = (*vertexNum) / size; // Number of rows that each process deals with
	int count = processRow * (*vertexNum); //Number of elements each process deals with
	int offset = (count * rank) + 4; //File position each process starts at

	int (*buffer)[*vertexNum] = malloc(sizeof *buffer * processRow);

	printf("Process %d ready with offset: %d and will process %d rows, No. vertices: %d\n", rank, offset, processRow, *vertexNum);

	//Each process reads the portion of the file that it is allocated
	MPI_File_read_at_all(fh, offset, &(buffer[0][0]), count, MPI_INT, MPI_STATUS_IGNORE);

	//printf("Process:%d First Number: %d\n", rank, buffer[0][0]);

	//Declare and initialise a contiguous array of pointers
	int (*matrix)[*vertexNum];
	matrix = malloc(sizeof *matrix * *vertexNum);

	MPI_Gather(&(buffer[0][0]), count, MPI_INT, &(matrix[0][0]), count, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		/*for(int i = 4; i < 5; i++) {
			for(int j = 0; j < 5; j++) {
				printf("matrix[%d][%d]: %d\n", i,j, matrix[i][j]);
			}
		}*/
	}

	for(int k = 0; k < *vertexNum; k++) { 	//k is both the intermediate vertex and the process id
		MPI_Barrier(MPI_COMM_WORLD);

		//int (*sub_matrix)[*vertexNum] = malloc(sizeof *sub_matrix * processRow);	
		MPI_Bcast(&(matrix[0][0]), (*vertexNum)*(*vertexNum), MPI_INT, 0, MPI_COMM_WORLD);	
		int start = processRow* rank;
		int end = processRow * (rank + 1);
		//printf("Processor %s rank %d/%d will deal with row %d - %d\n", processor_name, rank, size, start, end);
			
		for(int i = start; i < end; i++) { //ROW
			for(int j = 0; j < *vertexNum; j++) { //COLUMN
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

/*
	if(rank == 0) {
		printf("%d\n", p.matrix[0]);
	}

	if(rank == 0) {		
		start = clock();
		fp = fopen("output.out", "w");
		if(fp == NULL) {
			printf("Error!");
			exit(1);
		}
		fprintf(fp, "%d ", n);	//Writes number of vertices to the output file 
	}
*/
/*
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);

	printf("processor %s, rank %d/%d processors\n", processor_name, rank, size);
*/
	MPI_File_close(&fh);
        MPI_Finalize();

}

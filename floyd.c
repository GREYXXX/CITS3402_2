/*
NAME:	Syukri Zainal Abidin	Student ID:	21972786
	Xi Rao					22435044

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char** argv) {

	//Checks arguments
	if(argc != 2) {
		printf("Usage is mpirun -n [number of processes] %s [filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Declare COMM rank & size
	int size,
	rank;

	//Number of vertices	
	int *vertexNum;
	
	//Timer definitions
	clock_t start_time, end_time;

	//MPI declaractions
	MPI_File fh;				//MPI File handle
        MPI_Init(&argc, &argv);			
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//MPI_File_open reads the provided file
	int rc = MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	if(rc) {
		if(rank == 0) {
			fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		}
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

	vertexNum = malloc(sizeof(int));
	//Root process reads the first element of the file which holds the number of vertices
	if(rank == 0) {
		MPI_File_read_at(fh, 0, vertexNum, 1, MPI_INT, MPI_STATUS_IGNORE);
	}

	//vertexNum is broadcasted to all processes
	MPI_Bcast(vertexNum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int processRow = (*vertexNum) / size; 		//Number of rows that each process deals with
	int count = processRow * (*vertexNum); 		//Number of elements each process deals with
	int offset = (count * sizeof(int) * rank) + 4; 	//File position each process starts at

	//Buffer stores all the elements read by each process
	int (*buffer)[*vertexNum] = malloc(sizeof *buffer * processRow);

	//Each process reads the portion of the file that it is allocated
	MPI_File_read_at_all(fh, offset, &(buffer[0][0]), count, MPI_INT, MPI_STATUS_IGNORE);

	//Declare and initialise a contiguous array of pointers
	int (*matrix)[*vertexNum];
	matrix = malloc(sizeof *matrix * *vertexNum);

	//MPI_Gather gathers all the buffers together into a single matrix
	MPI_Gather(&(buffer[0][0]), count, MPI_INT, &(matrix[0][0]), count, MPI_INT, 0, MPI_COMM_WORLD);

	//Timer to record length of time for Floyd-Warshall algorithm
	if(rank == 0) {
		start_time = clock();
	}

	for(int k = 0; k < *vertexNum; k++) { 	//k is both the intermediate vertex and the process id
		//The matrix is broadcasted to all processes
		MPI_Bcast(&(matrix[0][0]), (*vertexNum)*(*vertexNum), MPI_INT, 0, MPI_COMM_WORLD);	
		int start = processRow* rank;
		int end = processRow * (rank + 1);

		for(int i = start; i < end; i++) { //ROW
			for(int j = 0; j < *vertexNum; j++) { //COLUMN
				if(matrix[i][j] > matrix[i][k] + matrix[k][j]) {
					matrix[i][j] = matrix[i][k] + matrix[k][j];
			
				}
			}
		}
		//Each process sends back the portion of the matrix that it worked on to the root process
		MPI_Gather(&(matrix[start][0]), count, MPI_INT, &(matrix[0][0]), count, MPI_INT, 0, MPI_COMM_WORLD);
	}

	//Root process stops the timer and prints the matrix onto the output file
	if(rank == 0) {
		end_time = clock();
		double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		printf("Time taken: %f\n", time_taken);
	
		FILE *fp = fopen("file.out", "w");
		if(fp == NULL) {
			fprintf(stderr, "Could not open %s\n", "file.out");
			exit(EXIT_FAILURE);
		}
	
		fwrite(vertexNum, sizeof(int), 1, fp);	
		//fprintf(fp, "%d ", *vertexNum);

		for(int i = 0; i < *vertexNum; i++) {

			fwrite(matrix[i], sizeof(int), *vertexNum, fp);
		/*
			for(int j = 0; j < *vertexNum; j++) {
				fprintf(fp, "%d ", matrix[i][j]);
			
			}*/
		}
		fclose(fp);
	}

	free(buffer);
	free(matrix);
	
	MPI_File_close(&fh);
        MPI_Finalize();

}

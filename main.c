#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>


int find_k(int k, int p, int n){
	return k/(n/p);
}

void copy_row(int local_mat[], int n, int p, int k_row[], int k){
        int job_k = k % (n/p);
	int j;
        for(j = 0; j < n; j++){
                k_row[j] = local_mat[job_k*n + j];
        }
}

int floyd(int matrix[],int n, int rank, int p, MPI_Comm comm){
	int *D = (int*)malloc(n * n * sizeof(int));
	int *local_mat = (int*)malloc(n*n/p*sizeof(int));
    	int i,j,k,a,temp;
	//Initializing D;
    	for(i=0;i<n;i++){
            	D[i] = matrix[i];
        }
	//If root process, scatter matrix D;
	if(rank == 0){
		MPI_Scatter(D, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm)
	}
	//Starting floyd;
    	for(k=0;k<n;k++){
		//Finding the process number of k-th row;
		int root = find_k(k,p,n);
		int *k_row = malloc(sizeof(int)*n);
		if(rank == root)
			//Copy the k-th row numbers to the buffer k_row;
			copy_row(local_mat, n, p, k_row, k);
		MPI_Bcast(k_row, n, MPI_INT, root, comm);
        	for(a=0;a < n/p;a++){
            		for(int j=0;j<n;j++){
				temp = local_mat[a*n + k] + k_row[j];
                		if(temp < local_mat[a*n + j]){
					//Updating local_mat;
                    			local_mat[a*n + j] = temp;
                		}
            		}
        	}
    	}
	//Writting the shorest path to .out file;
	write_mat(local_mat, n, rank, p,comm);
	free(D);
	free(k_row);
	free(local_mat);
	return 0;
}


void write_mat(int local_mat[], int n, int rank, int p, MPI_Comm comm)
{
	FILE *fp = fopen("output.out", "w");
	if(fp == NULL) {
		printf("Error!");
		exit(1);
	}
	fprintf(fp, "%d", n);
	int *temp = NULL;
	if(rank == 0){
		temp = malloc(n*n*sizeof(int));
		MPI_Gather(local_mat, n*n/p, MPI_INT, temp, n*n/p, MPI_INT, 0, comm);
		for(int i = 0;i < n;i++){
			for(int j = 0;j < n;j++){
				fprintf(fp, "%d", temp[i*n + j]);
			}
		}
	}
	free(temp);
}		

int main(int argc, char *argv[]){
	if(argc != 2) {
		printf("Usage is mpirun -n [number of processes] %s [filename]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int size,
	rank,
	max_process_id;
	MPI_File fh;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm Comm;
	Comm = MPI_COMM_WORLD;
	MPI_Comm_rank(Comm, &rank); //ID of this process in that communicator
	MPI_Comm_size(Comm, &size); //Number of processes in the communicator
		
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	MPI_Get_processor_name(processor_name, &name_len);
	int rc = MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	if(rc) {
		if(rank == 0) {
			fprintf(stderr, "%s: Could not open file %s\n", argv[0], argv[1]);
		}
		MPI_Finalize();
		exit(2);
	}

	int *vertexNum = malloc(sizeof(int));
	if(rank == 0) {
		MPI_File_read_at(fh, 0, vertexNum, 1, MPI_INT, MPI_STATUS_IGNORE);
		printf("Number of vertices is %d\n", *vertexNum);
	}
	MPI_Bcast(vertexNum, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int processRow = (*vertexNum) / size; // Number of rows that each process deals with
	int count = processRow * (*vertexNum); //Number of elements each process deals with
	int offset = (count * sizeof(int) * rank) + 4; //File position each process starts at

	int *buffer = malloc(sizeof(int) * processRow * (*vertexNum));

	printf("Process %d ready with offset: %d and will process %d rows, No. vertices: %d\n", rank, offset, processRow, *vertexNum);

	//Each process reads the portion of the file that it is allocated
	MPI_File_read_at_all(fh, offset, &buffer, count, MPI_INT, MPI_STATUS_IGNORE);

	//Declare and initialise a contiguous array of pointers
	int *matrix;
	matrix = malloc(sizeof(int) *(*vertexNum) * (*vertexNum));

	MPI_Gather(&buffer, count, MPI_INT, &matrix, count, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0){
		clock_t start = clock();
	}
	int n = *vertexNum;
	MPI_Bcast(&n, 1, MPI_INT, 0, Comm);	
	//readfile(argv[1]);
	floyd(matrix, n, rank, size, Comm);
	
	if(rank == 0){
		clock_t end = clock();
		double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
		printf("Time taken: %f\n", time_taken);
	}

	MPI_Finalize();
	return 0;
}


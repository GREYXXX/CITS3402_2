#include "variables.h"

void computePath(int k, int n, int start, int end)
{
	if(start != 0) {
		start = start / n; 	//The row to start on
	}	
	end = end / n;			//The row to end on
	for(int i = start; i < end; i++) {
		for(int j = 0; j < n; j++) {
			if(p.matrix[i][j] > p.matrix[i][k] + p.matrix[k][j]) {
				p.matrix[i][j] = p.matrix[i][k] + p.matrix[k][j];
			
			}
		}
	}
}

int main(int argc, char *argv[]){

	int size,
	rank,
	max_process_id;

	int n = p.vexnum;
	FILE *fp;
	clock_t start, end;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //ID of this process in that communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size); //Number of processes in the communicator

	if(rank == 0) {		
		readfile(argv[1]);
	

		start = clock();

		fp = fopen("output.out", "w");
		if(fp == NULL) {
			printf("Error!");
			exit(1);
		}
		fprintf(fp, "%d ", n);	//Writes number of vertices to the output file 
	}


	int processPartition = (n * n) / size ; //Length of each partition that will be assigned to a process
	
	for(int k = 1; k <= max_process_id; k++) { 	//k is both the intermediate vertex and the process id
		MPI_Barrier(MPI_COMM_WORLD);
		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);
	 
		printf("processor %s, rank %d/%d processors.\n", processor_name, rank, size);
		max_process_id = size - 1;

		printf("Sending %d elements to task %d\n", processPartition, k);
		//Create a buffer that will hold a subset of array D1
		//int *D_buffer = malloc(sizeof(int) * processPartition);
		int start = processPartition * rank;
		int end = processPartition * (rank + 1);
		computePath(k, n, start, end);	
		
	}

	if(rank == 0) {	
	//All elements in p.matrix are now the shortest paths
		 for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				//printf("shortest path is %d\n",p.matrix[i][j]);
				fprintf(fp, "%d ", p.matrix[i][j]);
			}
		}
		fclose(fp);
		end = clock();
		double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
		printf("Time taken: %f\n", time_taken);
	}
	/*for(int i = 0;i < n;i++){
		free(D1[i]);
		free(path[i]);
	}
	free(D1);
	free(path);*/
	
	MPI_Finalize();

	//Floyd(argc, argv);
		//MPI_Finalize();

	return 0;
}


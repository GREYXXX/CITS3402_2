#include "variables.h"

//Computes the shortest path for a given parameters
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


int Floyd(){
	FILE *fp = fopen("output.out", "w");
	if(fp == NULL) {
		printf("Error!");
		exit(1);
	}

	int n = p.vexnum;	//Number of vertices
	fprintf(fp, "%d ", n);	//Writes number of vertices to the output file 

    	int world_size,	//Total number of ranks or the number of processes across all nodes
	world_rank,	//Rank
	max_process_id;	//Max ID of processors/workers
		
	int processPartition = (n * n) / world_size ; //Length of each partition that will be assigned to a process
	
	for(int k = 1; k <= max_process_id; k++) { 	//k is both the intermediate vertex and the process id
		MPI_Status status;
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);

		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);
	 
		printf("processor %s, rank %d/%d processors.\n", processor_name, world_rank, world_size);
		max_process_id = world_size - 1;

		printf("Sending %d elements to task %d\n", processPartition, k);
		//Create a buffer that will hold a subset of array D1
		//int *D_buffer = malloc(sizeof(int) * processPartition);

		//Scatter the elements across all processes
		//MPI_Scatter(&(D1[0][0]), processPartition, MPI_INT, D_buffer, processPartition, MPI_INT, 0, MPI_COMM_WORLD);
		int start = processPartition * world_rank;
		int end = processPartition * (world_rank + 1);
		computePath(k, n, start, end);	
		
		MPI_Finalize();
	}
		
	//All elements in p.matrix are now the shortest paths
	 for(i=0;i<n;i++){
         	for(j=0;j<n;j++){
			//printf("shortest path is %d\n",p.matrix[i][j]);
			fprintf(fp, "%d ", p.matrix[i][j]);
		}
	}
	for(int i = 0;i < n;i++){
		free(D1[i]);
		free(path[i]);
	}
	free(D1);
	free(path);
	fclose(fp);
	return 0;
}

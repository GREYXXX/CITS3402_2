#include "variables.h"

int Floyd(){
	FILE *fp = fopen("output.out", "w");
	if(fp == NULL) {
		printf("Error!");
		exit(1);
	}

	int n = p.vexnum;	//Number of vertices
	fprintf(fp, "%d ", n);	//Writes number of vertices to the output file 

	int **path = (int**)malloc(n * sizeof(int*));
	int **D = (int**)malloc(n * sizeof(int*));
	
 
    	int world_size,	//Total number of ranks or the number of processes across all nodes
	world_rank,	//Rank
	numworkers;	//Number of processors/workers
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);
 
        printf("processor %s, rank %d/%d processors.\n", processor_name, world_rank, world_size);
	numworkers = world_size - 1;
	if(world_rank == 0) { 	//MASTER rank
		printf("MASTER: INITIALISNG GRAPH\n");
		//GRAPH INITIALISATION
		for (int a = 0;a < n; a++){
		path[a] = (int*)malloc(n*sizeof(int));
		D[a] = (int*)malloc(n*sizeof(int));
	}
		int i,j,k;
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				D[i][j] = g.matrix[i][j];
				path[i][j] = j;
			}
		}
	
	int average_row = n / numworkers;
	//int extra = n % numworkers;
	int chunksize = (n * n) / world_size;
	//int tag1 = 2;
	//int tag2 = 1;
	int offset = chunksize;
	for(int k = 1; k <= numworkers; k++) {
		printf("Sending %d rows to task %d offset = %d\n", average_row, k, offset);
		MPI_Send(&offset, 1, MPI_INT, k, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(&p[offset], chunksize, MPI_INT, k, MPI_ANY_TAG, MPI_COMM_WORLD);
		printf("Sent %d elements to task %d offset = %d\n");
		offset = offset + chunksize;
	}
	//Master does its work
	offset = 0;
		
		
	}

    	for(k=0;k<n;k++){ //Keeps track of the outer loop
        	for(i=0;i<n;i++){ //Keeps track of each row
            		for(j=0;j<n;j++){ //Keeps track of each column
                		if(D[i][j]>D[i][k]+D[k][j]){
                    			D[i][j]=D[i][k]+D[k][j];	//Stores the current shortest path
                    			path[i][j]=path[i][k]; 		//Stores the number of nodes to get to the shortest path
                		}
            		}
        	}
    	}
	//All elements in D are now the shortest paths
	 for(i=0;i<n;i++){
         	for(j=0;j<n;j++){
			//printf("shortest path is %d\n",D[i][j]);
			fprintf(fp, "%d ", D[i][j]);
		}
	}
	for(int i = 0;i < n;i++){
		free(D[i]);
		free(path[i]);
	}
	free(D);
	free(path);
	fclose(fp);
	return 0;
}

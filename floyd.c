#include "variables.h"

int Floyd(Graph g){
	FILE *fp = fopen("output.out", "w");
	if(fp == NULL) {
		printf("Error!");
		exit(1);
	}

	int n = g.vexnum;	//Number of vertices
	fprintf(fp, "%d ", n);	//Writes number of vertices to the output file 

	int **path = (int**)malloc(n * sizeof(int*));
	int **D = (int**)malloc(n * sizeof(int*));
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
    
    	for(k=0;k<n;k++){
        	for(i=0;i<n;i++){
            		for(j=0;j<n;j++){
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

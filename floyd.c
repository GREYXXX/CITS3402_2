#include "variables.h"

int Floyd(Graph g){
	int n = g.vexnum;
	int **path = (int**)malloc(n * sizeof(int*));
	int **D = (int**)malloc(n * sizeof(int*));
	for (int a = 0;a < n; a++){
		path[a] = (int*)malloc(n*sizeof(int));
		D[a] = (int*)malloc(n*sizeof(int));
	}
    	int i,j,k;
    	for(i=0;i<n;i++){//Initializing the  adjacency matrix;
        	for(j=0;j<n;j++){
            		D[i][j] = g.matrix[i][j];
			path[i][j] = j;
        	}
	} 
    
    	for(k=0;k<n;k++){
        	for(i=0;i<n;i++){
            		for(j=0;j<n;j++){
                		if(D[i][j]>D[i][k]+D[k][j]){
                    			D[i][j]=D[i][k]+D[k][j];
                    			path[i][j]=path[i][k];
                		}
            		}
        	}
    	}
	 for(i=0;i<n;i++){
         	for(j=0;j<n;j++){
			printf("shortest path is %d\n",D[i][j]);
		}
	}
	return 0;
}

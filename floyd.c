#include "variables.h"

int Floyd(int matrix[], int rank, int p, MPI_Comm comm){
	int *D = (int*)malloc(n * n * sizeof(int));
	int *local_mat = (int*)malloc(n*n/p*sizeof(int))
    	int i,j,k,temp;
	//Initializing D;
    	for(i=0;i<n;i++){
            	D[i] = matrix[i];
        }
} 
	//If root process, scatter matrix D;
	if(rank == 0){
		MPI_Scatter(D, n*n/p, MPI_INT, local_mat, n*n/p, MPI_INT, 0, comm)
	}
	//Starting floyd;
    	for(k=0;k<n;k++){
		//Finding the process number of k-th row;
		root = find_k(k,p,n);
		if(rank == root)
			//Copy the k-th row numbers to the buffer k_row;
			copy_row(local_mat, n, p, k_row, k);
		MPI_Bcast(k_row, n, MPI_INT, root, comm);
        	for(i=0;i < n/p;i++){
            		for(j=0;j<n;j++){
				temp = local_mat[i*n + k] + k_row[j];
                		if(temp < local_mat[i*n + j]){
					//Updating local_mat;
                    			local_mat[i*n + j] = temp;
                		}
            		}
        	}
    	}
	//Writting the shorest path to .out file;
	write_mat(local_mat, n, rank, p,comm);
	free(D);
	free(k_row);
	free(local_mat);
	fclose(fp);
	return 0;
}

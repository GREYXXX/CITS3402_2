#include "variables.h"

int main(int argc, char *argv[]){

	int rank, size;
	//MPI_init(&argc, &argv);
	//MPI_Comm_rank(MPI_COMM_WORLD, &rank); //ID of this process in that communicator
	//MPI_Comm_size(MPI_COMM_WORLD, &size); //Number of processes in the communicator
	
	readfile(argv[1]);

	clock_t start = clock();
	Floyd(p);
	clock_t end = clock();

	double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Time taken: %f\n", time_taken);

	//MPI_Finalize();

	return 0;
}


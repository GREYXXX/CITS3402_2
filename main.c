#include "variables.h"

int main(int argc, char *argv[]){
	readfile(argv[1]);

	clock_t start = clock();
	Floyd(p);
	clock_t end = clock();

	double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Time taken: %f\n", time_taken);

	return 0;
}


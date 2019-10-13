#include "variables.h"

int main(int argc, char *argv[]){
	Graph p = readfile(argv[1]);
	Floyd(p);
	return 0;
}


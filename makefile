main: main.o readfile.o
	mpicc -o main main.o readfile.o -std=c99 -lm
main.o: main.c variables.h
	mpicc -c main.c -std=c99
readfile.o: readfile.c variables.h
	mpicc -c readfile.c -std=c99
clean:
	rm -f *.o main

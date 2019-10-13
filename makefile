main: main.o readfile.o floyd.o
	gcc -o main main.o readfile.o floyd.o -lm
main.o: main.c variables.h
        gcc -c main.c
readfile.o: readfile.c variables.h
	gcc -c readfile.c
floyd.o: floyd.c variables.h
	gcc -c floyd.c
clean:
	rm -f *.o main

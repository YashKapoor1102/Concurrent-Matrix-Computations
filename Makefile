DET: DET.o

DET.o: DET.c DET.h
	gcc -c DET.c

clean:
	rm *.o DET 

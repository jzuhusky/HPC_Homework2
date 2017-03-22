
CC=gcc48
ompFlag=-fopenmp
jacobiOut=jacobi2D.dat
jacobiExecutable=runJacobi2D.out

EXECS=*.out
DatOutputs=*.dat

jacobi2D-omp: jacobi2D-omp.c 
	$(CC) -o $(jacobiExecutable) $(ompFlag) $^


clean:
	rm -f $(EXECS) $(DatOutputs)

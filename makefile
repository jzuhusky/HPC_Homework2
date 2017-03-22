
CC=gcc48
ompFlag=-fopenmp
FLAGS=-lrt -O3
jacobiExecutable=Jacobi2D.run
gsExecutable=Gs2D.run

EXECS=*.run
DatOutputs=*.dat

METHODS=gs2D-omp jacobi2D-omp

all:${METHODS}

jacobi2D-omp: jacobi2D-omp.c 
	${CC} ${FLAGS} -o ${jacobiExecutable} ${ompFlag} $^
gs2D-omp: gs2D-omp.c 
	${CC} ${FLAGS} -o ${gsExecutable} ${ompFlag} $^


clean:
	rm -f ${EXECS} ${DatOutputs}

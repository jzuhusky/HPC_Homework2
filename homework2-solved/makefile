
CC=gcc48
ompFlag=-fopenmp

EXECS=omp_bug2 omp_bug3 omp_bug4 omp_bug5 omp_bug6

all: ${EXECS}

omp_bug2: omp_solved2.c 
	$(CC) -o omp-solved2.run $(ompFlag) $^

omp_bug3: omp_solved3.c 
	$(CC) -o omp-solved3.run $(ompFlag) $^

omp_bug4: omp_solved4.c 
	$(CC) -o omp-solved4.run $(ompFlag) $^

omp_bug5: omp_solved5.c 
	$(CC) -o omp-solved5.run $(ompFlag) $^

omp_bug6: omp_solved6.c 
	$(CC) -o omp-solved6.run $(ompFlag) $^

clean:
	rm -f *.run

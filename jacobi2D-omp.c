
/*
 *	Joe Zuhusky
 */

#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif


int main(int argc, char **argv){

	if ( argc != 4 ){
		printf("Please provide 4 Args: Mesh Size, Max Iterations, # Threads\n");
		exit(0);
	}
	#ifdef _OPENMP
        #pragma omp parllel private(argc, argv) 
        if ( argc > 3 && omp_get_thread_num() == 0){
                omp_set_num_threads(atoi(argv[argc-1]));
        //        printf("Using %d openMP Threads\n",atoi(argv[argc-1]));
        }
        #endif

	int print_flag = 1;

	const int N = atoi(argv[1])+2;
	long int steps=0;
	int i,j;
	long int max_iter = atoi(argv[2]);

	double **unew, **uold, **temp,h;

	unew = (double**)malloc(sizeof(double*)*N);
	uold = (double**)malloc(sizeof(double*)*N);
	
	for (i=0;i<N;i++){
		*(unew+i) = (double*)malloc(sizeof(double)*N);
		*(uold+i) = (double*)malloc(sizeof(double)*N);
	}

	h = 1.0/(N+1.0); 

	// initialize the grid...
	// and note: boundary conditions are such that u(boundary) == 0.0 
	int f,g;
	for (f=0; f<N; f++){
		for (g=0; g<N; g++){
			uold[f][g]=0.0;
			unew[f][g]=0.0;
		}
	}
	double eps = atof(argv[argc-1]), res,temp2, initRes;
	long int countNotConv = 1;
	/* timing */
  	timestamp_type time1, time2;
  	get_timestamp(&time1);

	// DOING JACOBI HERE!!!!
	while( steps < max_iter  ){
		temp = uold;
		uold = unew;
		unew = temp;
		countNotConv = 0;
		res = 0.0; temp2 = 0.0;
		#pragma omp parallel private(j,temp) shared(eps,unew,uold) reduction(+:countNotConv) reduction(+:res)
		{

		#pragma omp for
		for (i=1; i<N-1; i++){
			for (j=1; j < N-1; j++){
				unew[i][j] = (uold[i-1][j]+uold[i+1][j] + uold[i][j-1] + uold[i][j+1] + 1.0*h*h)/4.0;	
			}
		}
		#pragma omp for private(j)
		for(i=1;i<N-1;i++){
			for(j=1; j<N-1; j++){
				temp2 = (4*unew[i][j]-unew[i-1][j]-unew[i+1][j] - unew[i][j-1] - unew[i][j+1] - 1.0*h*h);
				res += temp2*temp2;
			}
		}
		}
		if ( steps == 0 ){
			initRes = res;
		}else{
			if ( initRes / res > 1000 ){
				break;
			}
		}
		steps++;	
    	} // end while
  	get_timestamp(&time2);
        //printf("%d Iterations & %f residual\n",steps,res/initRes*100.0);
	
	if (print_flag !=0 ){
		FILE *outFile = fopen("jacobi2D.dat", "w" );
		for (i=0;i<N;i++){
			for (j=0;j<N;j++){
				fprintf(outFile,"%f %f %f\n",h*i,h*j,unew[i][j]);
			}
		}
		int closed = fclose(outFile); 
	//	printf("Closed: %d\nSuccessfully wrote Jacobi Result to File: jacobi2D.dat\n",closed);
	}

	double elapsed = timestamp_diff_in_seconds(time1,time2);
	//printf("Time elapsed is % Seconds\n", elapsed);	
	printf("%d %d %f\n",N-2,atoi(argv[argc-1]) , elapsed);	
	for (i=0;i<N;i++){
		free(*(unew+i));
		free(*(uold+i));
	}
	free(unew); 
	free(uold);
	return 0;
}

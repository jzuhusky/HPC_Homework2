
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
                printf("Please provide 3 Args: Mesh Size, Max Iterations, # Threads\n");
                exit(0);
        }

	int print_flag = 1;

	#ifdef _OPENMP
	#pragma omp parllel private(argc, argv) 
	if ( argc > 3 && omp_get_thread_num() == 0){
		omp_set_num_threads(atoi(argv[argc-1]));
		printf("Using %d openMP Threads\n",atoi(argv[argc-1]));
	}
	#endif

	const int N = atoi(argv[1])+2;
	int steps=0;
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
	double temp2, res,initRes;
	/* timing */
  	timestamp_type time1, time2;
  	get_timestamp(&time1);
	// DOING GS here... 
	while( steps < max_iter ){
		temp = uold;
		uold = unew;
		unew = temp;
		res = 0.0;
		#pragma omp parallel shared(unew,uold) private(j) reduction(+:res)
		{
			#pragma omp barrier 
			#pragma omp for 
			for (i=1; i<N-1; i++){
				for (j=i%2+1; j < N-1; j+=2){
					unew[i][j] = (uold[i-1][j]+uold[i+1][j] + uold[i][j-1] + uold[i][j+1] + 1.0*h*h)/4.0;
				}
			}
			#pragma omp barrier 
		 	#pragma omp for
			for (i=1; i<N-1; i++){
				for (j=i%2; j < N-1; j+=2){
					unew[i][j] = (unew[i-1][j]+unew[i+1][j] + unew[i][j-1] + unew[i][j+1] + 1.0*h*h)/4.0;	
				}
			}
			// Now calculate Residuals...
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

	printf("%d Iterations & %f% of initial residual \n",steps,res/initRes*100.0);
	
	
	if (print_flag !=0 ){
		FILE *outFile = fopen("gs2D-omp.dat", "w" );
		for (i=0;i<N;i++){
			for (j=0;j<N;j++){
				fprintf(outFile,"%f %f %f\n",h*i,h*j,unew[i][j]);
			}
		}
		int closed = fclose(outFile); 
	//	printf("Closed: %d\nSuccessfully wrote Gauss-Seidel Result to File: gs2D-omp.dat\n",closed);
	}

	double elapsed = timestamp_diff_in_seconds(time1,time2);
	printf("Time elapsed is %f seconds.\n", elapsed);	
	printf("%d %d %f\n",N-2,atoi(argv[argc-1]) , elapsed);
	for (i=0;i<N;i++){
		free(*(unew+i));
		free(*(uold+i));
	}
	free(unew); 
	free(uold);
	return 0;
}

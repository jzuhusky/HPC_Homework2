/******************************************************************************
* FILE: omp_bug6.c
* DESCRIPTION:
*   This program compiles and runs fine, but produces the wrong result.
*   Compare to omp_orphan.c.
* AUTHOR: Blaise Barney  6/05
* LAST REVISED: 06/30/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define VECLEN 100

float a[VECLEN], b[VECLEN];

/*
float dotprod ()
{
int i,tid;

tid = omp_get_thread_num();
#pragma omp for reduction(+:sum)
  for (i=0; i < VECLEN; i++)
    {
    sum = sum + (a[i]*b[i]);
    printf("  tid= %d i=%d\n",tid,i);
    }
}
*/

int main (int argc, char *argv[]) {
	int i;
	float sum;	
	for (i=0; i < VECLEN; i++){
  		a[i] = b[i] = 1.0 * i;
		printf("a[i]=b[i]=%f\n",1.0*i);
	}
	sum = 0.0;
	// This is just poor programming... The issue here is scoping
	// and what "sum" is being used. Every time the function is 
	// Called, the "Sum" within the function is beign used and modified
	// NOT the "sum" declared in main. To fix, get rid of function call completely... 
	//  dotprod();
	#pragma omp parallel for shared(a,b) reduction(+:sum) 
	for (i=0; i < VECLEN; i++){
    		sum = sum + (a[i]*b[i]);
    		printf("  tid= %d i=%d\n",omp_get_thread_num(),i);
    	}
printf("Sum = %f\n",sum);

}


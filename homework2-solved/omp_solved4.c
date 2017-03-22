/******************************************************************************
* FILE: omp_bug4.c
* DESCRIPTION:
*   This very simple program causes a segmentation fault.
* AUTHOR: Blaise Barney  01/09/04
* LAST REVISED: 04/06/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 1048; // add this..
int main (int argc, char *argv[]) 
{
int nthreads, tid, i, j;
printf("Hello\n");
/* Fork a team of threads with explicit variable scoping */
// This works if you remove 'a' from the private variables

#pragma omp parallel private(i,j,tid) shared(nthreads)
  {

  double a[N][N];

  /* Obtain/print thread info */
  tid = omp_get_thread_num();
  printf("Hello world from thread #%d\n",tid);
  if (tid == 0) 
    {
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    }
  printf("Thread %d starting...\n", tid);

  /* Each thread works on its own private copy of the array */
  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      a[i][j] = tid + i + j;

  /* For confirmation */
  printf("Thread %d done. Last element= %f\n",tid,a[N-1][N-1]);

  }  /* All threads join master thread and disband */

}


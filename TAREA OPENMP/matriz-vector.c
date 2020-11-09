#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h"

/* Serial functions */
void Get_args(int argc, char* argv[], int* thread_count_p, 
      int* m_p, int* n_p);
void Usage(char* prog_name);
void Generate_matrix(double A[], int m, int n);
void Read_matrix(double A[], int m, int n);
void Generate_vector(double x[], int n);
void Read_vector(double x[], int n);

/* Parallel function */
void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count);

int main(int argc, char* argv[]) {
   int     thread_count;
   int     m, n;
   double* A;
   double* x;
   double* y;

   Get_args(argc, argv, &thread_count, &m, &n);

   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
#  ifdef DEBUG
   //Enter the matrix
   Read_matrix(A, m, n);
   //Enter the vector
   Read_vector(x, n);
#  else
   Generate_matrix(A, m, n);
   Generate_vector(x, n);
#  endif

   Omp_mat_vect(A, x, y, m, n, thread_count);

   free(A);
   free(x);
   free(y);

   return 0;
}

void Get_args(int argc, char* argv[], int* thread_count_p, 
      int* m_p, int* n_p)  {

   if (argc != 4) Usage(argv[0]);
   *thread_count_p = strtol(argv[1], NULL, 10);
   *m_p = strtol(argv[2], NULL, 10);
   *n_p = strtol(argv[3], NULL, 10);
   if (*thread_count_p <= 0 || *m_p <= 0 || *n_p <= 0) Usage(argv[0]);

}

void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <m> <n>\n", prog_name);
   exit(0);
}

void Read_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}

void Generate_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = random()/((double) RAND_MAX);
}

void Generate_vector(double x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = random()/((double) RAND_MAX);
}

void Read_vector( double x[], int n) {
   int   i;
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}

void Omp_mat_vect(double A[], double x[], double y[],
      int m, int n, int thread_count) {
   int i, j;
   double start, finish, elapsed, temp;

   GET_TIME(start);
#  pragma omp parallel for num_threads(thread_count)  \
      default(none) private(i, j, temp)  shared(A, x, y, m, n)
   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++) {
         temp = A[i*n+j]*x[j];
         y[i] += temp;
      }
   }
   GET_TIME(finish);
   elapsed = finish - start;
   printf("Time = %e seconds\n", elapsed);
}

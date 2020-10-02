#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
//#include "timer.h"

void Mat_vect_mult(
        double      local_A[]   /* in   */,
        double      local_x[]   /* in   */,
        double      local_y[]   /* out  */,
        int         local_m     /* in   */,
        int         n           /* in   */,
        int         local_n     /* in   */,
        MPI_Comm  comm       /* in  */) {

  double* x;
  int local_i, j;
  int local_ok = 1;

  x = malloc(n*sizeof(double));
  MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n, MPI_DOUBLE, comm);

  for (local_i = 0; local_i < local_m; local_i++) {
    local_y[local_i] = 0.0;
    for (j = 0; j < n; j++) {
      local_y[local_i] += local_A[local_i*n + j]*x[j];
    }
  }
  free(x);
} /* Mat_vect_mult */

void Get_dims(
      int*      m_p        /* out */, 
      int*      local_m_p  /* out */,
      int*      n_p        /* out */,
      int*      local_n_p  /* out */,
      int       my_rank    /* in  */,
      int       comm_sz    /* in  */,
      MPI_Comm  comm       /* in  */) {
  int local_ok = 1;
  if(my_rank == 0){
    printf("Number: ");
    scanf("%d", m_p);
    n_p = m_p;
  }
  MPI_Bcast(m_p, 1, MPI_INT, 0, comm);
  MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
  printf("comm %d\n",comm_sz);
  if(*m_p <= 0 || *n_p <= 0 ) local_ok = 0;
    //Check_for_error(local_ok, "Get_dims", "m and n must be positive and evenly divisible by comm_sz", comm);
  *local_m_p = *m_p/comm_sz;
  *local_n_p = *n_p/comm_sz;
}

void Generate_vector(
      double local_x[] /* out */, 
      int    local_n   /* in  */) {
  int i;
  for (i = 0; i < local_n; i++)
    local_x[i] = ((double) random())/((double) RAND_MAX);
}

void Check_for_error(
      int       local_ok   /* in */, 
      char      fname[]    /* in */,
      char      message[]  /* in */, 
      MPI_Comm  comm       /* in */) {
  int ok;
  MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
  if(ok == 0){
    int my_rank;
    MPI_Comm_rank(comm, &my_rank);
    if(my_rank == 0){
      fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, message);
      fflush(stderr);
    }
    MPI_Finalize();
    exit(-1);
  }
}

void Allocate_arrays(
      double**  local_A_pp  /* out */, 
      double**  local_x_pp  /* out */, 
      double**  local_y_pp  /* out */, 
      int       local_m     /* in  */, 
      int       n           /* in  */,   
      int       local_n     /* in  */, 
      MPI_Comm  comm        /* in  */) {
  int local_ok = 1;
  *local_A_pp = malloc(local_m*n*sizeof(double));
  *local_x_pp = malloc(local_n*sizeof(double));
  *local_y_pp = malloc(local_m*sizeof(double));
  if(*local_A_pp == NULL || local_x_pp == NULL || local_y_pp == NULL) local_ok = 0;
  Check_for_error(local_ok, "Allocate_arrays", "Can't allocate local arrays", comm);
}

void Generate_matrix(
      double local_A[]  /* out */, 
      int    local_m    /* in  */, 
      int    n          /* in  */) {
  int i, j;
  for (i = 0; i < local_m; i++)
    for (j = 0; j < n; j++) 
      local_A[i*n + j] = ((double) random())/((double) RAND_MAX);
}

int main() {
  double* local_A;
  double* local_x;
  double* local_y;
  int m, local_m, n, local_n;
  int my_rank, comm_sz;
  MPI_Comm comm;
  double local_start, local_finish, local_elapsed, elapsed;
  
  MPI_Init(NULL, NULL);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &comm_sz);
  MPI_Comm_rank(comm, &my_rank);

  Get_dims(&m, &local_m, &n, &local_n, my_rank, comm_sz, comm);
  Allocate_arrays(&local_A, &local_x, &local_y, local_m, n, local_n, comm);
  srandom(my_rank);
  Generate_matrix(local_A, local_m, n);
  Generate_vector(local_x, local_n);

  MPI_Barrier(comm);
  local_start = MPI_Wtime();
  //MAT_VECT_MULT
  Mat_vect_mult(local_A, local_x, local_y, local_m, n, local_n, comm);
  local_finish = MPI_Wtime();
  local_elapsed = local_finish-local_start;
  MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
  if(my_rank == 0)
    printf("Time = %e seconds\n", elapsed);
  
  free(local_A);
  free(local_x);
  free(local_y);
  MPI_Finalize();
  
  return 0;
}

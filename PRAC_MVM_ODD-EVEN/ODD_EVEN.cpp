#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <algorithm> 

using namespace std;

void Merge_low(
      int  my_keys[],     /* in/out    */
      int  recv_keys[],   /* in        */
      int  temp_keys[],   /* scratch   */
      int  local_n        /* = n/p, in */){
    int m_i, r_i, t_i;
    m_i = r_i = t_i = 0;
    while(t_i < local_n){
        if(my_keys[m_i] <= recv_keys[r_i]){
            temp_keys[t_i] = my_keys[m_i];
            t_i++; m_i++;
        }else{
            temp_keys[t_i] = recv_keys[r_i];
            t_i++; r_i++;
        }
    }
    //memcpy(my_keys, temp_keys, local_n*sizeof(int));
}
void Merge_high(int local_A[], int temp_B[], int temp_C[], int local_n){
    int ai, bi, ci;
    ai = local_n-1;
    bi = local_n-1;
    ci = local_n-1;
    while(ci >= 0){
        if(local_A[ai] >= temp_B[bi]){
            temp_C[ci] = local_A[ai];
            ci--; ai--;
        }else{
            temp_C[ci] = temp_B[bi];
            ci--; bi--;
        }
    }
    //memcpy(local_A, temp_C, local_n*sizeof(int));
}
void Odd_even_iter(int local_A[], int temp_B[], int temp_C[], int local_n, int phase, int even_partner, int odd_partner, int my_rank, int p, MPI_Comm comm){
    MPI_Status status;
    if(phase % 2 == 0){
        if(even_partner >= 0){
            MPI_Sendrecv(local_A, local_n, MPI_INT, even_partner, 0, temp_B, local_n, MPI_INT, even_partner, 0, comm, &status);
            if(my_rank % 2 != 0)
                Merge_high(local_A, temp_B, temp_C, local_n);
        else
            Merge_low(local_A, temp_B, temp_C, local_n);
        }
    }else{ /* odd phase */
        if(odd_partner >= 0) {
            MPI_Sendrecv(local_A, local_n, MPI_INT, odd_partner, 0, temp_B, local_n, MPI_INT, odd_partner, 0, comm, &status);
            if (my_rank % 2 != 0)
                Merge_low(local_A, temp_B, temp_C, local_n);
            else
                Merge_high(local_A, temp_B, temp_C, local_n);
        }
    }
}

int main(){

	int nump,rank;
	int n,localn;
	int *data,recdata[100],recdata2[100];
	int *temp;
	int ierr,i;
	int root_process;
	int sendcounts;
	MPI_Status status;

    double local_start, local_finish, local_elapsed, elapsed;

    local_start = MPI_Wtime();

	ierr = MPI_Init(NULL, NULL);
    root_process = 0;
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &nump);
    
    //Datos aleatorios
    if(rank == root_process) {
        printf("please enter the number of numbers to sort: ");
        fflush(stdout);
        scanf("%i", &n);
        int avgn = n / nump;
        localn=avgn;

        data=(int*)malloc(sizeof(int)*n);
        for(i = 0; i < n; i++) {
          data[i] = rand()%100;
        }
        printf("\n");
    }
    else{
    	data=NULL;
    }

    ierr=MPI_Bcast(&localn,1,MPI_INT,0,MPI_COMM_WORLD);
    ierr=MPI_Scatter(data, localn, MPI_INT, &recdata, 100, MPI_INT, 0, MPI_COMM_WORLD);
    sort(recdata,recdata+localn);

    //ODD - EVEN
    int oddrank,evenrank;

    if(rank%2==0){
    	oddrank = rank-1; 
    	evenrank = rank+1;
	}
 	else {
 		oddrank = rank+1;
 		evenrank = rank-1;
	}

    /* Set the ranks of the processors at the end of the linear */
    if (oddrank == -1 || oddrank == nump)
        oddrank = MPI_PROC_NULL;
    if (evenrank == -1 || evenrank == nump)
        evenrank = MPI_PROC_NULL;
        

    int p;
    for (p=0; p<nump-1; p++) {
        if (p%2 == 1) /* Odd phase */
        MPI_Sendrecv(recdata, localn, MPI_INT, oddrank, 1, recdata2,
        localn, MPI_INT, oddrank, 1, MPI_COMM_WORLD, &status);
        else /* Even phase */
        MPI_Sendrecv(recdata, localn, MPI_INT, evenrank, 1, recdata2,
        localn, MPI_INT, evenrank, 1, MPI_COMM_WORLD, &status);

        //extract localn after sorting the two
        temp=(int*)malloc(localn*sizeof(int));
        for(i=0;i<localn;i++){
         	temp[i]=recdata[i];
        }
        
        if(status.MPI_SOURCE==MPI_PROC_NULL)	
            continue;
        else if(rank<status.MPI_SOURCE){
         	//store the smaller of the two
         	int i,j,k;
         	for(i=j=k=0;k<localn;k++){
         		if(j==localn||(i<localn && temp[i]<recdata2[j]))
         			recdata[k]=temp[i++];
         		else
         			recdata[k]=recdata2[j++];
         	}
         }
         else{
         	//store the larger of the two
         	int i,j,k;
         	for(i=j=k=localn-1;k>=0;k--){
         		if(j==-1||(i>=0 && temp[i]>=recdata2[j]))
         			recdata[k]=temp[i--];
         		else
         			recdata[k]=recdata2[j--];
         	}
         }
     }



    ierr=MPI_Gather(recdata,localn,MPI_INT,data,localn,MPI_INT,0,MPI_COMM_WORLD);
    //if(rank==root_process){
        //Mostrar resultado
    //}

    ierr = MPI_Finalize();
    local_finish = MPI_Wtime();
    local_elapsed = local_finish-local_start;
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(rank == 0)
        printf("Time = %e seconds\n", elapsed);

return 0;
}
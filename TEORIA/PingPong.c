#include <stdio.h>
#include <mpi.h>

#define PROC_A 0
#define PROC_B 1
#define TAG 0 //LONG_MSJ
#define ITERACIONES 10
//#define CON_ball  0

void proceso_A(); //Ping
void proceso_B(); //Pong

int main(void){
	MPI_Init(NULL, NULL);
	int my_rank, my_size;
	MPI_Comm_size(MPI_COMM_WORLD, &my_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 

	int sender, recver;
	
	int CON_ball = 0;
	//Implementacion Basica
	while(CON_ball < ITERACIONES)
	{
		sender = CON_ball % 2;
        recver = 1 - sender;
        
        if ( my_rank == sender ){
       		MPI_Send(&CON_ball, 1, MPI_INT, recver, TAG , MPI_COMM_WORLD);
          	printf("%d Send sendbuffer %d from %d\n",
             my_rank, CON_ball, sender);
        }
        else {
          MPI_Recv(&CON_ball, 1, MPI_INT, sender, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("%d received sendbuffer %d from %d\n",
             my_rank, CON_ball, sender);
        }

		
		MPI_Barrier(MPI_COMM_WORLD);
		//printf("Process %d iter %d ball value is : %d\n",my_rank,i,CON_ball);

	}


	/*
	//Implementacion 2
	if (my_rank == PROC_A) proceso_A();
	else if (my_rank == PROC_B) proceso_B();*/
	
	MPI_Finalize();

	return 0;
}

void proceso_A(){
	int Mensaje[TAG];

	double time;

	for (int i = 0; i < TAG; ++i)
	{
		time = MPI_Wtime();

		for (int j = 0; j < 10; ++j)
		{
			MPI_Ssend(Mensaje, i, MPI_INTEGER, PROC_B, 10,MPI_COMM_WORLD);
            MPI_Recv(Mensaje, i, MPI_INTEGER, PROC_B, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	//CON_ball = CON_ball + 1;

	time = MPI_Wtime() - time;
	printf("%f\t \n",time);
}

void proceso_B(){
	int Mensaje[TAG];

	double time;

	for (int i = 0; i < TAG; ++i)
	{
		time = MPI_Wtime();

		for (int j = 0; j < 10; ++j)
		{
			MPI_Recv(Mensaje, i, MPI_INTEGER, PROC_A, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Ssend(Mensaje, i, MPI_INTEGER, PROC_A, 10,MPI_COMM_WORLD);
		}
	}
	//CON_ball = CON_ball + 1;
	time = MPI_Wtime() - time;
	printf("%f\t \n",time);
}
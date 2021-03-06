/*
	Trapezoidal Rule using MPI
	Code from "Introduction to Parallel Programming" by Peter S. Pacheco. 
*/

#include <stdio.h>
#include <mpi.h>

void GetInput(int my_rank, int comm_sz, double* a_p, double* b_p, int* n);
double f(double x);
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len);


int main(void){
	//int my_rank, comm_sz, n=1024, local_n;
	//double a=0.0, b=3.0, h, local_a, local_b;
	int my_rank, comm_sz, n, local_n;
	double a, b, h, local_a, local_b;
	double local_int, total_int;
	int source;

	MPI_Init(NULL,NULL);		// Initial setup
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);	
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	GetInput(my_rank, comm_sz, &a, &b, &n);		
	h = (b-a)/n;		 // h is the same for all processes 				
	local_n = n/comm_sz; // So is the number of trapezoids 				

	local_a = a + my_rank*local_n*h;				
	local_b = local_a + local_n * h;				
	local_int = Trap(local_a,local_b,local_n,h);	


	if(my_rank != 0){
		//Send local integral to process 0;
		MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}
	else{
		total_int = local_int;
		for (source = 1;source < comm_sz;source ++){
			MPI_Recv(&local_int,1,MPI_DOUBLE, source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);		
			total_int += local_int;
		}
	}

	if(my_rank == 0){																		
		printf("With n = %d trapezoids, out estimate \n", n);
		printf("of the integral from %f to %f = %.15e\n",a,b,total_int);
	}

	//printf("       Proc %d of %d > Does anyone have a toothpick?\n",my_rank, comm_sz);

	MPI_Finalize();																			
	return 0;
}

void GetInput(
	int my_rank, 	/*in*/
	int comm_sz, 	/*in*/								
	double* a_p, 	/*out*/
	double* b_p,	/*out*/
	int* n_p		/*out*/){
	int dest;

	if(my_rank == 0){					
		printf("Enter a, b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for(dest = 1; dest < comm_sz; dest++){							
			MPI_Send(a_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
			MPI_Send(b_p,1,MPI_DOUBLE,dest,0,MPI_COMM_WORLD);
			MPI_Send(n_p,1,MPI_INT,dest,0,MPI_COMM_WORLD);
		}
	}
	else{ /* my rank != 0 */
		MPI_Recv(a_p,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(b_p,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(n_p,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);		
	}
}

double f(double x){
	return 2 * x + 1;
}

double Trap(
	double left_endpt	/*in*/,
	double right_endpt	/*in*/,
	int trap_count 		/*in*/,	
	double base_len		/*in*/){

	double estimate,x;
	int i;

	estimate = (f(left_endpt) + f(right_endpt))/2.0;	
	for (i = 1; i <= trap_count-1; i ++){
		x = left_endpt + i*base_len;
		estimate += f(x);
	}

	estimate = estimate*base_len;							

	return estimate;
} /* Trap */



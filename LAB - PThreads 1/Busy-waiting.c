#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <time.h>
#include <sys/time.h>
int thread_count;            
int n = 10000000;           
double sum = 0.0;
int flag = 0;          

void *Thread_sum(void* rank);

int main(int argc,char* argv[]){
   struct  timeval start;
   struct  timeval end; 
   long thread;
   pthread_t* thread_handles=NULL; 
   gettimeofday(&start,NULL);
   thread_count = strtol(argv[1],NULL,10);
   thread_handles =(pthread_t*) malloc(thread_count*sizeof(pthread_t));

   for(thread = 0;thread < thread_count;thread++){//Create threads
      pthread_create(&thread_handles[thread],NULL,Thread_sum,(void*)thread);
   }
   for(thread = 0;thread < thread_count;thread++){
     pthread_join(thread_handles[thread],NULL);
   }
   gettimeofday(&end,NULL);
 
   long long startusec=start.tv_sec*1000000+start.tv_usec;
   long long endusec=end.tv_sec*1000000+end.tv_usec;
   double elapsed=(double)(endusec-startusec)/1000000.0;
 
   printf("the result of π en %.4f seconds\n",elapsed);
   free(thread_handles);
 
   return 0;
}

void *Thread_sum(void *rank){
   long my_rank=(long)rank;
   double factor,my_sum = 0.0;
   long long i;
   long long my_n = n/thread_count; 
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   
   if(my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;
   for(i = my_first_i;i < my_last_i;i++,factor = -factor){
      my_sum += factor/(2*i+1);
   }
   while(flag != my_rank);
   sum += my_sum;
   flag = (flag+1) % thread_count;

   return NULL;
}

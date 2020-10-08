#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/time.h>
int thread_count;           
int n = 10000000;           
double sum = 0.0;
int flag = 0;
int count = 0;        
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void *Thread_sum(void* rank);

int main(int argc,char* argv[]){ 
   long thread;
   pthread_t* thread_handles=NULL;
   struct  timeval start;
   struct  timeval end;
   gettimeofday(&start,NULL);
   thread_count = strtol(argv[1],NULL,10);
   thread_handles = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
   pthread_mutex_init(&mutex,NULL);
   
   for(thread = 0;thread < thread_count;thread++){
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
   pthread_mutex_destroy(&mutex); 
   return 0;
}


void* Thread_sum(void *rank){
   long my_rank= (long) rank;
   double factor;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;
   double my_sum = 0.0;

   if(my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;
   
   for(i = my_first_i;i < my_last_i;i++,factor = -factor){
      my_sum += factor/(2*i+1);
   }

   pthread_mutex_lock(&mutex);
   sum += my_sum;
   pthread_mutex_unlock(&mutex);
   return NULL;
} /* Theread_sum */
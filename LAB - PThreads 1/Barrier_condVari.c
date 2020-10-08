#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/time.h>

#define BARRIER_COUNT 100

int thread_count;
int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_var;

void Usage(char* prog_name);
void *Thread_work(void* rank);

int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles; 

   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_var, NULL);

   struct  timeval start;
   struct  timeval end;

   gettimeofday(&start,NULL);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL, Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   gettimeofday(&end,NULL);
   long long startusec=start.tv_sec*1000000+start.tv_usec;
   long long endusec=end.tv_sec*1000000+end.tv_usec;
   double elapsed=(double)(endusec-startusec)/1000000.0;
   printf("the result of π en %.4f seconds\n",elapsed);


   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_var);
   free(thread_handles);
   return 0;
} /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
} /* Usage */

void *Thread_work(void* rank){
   long my_rank = (long) rank;
   int i;
   for (i = 0; i < BARRIER_COUNT; i++) {

      /* Barrier */
      pthread_mutex_lock(&mutex);
      counter++;
      if (counter == thread_count) {
         counter = 0;
         printf("Thread %ld > Signalling other threads in barrier %d\n", my_rank, i);
         fflush(stdout);
         pthread_cond_broadcast(&cond_var);
      } else {
         while (pthread_cond_wait(&cond_var, &mutex) != 0);
         printf("Thread %ld > Awakened in barrier %d\n", my_rank, i);
         fflush(stdout);
      }
      pthread_mutex_unlock(&mutex);

      
      if (my_rank == 0) {
         printf("All threads completed barrier %d\n", i);
         fflush(stdout);
      }
   }
   return NULL;
} /*Thread_word */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>

#define BARRIER_COUNT 100

int thread_count;
int counter;
sem_t barrier_sems[BARRIER_COUNT];
sem_t count_sem;

void Usage(char* prog_name);
void *Thread_work(void* rank);

int main(int argc, char* argv[]) {
   long       thread, i;
   pthread_t* thread_handles; 

   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_init(&barrier_sems[i], 0, 0);
   sem_init(&count_sem, 0, 1);

   struct  timeval start;
   struct  timeval end;

   gettimeofday(&start,NULL);

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], (pthread_attr_t*) NULL, Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }

   sem_destroy(&count_sem);
   for (i = 0; i < BARRIER_COUNT; i++)
      sem_destroy(&barrier_sems[i]);

   gettimeofday(&end,NULL);
   long long startusec=start.tv_sec*1000000+start.tv_usec;
   long long endusec=end.tv_sec*1000000+end.tv_usec;
   double elapsed=(double)(endusec-startusec)/1000000.0;
   printf("the result of π en %.4f seconds\n",elapsed);

   free(thread_handles);
   return 0;
} /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
} /* Usage */

void *Thread_work(void* rank){
   long my_rank = (long) rank;
   int i, j;
   for(i = 0; i < BARRIER_COUNT; i++){
      sem_wait(&count_sem);

      /* Barrier */
      if (counter == thread_count - 1) {
         counter = 0;
         sem_post(&count_sem);
         for (j = 0; j < thread_count-1; j++)
            sem_post(&barrier_sems[i]);
      } else {
         counter++;
         sem_post(&count_sem);
         sem_wait(&barrier_sems[i]);
      }

      if (my_rank == 0) {
         printf("All threads completed barrier %d\n", i);
         fflush(stdout);
      }
   }
   return NULL;
} /*Thread_word */
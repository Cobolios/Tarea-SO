#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
volatile int total_counter = 0; // Global total counter
pthread_mutex_t lock; // Mutex lock to protect access to total_counter

void *mythread(void *arg) {
  int i;
  int local_counter = 0; // Local counter for each thread

  for (i = 0; i < max; i++) {
    local_counter++; // Increment local counter
  }

  pthread_mutex_lock(&lock);
  total_counter += local_counter;
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: main-second <loopcount> <num_threads>\n");
    exit(1);
  }
  max = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  pthread_t threads[num_threads];
  pthread_mutex_init(&lock, NULL); // Initialize mutex

  printf("main: begin [total_counter = %d] [%p]\n", total_counter, &total_counter);

  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, mythread, NULL);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("main: done\n [total_counter: %d]\n [should: %d]\n", total_counter, max * num_threads);

  pthread_mutex_destroy(&lock); // Clean up mutex
  return 0;
}

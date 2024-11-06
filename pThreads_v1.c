#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
volatile int total_counter = 0; // Global total counter

pthread_mutex_t lock; // Mutex lock to protect access to total_counter

void *mythread(void *arg) {
  char *letter = arg;
  int i;
  int local_counter = 0; // Local counter for each thread
  
  printf("%s: begin [addr of i: %p]\n", letter, &i);
  for (i = 0; i < max; i++) {
    local_counter++; // Increment local counter
  }
  
  // Add the local counter to the global total counter
  pthread_mutex_lock(&lock);
  total_counter += local_counter;
  pthread_mutex_unlock(&lock);

  printf("%s: done\n", letter);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: main-first <loopcount>\n");
    exit(1);
  }
  max = atoi(argv[1]);

  pthread_t p1, p2;
  pthread_mutex_init(&lock, NULL); // Initialize mutex

  printf("main: begin [total_counter = %d] [%p]\n", total_counter, &total_counter);
  pthread_create(&p1, NULL, mythread, "A");
  pthread_create(&p2, NULL, mythread, "B");

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  printf("main: done\n [total_counter: %d]\n [should: %d]\n", total_counter, max * 2);

  pthread_mutex_destroy(&lock); // Clean up mutex
  return 0;
}

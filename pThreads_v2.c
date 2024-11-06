/*
 * Esta version extiende la solución de concurrencia permitiendo que el
 * número de hilos sea variable. Cada hilo mantiene su propio contador local
 * y, al finalizar, suma su valor al contador global total. El usuario puede
 * especificar el número de iteraciones (max) y el número de hilos (num_threads),
 * garantizando el resultado correcto independientemente de la cantidad de hilos
 * Cristian Guaza (2372225)
 * 5-11-2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;
volatile int total_counter = 0; 
pthread_mutex_t lock; 

void *mythread(void *arg) {
  int i;
  int local_counter = 0; 

  for (i = 0; i < max; i++) {
    local_counter++; 
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
  pthread_mutex_init(&lock, NULL); 

  printf("main: begin [total_counter = %d] [%p]\n", total_counter, &total_counter);

  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, mythread, NULL);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("main: done\n [total_counter: %d]\n [should: %d]\n", total_counter, max * num_threads);

  pthread_mutex_destroy(&lock); 
  return 0;
}

#include "sync.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_THREADS_WAITING 2

#define NUM_READERS 3
#define NUM_WRITERS 2

Semaphore semaphore;
Barrier barrier;
RWLock rwlock;


/* -------------------------------test Semaphore ------------------*/
void *test_Semaphore(void *arg) {
  int thread_id = *((int *)arg);
  printf("Hilo %d esperando para acceder al semáforo...\n", thread_id);
  Semaphore_wait(&semaphore);

  printf("Hilo %d ha entrado en la sección crítica.\n", thread_id);
  // Simula trabajo en la sección crítica
  sleep(2);
  printf("Hilo %d está saliendo de la sección crítica.\n", thread_id);

  // Incrementar el valor del semáforo (señalizar)
  Semaphore_signal(&semaphore);

  return NULL;
}

/* -------------------------------test Barrier ------------------*/
void *test_Barrier(void *arg) {
  int thread_id = *((int *)arg);
  printf("Hilo %d esperando en la barrera...\n", thread_id);

  Barrier_wait(&barrier);

  printf("Hilo %d ha pasado la barrera.\n", thread_id);

  return NULL;
}

/* -------------------------------test RWLock ------------------*/
void *test_reader(void *arg) {
  int thread_id = *((int *)arg);
  printf("Lector %d esperando para leer...\n", thread_id);

  RWLock_begin_read(&rwlock);
  printf("Lector %d está leyendo.\n", thread_id);
  sleep(1); 
  printf("Lector %d terminó de leer.\n", thread_id);
  RWLock_end_read(&rwlock);

  return NULL;
}

void *test_writer(void *arg) {
  int thread_id = *((int *)arg);
  printf("Escritor %d esperando para escribir...\n", thread_id);

  RWLock_begin_write(&rwlock);
  printf("Escritor %d está escribiendo.\n", thread_id);
  sleep(2);
  printf("Escritor %d terminó de escribir.\n", thread_id);
  RWLock_end_write(&rwlock);

  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
  int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

  printf("Semaphore\n");

  // Inicializar el semaforo
  Semaphore_init(&semaphore, NUM_THREADS_WAITING);
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, test_Semaphore, &thread_ids[i]);
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("\n");
  printf("Barrera\n");

  // Inicializar la barrera
  Barrier_init(&barrier, NUM_THREADS);
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, test_Barrier, &thread_ids[i]);
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  printf("\n");
  printf("RW LOCK\n");
  // Inicializar el Read/Write Lock
  RWLock_init(&rwlock);

  for (int i = 0; i < NUM_READERS; i++) {
    reader_ids[i] = i;
    pthread_create(&readers[i], NULL, test_reader, &reader_ids[i]);
  }

  for (int i = 0; i < NUM_WRITERS; i++) {
    writer_ids[i] = i;
    pthread_create(&writers[i], NULL, test_writer, &writer_ids[i]);
  }

  for (int i = 0; i < NUM_READERS; i++) {
    pthread_join(readers[i], NULL);
  }

  for (int i = 0; i < NUM_WRITERS; i++) {
    pthread_join(writers[i], NULL);
  }

  return 0;
}

#include <pthread.h>

typedef struct {
  int value;
  pthread_mutex_t mutex;
  pthread_cond_t cond;  
} Semaphore;

typedef struct {
    int count;
    int total;
    pthread_mutex_t mutex;
    pthread_cond_t cond;  
} Barrier;

typedef struct {
    int num_readers_active;
    int num_writers_waiting;
    int writer_active;
    pthread_mutex_t mutex;
    pthread_cond_t cond_writers;
    pthread_cond_t cond_readers;
} RWLock;

void Semaphore_init(Semaphore *s, int value);
void Semaphore_wait(Semaphore *s);
void Semaphore_signal(Semaphore *s);

void Barrier_init(Barrier *b, int total);
void Barrier_wait(Barrier *b);

void RWLock_init(RWLock *rw);
void RWLock_begin_read(RWLock *rw);
void RWLock_end_read(RWLock *rw);
void RWLock_begin_write(RWLock *rw);
void RWLock_end_write(RWLock *rw);



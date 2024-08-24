#include"sync.h"
#include<stdatomic.h>
#include <pthread.h>

// Semaphore
void Semaphore_init(Semaphore *s, int value) {
    s->value = value;  
    pthread_mutex_init(&s->mutex, NULL); 
    pthread_cond_init(&s->cond, NULL);
}

void Semaphore_wait(Semaphore *s) {
    pthread_mutex_lock(&s->mutex);    
    while (s->value <= 0) {
        pthread_cond_wait(&s->cond, &s->mutex);
    }
    s->value--;            
    pthread_mutex_unlock(&s->mutex); 
}

void Semaphore_signal(Semaphore *s) {
    pthread_mutex_lock(&s->mutex);    
    s->value++;               
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex); 
}

// Barrier
void Barrier_init(Barrier *b, int total) {
    b->count = 0;
    b->total = total;
    pthread_mutex_init(&b->mutex, NULL); 
    pthread_cond_init(&b->cond, NULL);
}

void Barrier_wait(Barrier *b) {
    pthread_mutex_lock(&b->mutex);    
    b->count++;
    if (b->count == b->total) {
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    } else {
        pthread_cond_wait(&b->cond, &b->mutex);
    }
    pthread_mutex_unlock(&b->mutex);
}

// Read/Write lock
void RWLock_init(RWLock *rw) {
    rw->num_readers_active = 0;
    rw->num_writers_waiting = 0;
    rw->writer_active = 0;
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->cond_writers, NULL);
    pthread_cond_init(&rw->cond_readers, NULL);
}

void RWLock_begin_read(RWLock *rw) {
    pthread_mutex_lock(&rw->mutex); 
    while (rw->num_writers_waiting > 0 || rw->writer_active) { 
        pthread_cond_wait(&rw->cond_readers, &rw->mutex); 
    }
    rw->num_readers_active++; 
    pthread_mutex_unlock(&rw->mutex); 
}

void RWLock_end_read(RWLock *rw) {
    pthread_mutex_lock(&rw->mutex); 
    rw->num_readers_active--; 
    if (rw->num_readers_active == 0) { 
        pthread_cond_broadcast(&rw->cond_writers); 
    }
    pthread_mutex_unlock(&rw->mutex); 
}

void RWLock_begin_write(RWLock *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->num_writers_waiting++; 
    while (rw->num_readers_active > 0 || rw->writer_active) { 
        pthread_cond_wait(&rw->cond_writers, &rw->mutex);
    }
    rw->num_writers_waiting--; 
    rw->writer_active = 1; 
    pthread_mutex_unlock(&rw->mutex); 
}

void RWLock_end_write(RWLock *rw) {
    pthread_mutex_lock(&rw->mutex); 
    rw->writer_active = 0; 
    pthread_cond_broadcast(&rw->cond_writers); 
    pthread_mutex_unlock(&rw->mutex); 
}



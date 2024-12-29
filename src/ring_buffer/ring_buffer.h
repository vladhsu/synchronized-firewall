#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

typedef struct ring_buffer_t {
	char *data;
	size_t read_pos;
	size_t write_pos;
	bool stop;
	size_t len;
	size_t cap;
	pthread_mutex_t mutex;
	sem_t empty;
	sem_t count;
} ring_buffer_t;

int     ring_buffer_init(ring_buffer_t *rb, size_t cap);
ssize_t ring_buffer_enqueue(ring_buffer_t *rb, void *data, size_t size);
ssize_t ring_buffer_dequeue(ring_buffer_t *rb, void *data, size_t size);
void    ring_buffer_destroy(ring_buffer_t *rb);
void    ring_buffer_stop(ring_buffer_t *rb);

#endif
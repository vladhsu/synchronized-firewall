#include <stdio.h>
#include "ring_buffer.h"

int ring_buffer_init(ring_buffer_t *ring, size_t cap)
{
	ring->data = (char *)malloc(cap);
	if(!ring->data){
		printf("Error while allocing space for data");
		return -1;
	}

	ring->len = 0;
	ring->cap = cap;
	ring->stop = false;
	ring->read_pos = ring->write_pos = 0;

	pthread_mutex_init(&ring->mutex, NULL);
	sem_init(&ring->count, 0, 0);
	sem_init(&ring->empty, 0, 1000);

	return 1;
}

ssize_t ring_buffer_enqueue(ring_buffer_t *ring, void *data, size_t size)
{
	sem_wait(&ring->empty);
	pthread_mutex_lock(&ring->mutex);

	char *ch_data = data;

	for (size_t i = 0; i < size; i++) {
		ring->data[ring->write_pos] = ch_data[i];
		ring->write_pos = (ring->write_pos + 1) % ring->cap;
	}
	ring->len++;

	pthread_mutex_unlock(&ring->mutex);
	sem_post(&ring->count);
	return (ssize_t)size;
}

ssize_t ring_buffer_dequeue(ring_buffer_t *ring, void *data, size_t size)
{
	sem_wait(&ring->count);
	pthread_mutex_lock(&ring->mutex);

	if (ring->len == 0 && ring->stop) {
		pthread_mutex_unlock(&ring->mutex);
		return -1;
	}

	char *ch_data = data;

	for (size_t i = 0; i < size; i++) {
		ch_data[i] = (ring->data)[ring->read_pos];
		ring->read_pos = (ring->read_pos + 1) % ring->cap;
	}
	ring->len--;
	pthread_mutex_unlock(&ring->mutex);
	sem_post(&ring->empty);
	return (ssize_t)size;
}

void ring_buffer_destroy(ring_buffer_t *ring)
{
	free(ring->data);
	pthread_mutex_destroy(&ring->mutex);
	sem_destroy(&ring->empty);
	sem_destroy(&ring->count);
}

void ring_buffer_stop(ring_buffer_t *ring)
{
	pthread_mutex_lock(&ring->mutex);
	ring->stop = true;
	for (size_t i = 0; i < ring->cap; i++)
		sem_post(&ring->count);
	pthread_mutex_unlock(&ring->mutex);
}

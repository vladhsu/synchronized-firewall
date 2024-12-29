#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

#include "producer/producer.h"
#include "consumer/consumer.h"

#define SO_RING_SZ		(256 * 1000)

int main(int argc, char **argv)
{
	ring_buffer_t ring_buffer;
	int num_consumers, threads, rc;
	pthread_t *thread_ids = NULL;

	if (argc < 4) {
		fprintf(stderr, "Usage %s <input-file> <output-file> <num-consumers:1-32>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	rc = ring_buffer_init(&ring_buffer, SO_RING_SZ);
	if(rc < 0){
		printf("Ring buffer init error");
		exit(EXIT_FAILURE);
	}

	num_consumers = strtol(argv[3], NULL, 10);

	if (num_consumers <= 0 || num_consumers > 32) {
		fprintf(stderr, "num-consumers [%d] must be in the interval [1-32]\n", num_consumers);
		exit(EXIT_FAILURE);
	}

	thread_ids = calloc(num_consumers, sizeof(pthread_t));
	if(thread_ids == NULL){
		printf("Thread ids error");
		exit(EXIT_FAILURE);
	}

	/* create consumer threads */
	threads = create_consumers(thread_ids, num_consumers, &ring_buffer, argv[2]);

	/* start publishing data */
	publish_data(&ring_buffer, argv[1]);

	/* Wait for child threads */
	for (int i = 0; i < threads; i++)
		pthread_join(thread_ids[i], NULL);

	free(thread_ids);

	return 0;
}


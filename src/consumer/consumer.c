#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "consumer.h"

void *consumer_thread(consumer_context_t *ctx)
{
	while (true) {
		packet_t *data = (packet_t *)malloc(PKT_SZ);
		ssize_t bytes = ring_buffer_dequeue(ctx->producer_rb, data, PKT_SZ);

		if (bytes < 0)
			break;

		decision_t result;
		unsigned long hash;

		result = process_packet(data);
		hash = packet_hash(data);
		pthread_mutex_lock(&ctx->mutex);
		FILE *out_file = fopen(ctx->out_filename, "a");

		if (!out_file) {
			pthread_mutex_unlock(&ctx->mutex);
			break;
		}

		char buffer[256];

		snprintf(buffer, 256, "%s %016lx %lu\n", RES_TO_STR(result), hash, data->hdr.timestamp);
		fwrite(buffer, 1, strlen(buffer), out_file);
		fclose(out_file);
		pthread_mutex_unlock(&ctx->mutex);
		free(data);
	}
	pthread_exit(NULL);
}

int create_consumers(pthread_t *tids, int num_consumers,
					 struct ring_buffer_t *rb, const char *out_filename)
{
	for (int i = 0; i < num_consumers; i++) {
		consumer_context_t *ctx =
			(consumer_context_t *)malloc(sizeof(consumer_context_t));

		ctx->producer_rb = rb;
		ctx->out_filename = strdup(out_filename);
		pthread_mutex_t mutex;

		pthread_mutex_init(&mutex, NULL);
		ctx->mutex = mutex;
		pthread_create(&tids[i], NULL, (void *)consumer_thread, (void *)ctx);
	}

	return num_consumers;
}

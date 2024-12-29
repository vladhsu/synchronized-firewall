#include <pthread.h>
#include "../ring_buffer/ring_buffer.h"
#include "../packet/packet.h"

typedef struct consumer_context_t {
	struct ring_buffer_t *producer_rb;
	const char *out_filename;
	pthread_mutex_t mutex;
} consumer_context_t;

int create_consumers(pthread_t *tids,
					int num_consumers,
					ring_buffer_t *rb,
					const char *out_filename);

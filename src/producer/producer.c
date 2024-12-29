#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "producer.h"

void publish_data(ring_buffer_t *rb, const char *filename)
{
	char buffer[PKT_SZ];
	ssize_t sz;
	int fd;

	fd = open(filename, O_RDONLY);
	if(fd < 0){
		printf("open");
		return;
	}

	while ((sz = read(fd, buffer, PKT_SZ)) != 0) {
		if(sz != PKT_SZ){
			printf("packet truncated");
			return;
		}

		/* enequeue packet into ring buffer */
		ring_buffer_enqueue(rb, buffer, sz);
	}

	ring_buffer_stop(rb);
}

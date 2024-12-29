#include <stdint.h>
#include "packet.h"

#define HASH_ITER 50

unsigned long packet_hash(const struct packet_t *pkt)
{
    uint64_t hash = 0xcbf29ce484222325;
    const char *pkt_it = (const char *)pkt;

    for (int i = 0; i < PKT_SZ; i++) {
        hash ^= (unsigned char)pkt_it[i];
        hash *= 0x100000001b3;
    }

    return (unsigned long)hash;
}

static struct range {
	unsigned int start;
	unsigned int end;
} allowed_sources_range[] = {
	{
		.start = 0xf1000000,
		.end   = 0xf1ffffff,
	}, {
		.start = 0x1f1f1f1f,
		.end   = 0x1f1f1f1f,
	}, {
		.start = 0x80000000,
		.end   = 0xffffffff,
	}
};

decision_t process_packet(const struct packet_t *pkt)
{
	const int sz = sizeof(allowed_sources_range) / sizeof(allowed_sources_range[0]);

	for (int i = 0; i < sz; i++) {
		if (allowed_sources_range[i].start <= pkt->hdr.source &&
				pkt->hdr.source <= allowed_sources_range[i].end)
			return PASS;
	}

	return DROP;
}

#ifndef PACKET_H
#define PACKET_H

#ifndef __packed
#define __packed __attribute((__packed__))
#endif

#define PKT_SZ 256

typedef enum {
	DROP = 0,
	PASS = 1,
} decision_t;

#define RES_TO_STR(decision) ((decision == PASS) ? "PASS" : "DROP")

typedef struct __packed packet_header_t {
	unsigned int source;
	unsigned int dest;
	unsigned long timestamp;
} packet_header_t;

typedef struct __packed packet_t {
	packet_header_t hdr;
	char payload[PKT_SZ - sizeof(packet_header_t)];
} packet_t;

unsigned long packet_hash(const packet_t *pkt);
decision_t process_packet(const packet_t *pkt);

#endif
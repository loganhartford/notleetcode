// Time: O(1) per byte fed — simple state machine
#include <stdint.h>
#include <stddef.h>
#include <string.h>


typedef enum { PARSER_NONE=0, PARSER_PACKET, PARSER_ERROR } parser_result_t;
#define PARSER_MAX_PAYLOAD 64
typedef struct {
    int     state;
    uint8_t len;
    uint8_t idx;
    uint8_t buf[PARSER_MAX_PAYLOAD];
    uint8_t cksum;
} parser_t;

#define WAIT_START  0
#define READ_LEN    1
#define READ_PAYLOAD 2
#define READ_CKSUM  3

void parser_init(parser_t *p) {
    p->state = WAIT_START;
    p->len   = 0;
    p->idx   = 0;
    p->cksum = 0;
}

parser_result_t parser_feed(parser_t *p, uint8_t byte,
                             uint8_t *payload_out, size_t *len_out) {
    switch (p->state) {
    case WAIT_START:
        if (byte == 0xAA) p->state = READ_LEN;
        return PARSER_NONE;

    case READ_LEN:
        if (byte > PARSER_MAX_PAYLOAD) {
            p->state = WAIT_START;
            return PARSER_ERROR;
        }
        p->len   = byte;
        p->idx   = 0;
        p->cksum = byte; /* start checksum with LEN */
        if (byte == 0) {
            p->state = READ_CKSUM;
        } else {
            p->state = READ_PAYLOAD;
        }
        return PARSER_NONE;

    case READ_PAYLOAD:
        p->buf[p->idx++] = byte;
        p->cksum = (uint8_t)(p->cksum + byte);
        if (p->idx == p->len) p->state = READ_CKSUM;
        return PARSER_NONE;

    case READ_CKSUM:
        p->state = WAIT_START;
        if (byte != p->cksum) return PARSER_ERROR;
        memcpy(payload_out, p->buf, p->len);
        *len_out = p->len;
        return PARSER_PACKET;
    }
    return PARSER_NONE;
}

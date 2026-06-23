#include <stdint.h>

#define PKT_START 0xAA

enum { WAIT_START, READ_LEN, READ_PAYLOAD, READ_CKSUM };

typedef struct {
    int     state;
    uint8_t len;
    uint8_t idx;
    uint8_t buf[256];
    uint8_t cksum;
} parser_t;

void parser_init(parser_t* p) {
    p->state = WAIT_START;
    p->len = 0;
    p->idx = 0;
    p->cksum = 0;
}

int parser_feed(parser_t* p, uint8_t byte, uint8_t* out, int* out_len) {
    switch (p->state) {
        case WAIT_START:
            if (byte == PKT_START) p->state = READ_LEN;
            break;
        case READ_LEN:
            p->len = byte;
            p->idx = 0;
            p->cksum = 0;
            p->state = (p->len == 0) ? READ_CKSUM : READ_PAYLOAD;
            break;
        case READ_PAYLOAD:
            p->buf[p->idx++] = byte;
            p->cksum ^= byte;
            if (p->idx == p->len) p->state = READ_CKSUM;
            break;
        case READ_CKSUM:
            p->state = WAIT_START;
            if (byte == p->cksum) {
                for (int i = 0; i < p->len; i++) out[i] = p->buf[i];
                *out_len = p->len;
                return 1;
            }
            break;
    }
    return 0;
}

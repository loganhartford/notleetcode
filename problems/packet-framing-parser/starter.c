#include <stdint.h>

#define PKT_START 0xAA

typedef struct {
    int     state;
    uint8_t len;
    uint8_t idx;
    uint8_t buf[256];
    uint8_t cksum;
} parser_t;

void parser_init(parser_t* p) {
}

/* Return 1 and fill out/out_len when a valid frame completes; else 0. */
int parser_feed(parser_t* p, uint8_t byte, uint8_t* out, int* out_len) {
    return 0;
}

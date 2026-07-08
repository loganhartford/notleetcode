#include <stdint.h>
#include <stddef.h>


typedef enum { PARSER_NONE=0, PARSER_PACKET, PARSER_ERROR } parser_result_t;
typedef enum { WAIT_START=0, READ_LEN, READ_PAYLOAD, READ_CKSUM};
#define PARSER_MAX_PAYLOAD 64
typedef struct {
    int     state;
    uint8_t len;
    uint8_t idx;
    uint8_t buf[PARSER_MAX_PAYLOAD];
    uint8_t cksum;
} parser_t;

void parser_init(parser_t *p) {
    p->state = WAIT_START;
    p->len = PARSER_MAX_PAYLOAD;
    p->idx = 0;
    p->cksum = 0;
}

parser_result_t parser_feed(parser_t *p, uint8_t byte,
                             uint8_t *payload_out, size_t *len_out) {
    
    return PARSER_NONE;
}

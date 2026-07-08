#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static void chk_result(const char *nm, const char *in,
                        parser_result_t got, parser_result_t exp) {
    const char *names[] = {"NONE","PACKET","ERROR"};
    nlc_case(nm, got == exp, in, names[exp], names[got], "");
}
static void chk_mem(const char *nm, const char *in,
                    const uint8_t *got, const uint8_t *exp, size_t n) {
    int ok = memcmp(got, exp, n) == 0;
    char g[64]="", e[64]="";
    for (size_t i = 0; i < n && i < 8; i++) {
        char tmp[8];
        snprintf(tmp,sizeof tmp,"%02X ",got[i]); strncat(g,tmp,sizeof g-strlen(g)-1);
        snprintf(tmp,sizeof tmp,"%02X ",exp[i]); strncat(e,tmp,sizeof e-strlen(e)-1);
    }
    nlc_case(nm, ok, in, e, g, "");
}

/* Feed a sequence of bytes; return final result */
static parser_result_t feed_seq(parser_t *p, const uint8_t *bytes, size_t n,
                                 uint8_t *pout, size_t *lout) {
    parser_result_t r = PARSER_NONE;
    for (size_t i = 0; i < n; i++) {
        r = parser_feed(p, bytes[i], pout, lout);
    }
    return r;
}

int main(void) {
    nlc_begin();

    parser_t p;
    uint8_t  pout[PARSER_MAX_PAYLOAD];
    size_t   lout;

    /* valid empty payload: AA 00 [cksum=00] */
    parser_init(&p);
    uint8_t empty_pkt[] = {0xAA, 0x00, 0x00};
    parser_result_t r = feed_seq(&p, empty_pkt, sizeof empty_pkt, pout, &lout);
    chk_result("empty payload PACKET", "feed AA 00 00", r, PARSER_PACKET);
    {
        char g[8], e[8];
        snprintf(g,sizeof g,"%zu",lout); snprintf(e,sizeof e,"0");
        nlc_case("empty payload len=0",(int)(lout==0),"feed AA 00 00","0",g,"");
    }

    /* valid 3-byte payload: AA 03 11 22 33 [cksum=03+11+22+33=0x69] */
    parser_init(&p);
    uint8_t pkt3[] = {0xAA, 0x03, 0x11, 0x22, 0x33, 0x69};
    r = feed_seq(&p, pkt3, sizeof pkt3, pout, &lout);
    chk_result("3-byte payload PACKET", "feed 3-byte pkt", r, PARSER_PACKET);
    uint8_t exp3[] = {0x11,0x22,0x33};
    chk_mem("3-byte payload data","payload data", pout, exp3, 3);

    /* noise before packet: DE AD AA 01 55 [cksum=01+55=0x56] */
    parser_init(&p);
    uint8_t noisy[] = {0xDE, 0xAD, 0xAA, 0x01, 0x55, 0x56};
    r = feed_seq(&p, noisy, sizeof noisy, pout, &lout);
    chk_result("noise before PACKET", "feed noisy then pkt", r, PARSER_PACKET);

    /* bad checksum: AA 02 10 20 FF (expected 0x32) */
    parser_init(&p);
    uint8_t bad_ck[] = {0xAA, 0x02, 0x10, 0x20, 0xFF};
    r = feed_seq(&p, bad_ck, sizeof bad_ck, pout, &lout);
    chk_result("bad checksum ERROR", "feed bad checksum", r, PARSER_ERROR);

    /* oversized LEN: AA 65 -> LEN=101 > 64 */
    parser_init(&p);
    r = parser_feed(&p, 0xAA, pout, &lout);
    r = parser_feed(&p, 0x65, pout, &lout);
    chk_result("oversized len ERROR", "LEN=101 > MAX", r, PARSER_ERROR);

    /* after error, parser resets and finds next packet */
    parser_init(&p);
    uint8_t recover[] = {0xAA, 0x65, /* error */
                          0xAA, 0x01, 0x42, 0x43 /* valid: cksum=01+42=0x43 */};
    r = PARSER_NONE;
    for (size_t i = 0; i < sizeof recover; i++)
        r = parser_feed(&p, recover[i], pout, &lout);
    chk_result("recover after error", "error then valid pkt", r, PARSER_PACKET);

    nlc_end();
    return 0;
}

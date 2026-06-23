#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

int main(void) {
    /* AA 03 10 20 30 00 | FF | AA 01 55 55 | AA 02 01 02 FF */
    uint8_t stream[] = {
        0xAA, 0x03, 0x10, 0x20, 0x30, 0x00,
        0xFF,
        0xAA, 0x01, 0x55, 0x55,
        0xAA, 0x02, 0x01, 0x02, 0xFF
    };
    int n = (int)(sizeof stream / sizeof stream[0]);

    uint8_t payloads[8][256];
    int lens[8];
    int delivered = 0;

    parser_t p;
    nlc_capture_begin();
    parser_init(&p);
    for (int i = 0; i < n; i++) {
        uint8_t out[256];
        int out_len = 0;
        if (parser_feed(&p, stream[i], out, &out_len)) {
            lens[delivered] = out_len;
            for (int j = 0; j < out_len; j++) payloads[delivered][j] = out[j];
            delivered++;
        }
    }
    char* cap = nlc_capture_end();

    nlc_begin();
    {
        char g[16], e[16];
        snprintf(g, sizeof g, "%d", delivered);
        snprintf(e, sizeof e, "%d", 2);
        nlc_case("delivers 2 packets", delivered == 2, "16-byte stream", e, g, cap);
    }
    if (delivered >= 1) {
        int exp0[] = {0x10, 0x20, 0x30};
        int got0[256];
        for (int j = 0; j < lens[0]; j++) got0[j] = payloads[0][j];
        int ok = lens[0] == 3;
        for (int j = 0; ok && j < 3; j++) if (got0[j] != exp0[j]) ok = 0;
        char gb[128], eb[128];
        nlc_fmt_ints(gb, sizeof gb, got0, lens[0]);
        nlc_fmt_ints(eb, sizeof eb, exp0, 3);
        nlc_case("packet 1 payload", ok, "first frame", eb, gb, "");
    } else {
        nlc_case("packet 1 payload", 0, "first frame", "[16, 32, 48]", "(none)", "");
    }
    if (delivered >= 2) {
        int ok = lens[1] == 1 && payloads[1][0] == 0x55;
        char gb[64], eb[64];
        int got1[1] = {lens[1] >= 1 ? payloads[1][0] : -1};
        int exp1[1] = {0x55};
        nlc_fmt_ints(gb, sizeof gb, got1, 1);
        nlc_fmt_ints(eb, sizeof eb, exp1, 1);
        nlc_case("packet 2 payload", ok, "second frame", eb, gb, "");
    } else {
        nlc_case("packet 2 payload", 0, "second frame", "[85]", "(none)", "");
    }
    nlc_end();
    free(cap);
    return 0;
}

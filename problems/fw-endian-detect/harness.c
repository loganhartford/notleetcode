#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"
#include <arpa/inet.h>

static void fmt_mem(char *buf, size_t cap, uint32_t v) {
    unsigned char b[4];
    memcpy(b, &v, sizeof b);
    snprintf(buf, cap, "%02X %02X %02X %02X", b[0], b[1], b[2], b[3]);
}

/* Value returned by my_htonl must be laid out in memory as these bytes. */
static void chk_mem(const char *name, uint32_t x, const char *expected_bytes) {
    char in[48], g[32];
    uint32_t got = my_htonl(x);
    fmt_mem(g, sizeof g, got);
    snprintf(in, sizeof in, "bytes in memory of my_htonl(0x%08X)", x);
    nlc_case(name, strcmp(g, expected_bytes) == 0, in, expected_bytes, g, "");
}

static void chk_val(const char *name, uint32_t x, uint32_t expected) {
    char in[32], e[16], g[16];
    uint32_t got = my_htonl(x);
    snprintf(in, sizeof in, "my_htonl(0x%08X)", x);
    snprintf(e, sizeof e, "0x%08X", expected);
    snprintf(g, sizeof g, "0x%08X", got);
    nlc_case(name, got == expected, in, e, g, "");
}

static void chk_sys(uint32_t x) {
    char name[48];
    snprintf(name, sizeof name, "matches system htonl(0x%08X)", x);
    chk_val(name, x, htonl(x));
}

int main(void) {
    nlc_begin();

    {
        int ref = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
        int got = is_little_endian();
        char g[16];
        snprintf(g, sizeof g, "%d", got);
        nlc_case("is_little_endian() matches this host",
                 (got != 0) == ref, "is_little_endian()",
                 ref ? "1" : "0", g, "");
    }

    chk_mem("my_htonl(0x12345678) is big-endian in memory", 0x12345678u, "12 34 56 78");
    chk_mem("my_htonl(0x000000FF) is big-endian in memory", 0x000000FFu, "00 00 00 FF");
    chk_val("zero", 0x00000000u, 0x00000000u);
    chk_val("all ones", 0xFFFFFFFFu, 0xFFFFFFFFu);

    {
        static const uint32_t vals[] = {0x12345678u, 0xDEADBEEFu, 0x00000001u, 0x80000000u};
        int ok = 1;
        char g[64] = "all round-trip";
        for (size_t i = 0; i < sizeof vals / sizeof vals[0]; i++) {
            uint32_t r = my_htonl(my_htonl(vals[i]));
            if (r != vals[i]) {
                snprintf(g, sizeof g, "my_htonl(my_htonl(0x%08X)) = 0x%08X", vals[i], r);
                ok = 0;
                break;
            }
        }
        nlc_case("my_htonl(my_htonl(x)) == x",
                 ok, "x in {0x12345678, 0xDEADBEEF, 0x00000001, 0x80000000}",
                 "all round-trip", g, "");
    }

    chk_sys(0x12345678u);
    chk_sys(0xDEADBEEFu);
    chk_sys(0x00000001u);
    chk_sys(0x80000000u);
    chk_sys(0x0000FF00u);
    chk_sys(0xA1B2C3D4u);

    nlc_end();
    return 0;
}

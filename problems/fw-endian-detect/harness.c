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

/* Pass a simulated memory layout of the uint32_t value 1. The union keeps
   the bytes 4-byte aligned in case the solution reads them as a word. */
static void chk_layout(const char *name, const uint8_t bytes[4], int expected) {
    union { uint32_t w; uint8_t b[4]; } mem;
    memcpy(mem.b, bytes, 4);
    int got = is_little_endian_layout(&mem);
    char in[64], e[4], g[16];
    snprintf(in, sizeof in, "is_little_endian_layout(bytes {%02X %02X %02X %02X})",
             bytes[0], bytes[1], bytes[2], bytes[3]);
    snprintf(e, sizeof e, "%d", expected);
    snprintf(g, sizeof g, "%d", got);
    nlc_case(name, (got != 0) == expected, in, e, g, "");
}

static void chk_order(const char *name, uint32_t x, int host_is_little, uint32_t expected) {
    char in[48], e[16], g[16];
    uint32_t got = to_network_order(x, host_is_little);
    snprintf(in, sizeof in, "to_network_order(0x%08X, %d)", x, host_is_little);
    snprintf(e, sizeof e, "0x%08X", expected);
    snprintf(g, sizeof g, "0x%08X", got);
    nlc_case(name, got == expected, in, e, g, "");
}

/* Value returned by my_htonl must be laid out in memory as these bytes. */
static void chk_mem(const char *name, uint32_t x, const char *expected_bytes) {
    char in[48], g[32];
    uint32_t got = my_htonl(x);
    fmt_mem(g, sizeof g, got);
    snprintf(in, sizeof in, "bytes in memory of my_htonl(0x%08X)", x);
    nlc_case(name, strcmp(g, expected_bytes) == 0, in, expected_bytes, g, "");
}

static void chk_sys(uint32_t x) {
    char name[48], in[32], e[16], g[16];
    uint32_t got = my_htonl(x), expected = htonl(x);
    snprintf(name, sizeof name, "matches system htonl(0x%08X)", x);
    snprintf(in, sizeof in, "my_htonl(0x%08X)", x);
    snprintf(e, sizeof e, "0x%08X", expected);
    snprintf(g, sizeof g, "0x%08X", got);
    nlc_case(name, got == expected, in, e, g, "");
}

int main(void) {
    nlc_begin();

    static const uint8_t le_one[4] = {0x01, 0x00, 0x00, 0x00};
    static const uint8_t be_one[4] = {0x00, 0x00, 0x00, 0x01};
    chk_layout("little-endian layout of 1", le_one, 1);
    chk_layout("big-endian layout of 1", be_one, 0);

    {
        int ref = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;
        int got = is_little_endian();
        char g[16];
        snprintf(g, sizeof g, "%d", got);
        nlc_case("is_little_endian() matches this host",
                 (got != 0) == ref, "is_little_endian()",
                 ref ? "1" : "0", g, "");
    }

    chk_order("little-endian host swaps",           0x12345678u, 1, 0x78563412u);
    chk_order("big-endian host leaves it alone",    0x12345678u, 0, 0x12345678u);
    chk_order("little-endian host: low byte moves to top", 0x000000FFu, 1, 0xFF000000u);
    chk_order("big-endian host: DEADBEEF unchanged", 0xDEADBEEFu, 0, 0xDEADBEEFu);
    chk_order("little-endian host: top byte moves to bottom", 0x80000000u, 1, 0x00000080u);
    chk_order("little-endian host: middle bytes swap", 0x0000FF00u, 1, 0x00FF0000u);
    chk_order("zero", 0x00000000u, 1, 0x00000000u);
    chk_order("all ones", 0xFFFFFFFFu, 1, 0xFFFFFFFFu);

    {
        static const uint32_t vals[] = {0x12345678u, 0xDEADBEEFu, 0x00000001u, 0x80000000u};
        int ok = 1;
        char g[64] = "all round-trip";
        for (size_t i = 0; i < sizeof vals / sizeof vals[0]; i++) {
            uint32_t r = to_network_order(to_network_order(vals[i], 1), 1);
            if (r != vals[i]) {
                snprintf(g, sizeof g, "twice on 0x%08X gives 0x%08X", vals[i], r);
                ok = 0;
                break;
            }
        }
        nlc_case("swapping twice gives the original",
                 ok, "to_network_order(to_network_order(x, 1), 1), x in {0x12345678, 0xDEADBEEF, 0x00000001, 0x80000000}",
                 "all round-trip", g, "");
    }

    chk_mem("my_htonl(0x12345678) is big-endian in memory", 0x12345678u, "12 34 56 78");
    chk_sys(0xDEADBEEFu);
    chk_sys(0xA1B2C3D4u);
    chk_sys(0x00000001u);

    nlc_end();
    return 0;
}

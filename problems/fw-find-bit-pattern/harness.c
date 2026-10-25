#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

/* Each input is copied into a heap buffer of exactly `alloc` bytes. Normally
   alloc == shown, so any read past the end is out of bounds. A few cases
   allocate extra hidden bytes that would complete a match, so a solution that
   reads past numBytes returns the wrong answer instead of passing by luck. */
static void chk(const char *name, const uint8_t *bytes, uint32_t numBytes,
                uint32_t shown, uint32_t alloc, int expected) {
    char in[400], e[16], g[16];
    uint8_t *buf = NULL;

    if (bytes) {
        buf = malloc(alloc);
        memcpy(buf, bytes, alloc);
        size_t off = (size_t)snprintf(in, sizeof in, "numBytes=%u, data={", numBytes);
        for (uint32_t i = 0; i < shown && off < sizeof in; i++)
            off += (size_t)snprintf(in + off, sizeof in - off, "%s%02X", i ? " " : "", bytes[i]);
        if (off < sizeof in) snprintf(in + off, sizeof in - off, "}");
    } else {
        snprintf(in, sizeof in, "numBytes=%u, data=NULL", numBytes);
    }

    int got = findPattern(numBytes, buf);
    snprintf(e, sizeof e, "%d", expected);
    snprintf(g, sizeof g, "%d", got);
    nlc_case(name, got == expected, in, e, g, "");
    free(buf);
}

#define CHK(name, arr, expected) \
    chk(name, arr, (uint32_t)sizeof(arr), (uint32_t)sizeof(arr), (uint32_t)sizeof(arr), expected)

int main(void) {
    nlc_begin();

    static const uint8_t ex1[] = {0xFE, 0x6B, 0x28, 0x40};
    CHK("example 1: match at bit 0", ex1, 0);

    static const uint8_t ex2[] = {0x00, 0x00, 0xFE, 0x6B, 0x28, 0x40};
    CHK("example 2: byte-aligned match at bit 16", ex2, 16);

    static const uint8_t ex3[] = {0x00, 0x01, 0xFC, 0xD6, 0x50, 0x80};
    CHK("example 3: unaligned match at bit 15", ex3, 15);

    static const uint8_t ex4[] = {0xFE, 0x6B, 0x28, 0x41};
    CHK("example 4: last bit differs, no match", ex4, -1);

    /* Shown as 3 bytes; a 4th hidden byte would complete the pattern. */
    static const uint8_t short3[] = {0xFE, 0x6B, 0x28, 0x40};
    chk("example 5: numBytes < 4", short3, 3, 3, 4, -2);

    chk("NULL data", NULL, 8, 0, 0, -2);

    static const uint8_t zero[] = {0xFE, 0x6B, 0x28, 0x40};
    chk("numBytes = 0", zero, 0, 0, 4, -2);

    static const uint8_t lsb[] = {0x7F, 0xD6, 0x14, 0x02};
    CHK("pattern with bits reversed in each byte is not a match", lsb, -1);

    static const uint8_t le[] = {0x40, 0x28, 0x6B, 0xFE};
    CHK("pattern in little-endian byte order is not a match", le, -1);

    /* Shown as 6 bytes; a hidden trailing 0x00 would complete a match at bit 20. */
    static const uint8_t past[] = {0x12, 0x34, 0x0F, 0xE6, 0xB2, 0x84, 0x00};
    chk("pattern would run past the end", past, 6, 6, 7, -1);

    static const uint8_t off20[] = {0x12, 0x34, 0x0F, 0xE6, 0xB2, 0x84, 0x00};
    CHK("unaligned match at bit 20", off20, 20);

    static const uint8_t overlap[] = {0xFE, 0x6B, 0x28, 0xFE, 0x6B, 0x28, 0x40};
    CHK("partial match followed by real match", overlap, 24);

    static const uint8_t last[] = {0xA5, 0x5A, 0x3C, 0xC3, 0xFE, 0x6B, 0x28, 0x40};
    CHK("match at the last possible offset", last, 32);

    static const uint8_t lastu[] = {0x31, 0x41, 0x59, 0x27, 0xFC, 0xD6, 0x50, 0x81};
    CHK("unaligned match ending in the final byte", lastu, 31);

    static const uint8_t two[] = {0x1F, 0xCD, 0x65, 0x08, 0x0B, 0x5F, 0xF3, 0x59, 0x42, 0x06};
    CHK("two matches: return the first", two, 3);

    static const uint8_t longbuf[] = {
        0xDC, 0x04, 0x65, 0xAA, 0x1F, 0xAD, 0x1D, 0x5A, 0xDA, 0xE5, 0xAC, 0x1B,
        0x1E, 0x5F, 0x13, 0x70, 0x79, 0x6C, 0xFD, 0x10, 0xFF, 0x19, 0xAF, 0x60,
        0x1D, 0x04, 0xAC, 0xB4, 0x1D, 0x02, 0x2B, 0x46, 0x78, 0x73, 0x3A, 0xF2,
        0xDF, 0xF3, 0x59, 0x42, 0x00, 0x59, 0xD1, 0xEE, 0x39, 0x10, 0xCB, 0x48,
    };
    CHK("48-byte buffer, unaligned match at bit 293", longbuf, 293);

    nlc_end();
    return 0;
}

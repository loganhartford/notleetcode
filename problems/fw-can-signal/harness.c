#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

/* Expected values come from the literals in main() and, for the sweeps, from
 * the slow bit-by-bit model below. Nothing here calls the reference solution. */

#define H_GUARD 0xA5
#define H_PAIRS 1552u /* (start_bit, len) pairs with 1 <= len <= 32, start_bit + len <= 64 */

static unsigned h_bit(const uint8_t f[8], unsigned k) {
    return (f[k / 8] >> (k % 8)) & 1u;
}

static uint32_t h_get(const uint8_t f[8], unsigned start, unsigned len) {
    uint32_t v = 0;
    for (unsigned i = 0; i < len; i++)
        v |= (uint32_t)h_bit(f, start + i) << i;
    return v;
}

static int32_t h_get_signed(const uint8_t f[8], unsigned start, unsigned len) {
    int64_t v = h_get(f, start, len);
    if (h_bit(f, start + len - 1))
        v -= (int64_t)1 << len;
    return (int32_t)v;
}

static void h_set(uint8_t f[8], unsigned start, unsigned len, uint32_t value) {
    for (unsigned i = 0; i < len; i++) {
        unsigned k = start + i;
        uint8_t m = (uint8_t)(1u << (k % 8));
        if ((value >> i) & 1u) f[k / 8] |= m;
        else f[k / 8] &= (uint8_t)~m;
    }
}

static uint32_t h_low_bits(uint32_t value, unsigned len) {
    uint32_t v = 0;
    for (unsigned i = 0; i < len; i++)
        v |= value & ((uint32_t)1 << i);
    return v;
}

static uint32_t h_next(uint32_t *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s;
}

static void h_fill(uint8_t f[8], uint32_t *s) {
    for (int i = 0; i < 8; i++) f[i] = (uint8_t)(h_next(s) >> 24);
}

static void h_fmt_frame(char *out, size_t cap, const uint8_t f[8]) {
    snprintf(out, cap, "{%02X %02X %02X %02X %02X %02X %02X %02X}",
             f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7]);
}

/* Formats the 8 frame bytes of buf (buf[1..8]) and flags writes to the guard
 * bytes buf[0] and buf[9]. Returns 1 if a guard byte was overwritten. */
static int h_fmt_result(char *out, size_t cap, const uint8_t buf[10]) {
    char fs[40];
    h_fmt_frame(fs, sizeof fs, buf + 1);
    size_t used = (size_t)snprintf(out, cap, "frame=%s", fs);
    if (buf[0] != H_GUARD && used < cap) {
        used += (size_t)snprintf(out + used, cap - used,
                                 "; out-of-bounds write: frame[-1]=0x%02X (expected 0xA5)", buf[0]);
    }
    if (buf[9] != H_GUARD && used < cap) {
        snprintf(out + used, cap - used,
                 "; out-of-bounds write: frame[8]=0x%02X (expected 0xA5)", buf[9]);
    }
    return buf[0] != H_GUARD || buf[9] != H_GUARD;
}

static void h_check_get(const char *name, const uint8_t f[8], unsigned start, unsigned len,
                        uint32_t expected) {
    uint8_t buf[10];
    buf[0] = buf[9] = H_GUARD;
    memcpy(buf + 1, f, 8);
    uint32_t got = can_get_signal(buf + 1, start, len);
    char fs[40], in[160], e[32], g[32];
    h_fmt_frame(fs, sizeof fs, f);
    snprintf(in, sizeof in, "can_get_signal(frame=%s, start_bit=%u, len=%u)", fs, start, len);
    snprintf(e, sizeof e, "0x%lX", (unsigned long)expected);
    snprintf(g, sizeof g, "0x%lX", (unsigned long)got);
    nlc_case(name, got == expected, in, e, g, "");
}

static void h_check_signed(const char *name, const uint8_t f[8], unsigned start, unsigned len,
                           int32_t expected) {
    uint8_t buf[10];
    buf[0] = buf[9] = H_GUARD;
    memcpy(buf + 1, f, 8);
    int32_t got = can_get_signed(buf + 1, start, len);
    char fs[40], in[160], e[32], g[32];
    h_fmt_frame(fs, sizeof fs, f);
    snprintf(in, sizeof in, "can_get_signed(frame=%s, start_bit=%u, len=%u)", fs, start, len);
    snprintf(e, sizeof e, "%ld", (long)expected);
    snprintf(g, sizeof g, "%ld", (long)got);
    nlc_case(name, got == expected, in, e, g, "");
}

static void h_check_set(const char *name, const uint8_t before[8], unsigned start, unsigned len,
                        uint32_t value, const uint8_t expected[8]) {
    uint8_t buf[10];
    buf[0] = buf[9] = H_GUARD;
    memcpy(buf + 1, before, 8);
    can_set_signal(buf + 1, start, len, value);
    char fs[40], in[200], e[64], g[200];
    h_fmt_frame(fs, sizeof fs, before);
    snprintf(in, sizeof in, "can_set_signal(frame=%s, start_bit=%u, len=%u, value=0x%lX)",
             fs, start, len, (unsigned long)value);
    h_fmt_frame(fs, sizeof fs, expected);
    snprintf(e, sizeof e, "frame=%s", fs);
    int oob = h_fmt_result(g, sizeof g, buf);
    nlc_case(name, !oob && memcmp(buf + 1, expected, 8) == 0, in, e, g, "");
}

static void h_sweep_get(const char *name, uint32_t seed) {
    uint8_t f[8];
    h_fill(f, &seed);
    char fs[40], in[256], e[64], g[64];
    h_fmt_frame(fs, sizeof fs, f);
    snprintf(in, sizeof in, "can_get_signal(frame=%s, start_bit, len) for all %u pairs", fs, H_PAIRS);
    snprintf(e, sizeof e, "all %u pairs match", H_PAIRS);
    snprintf(g, sizeof g, "all %u pairs match", H_PAIRS);
    for (unsigned len = 1; len <= 32; len++) {
        for (unsigned start = 0; start + len <= 64; start++) {
            uint32_t want = h_get(f, start, len);
            uint32_t got = can_get_signal(f, start, len);
            if (got != want) {
                snprintf(in, sizeof in, "can_get_signal(frame=%s, start_bit=%u, len=%u) "
                         "(first failing pair of %u)", fs, start, len, H_PAIRS);
                snprintf(e, sizeof e, "0x%lX", (unsigned long)want);
                snprintf(g, sizeof g, "0x%lX", (unsigned long)got);
                nlc_case(name, 0, in, e, g, "");
                return;
            }
        }
    }
    nlc_case(name, 1, in, e, g, "");
}

static void h_sweep_signed(const char *name, uint32_t seed) {
    uint8_t f[8];
    h_fill(f, &seed);
    char fs[40], in[256], e[64], g[64];
    h_fmt_frame(fs, sizeof fs, f);
    snprintf(in, sizeof in, "can_get_signed(frame=%s, start_bit, len) for all %u pairs", fs, H_PAIRS);
    snprintf(e, sizeof e, "all %u pairs match", H_PAIRS);
    snprintf(g, sizeof g, "all %u pairs match", H_PAIRS);
    for (unsigned len = 1; len <= 32; len++) {
        for (unsigned start = 0; start + len <= 64; start++) {
            int32_t want = h_get_signed(f, start, len);
            int32_t got = can_get_signed(f, start, len);
            if (got != want) {
                snprintf(in, sizeof in, "can_get_signed(frame=%s, start_bit=%u, len=%u) "
                         "(first failing pair of %u)", fs, start, len, H_PAIRS);
                snprintf(e, sizeof e, "%ld", (long)want);
                snprintf(g, sizeof g, "%ld", (long)got);
                nlc_case(name, 0, in, e, g, "");
                return;
            }
        }
    }
    nlc_case(name, 1, in, e, g, "");
}

static void h_sweep_set(const char *name, uint32_t seed) {
    char in[256], e[64], g[200];
    for (unsigned len = 1; len <= 32; len++) {
        for (unsigned start = 0; start + len <= 64; start++) {
            uint8_t before[8], want[8], buf[10];
            h_fill(before, &seed);
            uint32_t value = h_next(&seed);
            memcpy(want, before, 8);
            h_set(want, start, len, value);
            buf[0] = buf[9] = H_GUARD;
            memcpy(buf + 1, before, 8);
            can_set_signal(buf + 1, start, len, value);
            int oob = h_fmt_result(g, sizeof g, buf);
            if (oob || memcmp(buf + 1, want, 8) != 0) {
                char fs[40];
                h_fmt_frame(fs, sizeof fs, before);
                snprintf(in, sizeof in, "can_set_signal(frame=%s, start_bit=%u, len=%u, value=0x%lX) "
                         "(first failing pair of %u)", fs, start, len, (unsigned long)value, H_PAIRS);
                h_fmt_frame(fs, sizeof fs, want);
                snprintf(e, sizeof e, "frame=%s", fs);
                nlc_case(name, 0, in, e, g, "");
                return;
            }
        }
    }
    snprintf(in, sizeof in, "can_set_signal on a random frame with a random 32-bit value, "
             "for all %u (start_bit, len) pairs", H_PAIRS);
    snprintf(e, sizeof e, "all %u pairs match", H_PAIRS);
    nlc_case(name, 1, in, e, e, "");
}

static void h_round_trip(const char *name, uint32_t seed) {
    char in[320], e[64], g[64];
    for (unsigned len = 1; len <= 32; len++) {
        for (unsigned start = 0; start + len <= 64; start++) {
            uint8_t before[8], f[8];
            h_fill(before, &seed);
            uint32_t value = h_next(&seed);
            uint32_t want = h_low_bits(value, len);
            memcpy(f, before, 8);
            can_set_signal(f, start, len, value);
            uint32_t got = can_get_signal(f, start, len);
            if (got != want) {
                char fs[40];
                h_fmt_frame(fs, sizeof fs, before);
                snprintf(in, sizeof in, "can_set_signal(frame=%s, start_bit=%u, len=%u, value=0x%lX), "
                         "then can_get_signal(frame, %u, %u) (first failing pair of %u)",
                         fs, start, len, (unsigned long)value, start, len, H_PAIRS);
                snprintf(e, sizeof e, "0x%lX", (unsigned long)want);
                snprintf(g, sizeof g, "0x%lX", (unsigned long)got);
                nlc_case(name, 0, in, e, g, "");
                return;
            }
        }
    }
    snprintf(in, sizeof in, "can_set_signal then can_get_signal on a random frame with a random "
             "32-bit value, for all %u (start_bit, len) pairs", H_PAIRS);
    snprintf(e, sizeof e, "all %u pairs match", H_PAIRS);
    nlc_case(name, 1, in, e, e, "");
}

int main(void) {
    nlc_begin();

    static const uint8_t seq[8]  = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    static const uint8_t rev[8]  = {0x21, 0x43, 0x65, 0x87, 0xA9, 0xCB, 0xED, 0x0F};
    static const uint8_t ab[8]   = {0xAB, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t span[8] = {0xC5, 0x3A, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t top[8]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
    static const uint8_t ones[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const uint8_t zero[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    h_check_get("example 1: byte-aligned 8-bit signal", seq, 16, 8, 0x56u);
    h_check_get("byte-aligned 16-bit signal, little-endian", seq, 8, 16, 0x5634u);
    h_check_get("example 2: 12-bit signal at bit 4", ab, 4, 12, 0xCDAu);
    h_check_get("signal spanning three bytes (start 6, len 13)", span, 6, 13, 0x18EBu);
    h_check_get("full 32-bit signal at bit 0", seq, 0, 32, 0x78563412u);
    h_check_get("full 32-bit signal at bit 17 (spans five bytes)", rev, 17, 32, 0xE5D4C3B2u);
    h_check_get("full 32-bit signal in the top half (bit 32)", seq, 32, 32, 0xF0DEBC9Au);
    h_check_get("1-bit signal at bit 63", top, 63, 1, 1u);

    static const uint8_t s_min12[8] = {0x0F, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t s_max12[8] = {0xFF, 0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t s_neg1[8]  = {0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00};
    static const uint8_t s_min32[8] = {0xFF, 0x1F, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF};

    h_check_signed("example 3: signed 0x800 in 12 bits is -2048", s_min12, 4, 12, -2048);
    h_check_signed("signed 0x7FF in 12 bits is 2047", s_max12, 4, 12, 2047);
    h_check_signed("signed all-ones 5-bit signal is -1", s_neg1, 30, 5, -1);
    h_check_signed("signed 32-bit signal at bit 13 is INT32_MIN", s_min32, 13, 32, INT32_MIN);

    static const uint8_t x_ex4[8]   = {0x3F, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const uint8_t x_span1[8] = {0x3F, 0x8D, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const uint8_t x_span0[8] = {0x00, 0x8D, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t x_trunc[8] = {0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const uint8_t x_w32[8]   = {0xFF, 0xFF, 0xBF, 0x37, 0xAF, 0x26, 0xFE, 0xFF};
    static const uint8_t x_c32[8]   = {0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xDE, 0xF0};
    static const uint8_t x_top[8]   = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};

    h_check_set("example 4: set 12 bits at bit 4 in an all-0xFF frame", ones, 4, 12, 0x123u, x_ex4);
    h_check_set("set across three bytes, neighbours stay 1 (0xFF frame)", ones, 6, 13, 0x1234u, x_span1);
    h_check_set("set across three bytes, neighbours stay 0 (zero frame)", zero, 6, 13, 0x1234u, x_span0);
    h_check_set("example 5: value wider than len is truncated", zero, 10, 4, 0xFFFFFFF5u, x_trunc);
    h_check_set("set full 32-bit signal at bit 17 (0xFF frame)", ones, 17, 32, 0x13579BDFu, x_w32);
    h_check_set("clear full 32-bit signal at bit 17", seq, 17, 32, 0u, x_c32);
    h_check_set("clear 1-bit signal at bit 63", ones, 63, 1, 0u, x_top);

    h_sweep_get("get: every start_bit/len pair", 0x2545F491u);
    h_sweep_signed("get signed: every start_bit/len pair", 0x9E3779B9u);
    h_sweep_set("set: every start_bit/len pair, random frame and value", 0xC0FFEE11u);
    h_round_trip("set then get round trip: every start_bit/len pair", 0x1234ABCDu);

    nlc_end();
    return 0;
}

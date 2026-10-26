#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

enum { H_ADD, H_SUB, H_MUL };

static const char *const h_names[] = { "q15_add_sat", "q15_sub_sat", "q15_mul" };

static const int32_t h_edges[] = {
    -32768, -32767, -32766, -16385, -16384, -16383, -8192, -3, -2, -1,
    0, 1, 2, 3, 8192, 16383, 16384, 16385, 32766, 32767,
};
#define H_NEDGES ((int)(sizeof h_edges / sizeof h_edges[0]))
#define H_NRANDOM 4000
#define H_NMIXED 1000
#define H_NPAIRS (H_NEDGES * H_NEDGES + H_NRANDOM + 2 * H_NMIXED)

static int32_t h_pa[H_NPAIRS], h_pb[H_NPAIRS];
static uint32_t h_rng = 0x2545F491u;

static int32_t h_call(int op, int32_t a, int32_t b) {
    switch (op) {
    case H_ADD: return q15_add_sat((int16_t)a, (int16_t)b);
    case H_SUB: return q15_sub_sat((int16_t)a, (int16_t)b);
    default:    return q15_mul((int16_t)a, (int16_t)b);
    }
}

/* Slow reference, straight from the definition: exact int64 math, floor
 * division done explicitly (C '/' truncates toward zero), then clamp. */
static int64_t h_floor_div(int64_t n, int64_t d) {
    int64_t q = n / d;
    if (n % d != 0 && n < 0) q -= 1;
    return q;
}

static int32_t h_clamp(int64_t v) {
    if (v > 32767) return 32767;
    if (v < -32768) return -32768;
    return (int32_t)v;
}

static int32_t h_expect(int op, int32_t a, int32_t b) {
    switch (op) {
    case H_ADD: return h_clamp((int64_t)a + b);
    case H_SUB: return h_clamp((int64_t)a - b);
    default:    return h_clamp(h_floor_div(2 * (int64_t)a * b + 32768, 65536));
    }
}

static void h_fmt(char *buf, size_t cap, int32_t v) {
    snprintf(buf, cap, "%d (0x%04X, %.6g)", (int)v, (unsigned)((uint32_t)v & 0xFFFFu), v / 32768.0);
}

static void h_case(const char *name, int op, int32_t a, int32_t b, int32_t expected) {
    int32_t got = h_call(op, a, b);
    char in[128], e[64], g[64];
    snprintf(in, sizeof in, "%s(%d, %d)  [a = %.6g, b = %.6g]",
             h_names[op], (int)a, (int)b, a / 32768.0, b / 32768.0);
    h_fmt(e, sizeof e, expected);
    h_fmt(g, sizeof g, got);
    nlc_case(name, got == expected, in, e, g, "");
}

static uint32_t h_next(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

static int32_t h_rand_q15(void) {
    return (int32_t)(h_next() & 0xFFFFu) - 32768;
}

static void h_build_pairs(void) {
    int n = 0;
    for (int i = 0; i < H_NEDGES; i++) {
        for (int j = 0; j < H_NEDGES; j++) {
            h_pa[n] = h_edges[i];
            h_pb[n] = h_edges[j];
            n++;
        }
    }
    for (int k = 0; k < H_NRANDOM; k++) {
        h_pa[n] = h_rand_q15();
        h_pb[n] = h_rand_q15();
        n++;
    }
    for (int k = 0; k < H_NMIXED; k++) {
        h_pa[n] = h_rand_q15();
        h_pb[n] = h_edges[k % H_NEDGES];
        n++;
        h_pa[n] = h_edges[k % H_NEDGES];
        h_pb[n] = h_rand_q15();
        n++;
    }
}

static void h_sweep(int op) {
    int wrong = 0;
    int32_t fa = 0, fb = 0, fexp = 0, fgot = 0;
    for (int i = 0; i < H_NPAIRS; i++) {
        int32_t got = h_call(op, h_pa[i], h_pb[i]);
        int32_t exp = h_expect(op, h_pa[i], h_pb[i]);
        if (got != exp) {
            if (!wrong) { fa = h_pa[i]; fb = h_pb[i]; fexp = exp; fgot = got; }
            wrong++;
        }
    }
    char name[64], in[160], e[64], g[256];
    snprintf(name, sizeof name, "%s sweep", h_names[op]);
    snprintf(in, sizeof in, "%s(a, b) over %d pairs (every pair of %d edge values + %d pseudo-random)",
             h_names[op], H_NPAIRS, H_NEDGES, H_NRANDOM + 2 * H_NMIXED);
    snprintf(e, sizeof e, "all %d correct", H_NPAIRS);
    if (!wrong) {
        snprintf(g, sizeof g, "all %d correct", H_NPAIRS);
    } else {
        char fe[64], fg[64];
        h_fmt(fe, sizeof fe, fexp);
        h_fmt(fg, sizeof fg, fgot);
        snprintf(g, sizeof g, "%s(%d, %d) returned %s, expected %s; %d of %d wrong",
                 h_names[op], (int)fa, (int)fb, fg, fe, wrong, H_NPAIRS);
    }
    nlc_case(name, !wrong, in, e, g, "");
}

int main(void) {
    nlc_begin();

    h_case("add 0.5 + 0.25",            H_ADD,  16384,   8192,  24576);
    h_case("add positive overflow",     H_ADD,  28672,   8192,  32767);
    h_case("add negative overflow",     H_ADD, -28672,  -8192, -32768);
    h_case("add max + 1 LSB",           H_ADD,  32767,      1,  32767);
    h_case("add min + (-1 LSB)",        H_ADD, -32768,     -1, -32768);
    h_case("add min + min",             H_ADD, -32768, -32768, -32768);
    h_case("add max + min",             H_ADD,  32767, -32768,     -1);

    h_case("sub 0.25 - 0.75",           H_SUB,   8192,  24576, -16384);
    h_case("sub max - (-1 LSB)",        H_SUB,  32767,     -1,  32767);
    h_case("sub min - 1 LSB",           H_SUB, -32768,      1, -32768);
    h_case("sub 0 - (-1.0)",            H_SUB,      0, -32768,  32767);
    h_case("sub min - min",             H_SUB, -32768, -32768,      0);

    h_case("mul 0.5 * 0.5",             H_MUL,  16384,  16384,   8192);
    h_case("mul -1.0 * 0.5",            H_MUL, -32768,  16384, -16384);
    h_case("mul -1.0 * max",            H_MUL, -32768,  32767, -32767);
    h_case("mul max * max",             H_MUL,  32767,  32767,  32766);
    h_case("mul -1.0 * -1.0",           H_MUL, -32768, -32768,  32767);
    h_case("mul -1.0 * -1 LSB",         H_MUL, -32768,     -1,      1);
    h_case("mul positive tie",          H_MUL,      1,  16384,      1);
    h_case("mul negative tie",          H_MUL,     -1,  16384,      0);
    h_case("mul negative tie at -1.5",  H_MUL,     -3,  16384,     -1);
    h_case("mul 0.5 * max (tie)",       H_MUL,  16384,  32767,  16384);
    h_case("mul -0.5 * max (tie)",      H_MUL, -16384,  32767, -16383);
    h_case("mul rounds 1.75 up",        H_MUL,      7,   8192,      2);
    h_case("mul rounds -1.75 down",     H_MUL,     -7,   8192,     -2);

    h_build_pairs();
    h_sweep(H_ADD);
    h_sweep(H_SUB);
    h_sweep(H_MUL);

    nlc_end();
    return 0;
}

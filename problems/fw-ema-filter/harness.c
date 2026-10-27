#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

/* Every check below comes from the contract, not from a reference solution:
 *   accuracy:     |output_n - y_n| < 1, where y_n is the ideal real-valued EMA
 *                 (tracked here in double; its error is far below 2^-8);
 *   steady state: once the same x has been fed 16 * 2^k times in a row, the
 *                 output is exactly x on that update and every later one. */

#define H_NO_RUN INT32_MIN
#define H_SWEEP_LEN 2400
#define H_CONV_TRIALS 24

typedef struct {
    ema_t f;
    char tag[48];
    int k;
    double ideal;
    long n;
    int32_t run_x;
    long run_len;
} h_trk_t;

static uint32_t h_rng = 0x6C8E9CF5u;
static int h_bad;
static char h_why[400];
static long h_updates;

static const int32_t h_rails[] = { -32768, 32767, -32767, 32766, 0, -1, 1 };
#define H_NRAILS ((int)(sizeof h_rails / sizeof h_rails[0]))

enum { H_NOISE, H_WALK, H_STEPS, H_LEVEL, H_RAIL, H_NSIG };
static const char *const h_sig_names[] = {
    "white noise", "random walk", "random steps", "noisy level", "rail-to-rail",
};

static uint32_t h_next(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

static int32_t h_rand_i16(void) {
    return (int32_t)(h_next() & 0xFFFFu) - 32768;
}

static int32_t h_rand_in(int32_t lo, int32_t hi) {
    return lo + (int32_t)(h_next() % (uint32_t)(hi - lo + 1));
}

static int32_t h_rand_rail(void) {
    return h_rails[h_next() % H_NRAILS];
}

static int32_t h_clamp16(int32_t v) {
    if (v > 32767) return 32767;
    if (v < -32768) return -32768;
    return v;
}

static long h_bound(int k) {
    return 16L << k;
}

static void h_fail(const char *fmt, ...) {
    if (h_bad) return;
    h_bad = 1;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(h_why, sizeof h_why, fmt, ap);
    va_end(ap);
}

static void h_begin(void) {
    h_bad = 0;
    h_why[0] = '\0';
    h_updates = 0;
}

static void h_end(const char *name, const char *in, const char *exp) {
    nlc_case(name, !h_bad, in, exp, h_bad ? h_why : exp, "");
}

static void h_allowed(char *buf, size_t cap, double y) {
    long lo = (long)y;
    if ((double)lo > y) lo--;
    if ((double)lo == y) snprintf(buf, cap, "exactly %ld", lo);
    else snprintf(buf, cap, "%ld or %ld", lo, lo + 1);
}

static void h_init(h_trk_t *t, int k, int32_t initial) {
    ema_init(&t->f, (uint8_t)k, (int16_t)initial);
    t->k = k;
    t->ideal = initial;
    t->n = 0;
    t->run_x = H_NO_RUN;
    t->run_len = 0;
}

static int32_t h_feed(h_trk_t *t, int32_t x) {
    int32_t got = ema_update(&t->f, (int16_t)x);
    t->n++;
    h_updates++;
    t->ideal += (x - t->ideal) / (double)(1L << t->k);
    if (x == t->run_x) {
        t->run_len++;
    } else {
        t->run_x = x;
        t->run_len = 1;
    }
    double err = got - t->ideal;
    if (err < 0) err = -err;
    if (!(err < 1.0)) {
        char want[48];
        h_allowed(want, sizeof want, t->ideal);
        h_fail("%supdate #%ld (k = %d, x = %d) returned %d, but the ideal y_n is %.4f, so it must be %s",
               t->tag, t->n, t->k, (int)x, (int)got, t->ideal, want);
    } else if (t->run_len >= h_bound(t->k) && got != x) {
        h_fail("%supdate #%ld (k = %d) returned %d, but x = %d has now been fed %ld times in a row "
               "(limit 16 * 2^k = %ld), so the output must be exactly %d (ideal y_n = %.4f)",
               t->tag, t->n, t->k, (int)got, (int)x, t->run_len, h_bound(t->k), (int)x, t->ideal);
    }
    return got;
}

static void h_hold(h_trk_t *t, int32_t x, long count) {
    for (long i = 0; i < count; i++) h_feed(t, x);
}

static void h_step_case(const char *name, int k, int32_t initial, int32_t x) {
    h_trk_t t;
    h_begin();
    t.tag[0] = '\0';
    h_init(&t, k, initial);
    long need = h_bound(k);
    h_hold(&t, x, 2 * need);

    char in[128], exp[160];
    snprintf(in, sizeof in, "ema_init(&f, %d, %d), then ema_update(&f, %d) x %ld",
             k, (int)initial, (int)x, 2 * need);
    if (initial == x)
        snprintf(exp, sizeof exp, "exactly %d on every update (the ideal y_n is exactly %d)",
                 (int)x, (int)x);
    else
        snprintf(exp, sizeof exp, "every output within 1 of the ideal y_n, and exactly %d from update %ld on",
                 (int)x, need);
    h_end(name, in, exp);
}

static void h_rails_case(void) {
    static const int ks[] = { 1, 8 };
    h_trk_t t[4];
    h_begin();
    for (int i = 0; i < 4; i++) {
        snprintf(t[i].tag, sizeof t[i].tag, "filter %c: ", 'a' + i);
        h_init(&t[i], ks[i / 2], (i % 2) ? 32767 : -32768);
    }
    for (int i = 0; i < 4; i++) h_hold(&t[i], (i % 2) ? 32767 : -32768, 2 * h_bound(ks[i / 2]));
    h_end("initial == input at the rails, k = 1 and 8",
          "four filters: (k=1, -32768), (k=1, 32767), (k=8, -32768), (k=8, 32767); "
          "each fed its own initial value 32 * 2^k times",
          "each output equals its initial value on every update");
}

static void h_square_case(void) {
    h_trk_t t;
    h_begin();
    t.tag[0] = '\0';
    h_init(&t, 1, 0);
    for (int i = 0; i < 2000; i++) h_feed(&t, (i % 2) ? -32768 : 32767);
    h_hold(&t, 0, 2 * h_bound(1));
    h_end("full-scale square wave, k = 1",
          "ema_init(&f, 1, 0), then x = 32767, -32768, 32767, ... for 2000 updates, then x = 0 x 64",
          "every output within 1 of the ideal y_n, and exactly 0 from the 32nd update of the 0s on");
}

static void h_interleave_case(void) {
    h_trk_t a, b;
    h_begin();
    snprintf(a.tag, sizeof a.tag, "filter a: ");
    snprintf(b.tag, sizeof b.tag, "filter b: ");
    h_init(&a, 3, 0);
    h_init(&b, 6, -2000);
    long len = 2 * h_bound(6);
    for (long i = 0; i < len; i++) {
        h_feed(&a, 500);
        h_feed(&b, -7000);
    }
    for (long i = 0; i < len; i++) {
        h_feed(&b, 32767);
        h_feed(&a, -32768);
    }
    h_end("two filters interleaved",
          "a: ema_init(&a, 3, 0), b: ema_init(&b, 6, -2000); alternate updates: a gets 500 and b gets "
          "-7000 (2048 each), then b gets 32767 and a gets -32768 (2048 each)",
          "each filter tracks its own ideal y_n and settles exactly: a at 500 then -32768, b at -7000 then 32767");
}

static void h_reinit_case(void) {
    h_trk_t t;
    h_begin();
    t.tag[0] = '\0';
    h_init(&t, 2, 0);
    h_hold(&t, 30000, 2 * h_bound(2));
    snprintf(t.tag, sizeof t.tag, "after re-init: ");
    h_init(&t, 7, -100);
    h_hold(&t, -100, 2 * h_bound(7));
    h_hold(&t, 50, 2 * h_bound(7));
    h_end("re-init a used filter",
          "ema_init(&f, 2, 0), ema_update(&f, 30000) x 128, then ema_init(&f, 7, -100), "
          "ema_update(&f, -100) x 4096, ema_update(&f, 50) x 4096",
          "after re-init: exactly -100 on every update of -100, then exactly 50 from update 2048 of the 50s on");
}

static void h_garbage_case(void) {
    h_trk_t a, b;
    h_begin();
    memset(&a.f, 0xA5, sizeof a.f);
    memset(&b.f, 0xFF, sizeof b.f);
    snprintf(a.tag, sizeof a.tag, "filter a: ");
    snprintf(b.tag, sizeof b.tag, "filter b: ");
    h_init(&a, 5, 1234);
    h_init(&b, 1, -32768);
    h_hold(&a, 1234, 2 * h_bound(5));
    h_hold(&a, -1234, 2 * h_bound(5));
    h_hold(&b, -32768, 2 * h_bound(1));
    h_hold(&b, 32767, 2 * h_bound(1));
    h_end("garbage struct before init",
          "a: bytes 0xA5, ema_init(&a, 5, 1234), 1234 x 1024, -1234 x 1024; "
          "b: bytes 0xFF, ema_init(&b, 1, -32768), -32768 x 64, 32767 x 64",
          "each output equals its initial value until the input changes, then settles exactly on the new input");
}

typedef struct {
    int type;
    int32_t level;
    long left;
} h_gen_t;

static int32_t h_gen_next(h_gen_t *g, int k) {
    switch (g->type) {
    case H_NOISE:
        return h_rand_i16();
    case H_WALK:
        g->level = h_clamp16(g->level + h_rand_in(-64, 64));
        return g->level;
    case H_STEPS:
        if (g->left <= 0) {
            g->level = h_rand_i16();
            g->left = h_rand_in(1, 3 << k);
        }
        g->left--;
        return g->level;
    case H_LEVEL:
        if (g->left <= 0) {
            g->level = h_rand_i16();
            g->left = h_rand_in(100, 600);
        }
        g->left--;
        return h_clamp16(g->level + h_rand_in(-3, 3));
    default:
        if (g->left <= 0) {
            g->level = h_rand_rail();
            g->left = h_rand_in(1, 4 << k);
        }
        g->left--;
        return g->level;
    }
}

static void h_tolerance_sweep(void) {
    h_begin();
    for (int k = 1; k <= 8; k++) {
        for (int s = 0; s < H_NSIG; s++) {
            h_trk_t t;
            h_gen_t g;
            snprintf(t.tag, sizeof t.tag, "%s signal: ", h_sig_names[s]);
            int32_t initial = h_rand_i16();
            h_init(&t, k, initial);
            g.type = s;
            g.level = initial;
            g.left = 0;
            for (int i = 0; i < H_SWEEP_LEN; i++) h_feed(&t, h_gen_next(&g, k));
        }
    }
    char in[200], exp[96];
    snprintf(in, sizeof in, "for every k = 1..8, five %d-sample signals (white noise, random walk, random steps, "
             "noisy level, rail-to-rail) into a freshly initialized filter", H_SWEEP_LEN);
    snprintf(exp, sizeof exp, "all %ld outputs within 1 of the ideal y_n", h_updates);
    h_end("tolerance sweep, k = 1..8", in, exp);
}

static void h_convergence_sweep(void) {
    h_begin();
    long holds = 0;
    for (int k = 1; k <= 8; k++) {
        for (int trial = 0; trial < H_CONV_TRIALS; trial++) {
            h_trk_t t;
            snprintf(t.tag, sizeof t.tag, "trial %d: ", trial);
            int32_t initial = (trial % 4 == 0) ? h_rand_rail() : h_rand_i16();
            h_init(&t, k, initial);
            long hist = h_rand_in(0, 3 << k);
            for (long i = 0; i < hist; i++) h_feed(&t, (trial % 2) ? h_rand_rail() : h_rand_i16());
            int32_t target;
            if (trial % 3 == 0) target = (initial < 0) ? 32767 : -32768;
            else target = h_rand_i16();
            h_hold(&t, target, h_bound(k) + (2L << k));
            holds++;
        }
    }
    char in[200], exp[96];
    snprintf(in, sizeof in, "for every k = 1..8, %d trials: ema_init at a random value, up to 3 * 2^k random "
             "samples, then one random target held for 18 * 2^k updates", H_CONV_TRIALS);
    snprintf(exp, sizeof exp, "all %ld targets reached exactly by update 16 * 2^k of the hold, and held", holds);
    h_end("convergence sweep, k = 1..8", in, exp);
}

int main(void) {
    nlc_begin();

    h_step_case("example 1: step 0 -> 1000, k = 4",           4,      0,   1000);
    h_step_case("example 2: falling step 100 -> -100, k = 2", 2,    100,   -100);
    h_step_case("example 3: initial == input, k = 8",         8,     -5,     -5);
    h_step_case("rising step of 1 LSB, k = 8",                8,      0,      1);
    h_step_case("rising step -300 -> 200, k = 6",             6,   -300,    200);
    h_step_case("falling step 1000 -> -1000, k = 5",          5,   1000,  -1000);
    h_step_case("negative falling step, k = 3",               3,   -100, -20000);
    h_step_case("negative rising step, k = 7",                7, -20000,   -100);
    h_step_case("full-scale rise to INT16_MAX, k = 8",        8, -32768,  32767);
    h_step_case("full-scale fall to INT16_MIN, k = 8",        8,  32767, -32768);
    h_step_case("full-scale rise to INT16_MAX, k = 1",        1, -32768,  32767);
    h_step_case("small falling step 7 -> -3, k = 1",          1,      7,     -3);
    h_rails_case();
    h_square_case();
    h_interleave_case();
    h_reinit_case();
    h_garbage_case();
    h_tolerance_sweep();
    h_convergence_sweep();

    nlc_end();
    return 0;
}

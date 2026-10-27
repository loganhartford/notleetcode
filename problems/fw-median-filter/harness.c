#define _XOPEN_SOURCE 700

#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "nlc.h"
#include "solution.c"

_Static_assert(MEDIAN_MAX == 15, "keep MEDIAN_MAX at 15: the tests are sized for it");

/* Every test runs under a 0.5 s timer. A call that never returns is cut off
 * by SIGALRM, reported as a failed test naming the call, and the remaining
 * tests still run. */
#define H_LIMIT_US 500000
#define H_MAXSEQ 48
#define H_SWEEP_LEN 2000

static sigjmp_buf h_jmp;
static char h_step[160];
static char h_got[1024];

static void h_on_alarm(int sig) {
    (void)sig;
    siglongjmp(h_jmp, 1);
}

static size_t h_fmt16(char *buf, size_t cap, const int16_t *a, int len) {
    size_t off = (size_t)snprintf(buf, cap, "[");
    for (int i = 0; i < len && off < cap; i++)
        off += (size_t)snprintf(buf + off, cap - off, "%s%d", i ? ", " : "", a[i]);
    if (off < cap) off += (size_t)snprintf(buf + off, cap - off, "]");
    return off < cap ? off : cap - 1;
}

/* Fill the struct with garbage that looks like a full window of large values,
 * then init it. */
static void h_fresh(median_t *m, const char *name, uint8_t n) {
    memset(m, 0xA5, sizeof *m);
    for (int i = 0; i < MEDIAN_MAX; i++)
        m->ring[i] = (int16_t)(i % 2 ? -31000 - 100 * i : 31000 + 100 * i);
    m->n = MEDIAN_MAX;
    m->head = 6;
    m->count = MEDIAN_MAX;
    snprintf(h_step, sizeof h_step, "median_init(%s, %u) on a garbage-filled struct", name, (unsigned)n);
    median_init(m, n);
}

static void h_reinit(median_t *m, const char *name, uint8_t n) {
    snprintf(h_step, sizeof h_step, "median_init(%s, %u) on a filter already in use", name, (unsigned)n);
    median_init(m, n);
}

static int16_t h_update(median_t *m, const char *name, int16_t sample, int idx) {
    snprintf(h_step, sizeof h_step, "median_update(%s, %d), sample %d of %s", name, sample, idx, name);
    return median_update(m, sample);
}

/* Appends the outputs to h_got at *off; on the first mismatch, also appends
 * which call was wrong. Returns true if every output matches. */
static bool h_report(size_t *off, const char *name, const char *ctx, const int16_t *in,
                     const int16_t *got, const int16_t *want, int len) {
    *off += h_fmt16(h_got + *off, sizeof h_got - *off, got, len);
    for (int i = 0; i < len; i++) {
        if (got[i] != want[i]) {
            *off += (size_t)snprintf(h_got + *off, sizeof h_got - *off,
                                     "  <- first wrong: sample %d%s, median_update(%s, %d) returned %d, expected %d",
                                     i + 1, ctx, name, in[i], got[i], want[i]);
            if (*off >= sizeof h_got) *off = sizeof h_got - 1;
            return false;
        }
    }
    return true;
}

static void h_run(const char *name, const char *input, const char *expected,
                  bool (*body)(const void *), const void *arg) {
    struct itimerval arm = {{0, 0}, {0, H_LIMIT_US}};
    struct itimerval disarm = {{0, 0}, {0, 0}};
    bool ok = false;
    snprintf(h_step, sizeof h_step, "(setup)");
    snprintf(h_got, sizeof h_got, "(no result)");
    if (sigsetjmp(h_jmp, 1) == 0) {
        setitimer(ITIMER_REAL, &arm, NULL);
        ok = body(arg);
        setitimer(ITIMER_REAL, &disarm, NULL);
    } else {
        ok = false;
        snprintf(h_got, sizeof h_got, "%s did not return within %d ms (infinite loop?)",
                 h_step, H_LIMIT_US / 1000);
    }
    nlc_case(name, ok, input, expected, h_got, "");
}

/* One filter, one sequence of samples, every output checked. */
typedef struct {
    uint8_t n;
    int len;
    const int16_t *in;
    const int16_t *want;
} h_seq;

static bool h_seq_body(const void *arg) {
    const h_seq *s = arg;
    median_t m;
    int16_t got[H_MAXSEQ];
    h_fresh(&m, "m", s->n);
    for (int i = 0; i < s->len; i++) got[i] = h_update(&m, "m", s->in[i], i + 1);
    size_t off = 0;
    return h_report(&off, "m", "", s->in, got, s->want, s->len);
}

static void h_seq_case(const char *name, const char *note, uint8_t n, const int16_t *in,
                       const int16_t *want, int len) {
    h_seq s = {n, len, in, want};
    char input[512], expected[512];
    int off = snprintf(input, sizeof input, "median_init(m, %u); median_update(m, x) for x in ", (unsigned)n);
    off += (int)h_fmt16(input + off, sizeof input - (size_t)off, in, len);
    if (note) snprintf(input + off, sizeof input - (size_t)off, "  (%s)", note);
    h_fmt16(expected, sizeof expected, want, len);
    h_run(name, input, expected, h_seq_body, &s);
}

#define H_LEN(a) ((int)(sizeof(a) / sizeof((a)[0])))
#define H_SEQ(name, note, n, in, want)                                              \
    do {                                                                            \
        _Static_assert(H_LEN(in) == H_LEN(want), "in/want length mismatch");        \
        _Static_assert(H_LEN(in) <= H_MAXSEQ, "sequence too long");                 \
        h_seq_case(name, note, n, in, want, H_LEN(in));                             \
    } while (0)

static const int16_t h_ex1_in[]   = {10, 12, 11, 500, 13, 12, 11};
static const int16_t h_ex1_want[] = {10, 10, 11, 11, 12, 12, 12};

static const int16_t h_ex2_in[]   = {40, 20, 30, 10};
static const int16_t h_ex2_want[] = {40, 20, 30, 20};

static const int16_t h_ex3_in[]   = {5, 7, 900, 6, 8, 4};
static const int16_t h_ex3_want[] = {5, 5, 7, 7, 8, 6};

static const int16_t h_spike_in[]   = {10, 10, 500, 10, 10, 10, 10, -500, 10, 10, 10, 500, 501, 10, 10, 10};
static const int16_t h_spike_want[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

static const int16_t h_n1_in[]   = {5, -3, 32767, -32768, 0, 0, 7, 7, -1};
static const int16_t h_n1_want[] = {5, -3, 32767, -32768, 0, 0, 7, 7, -1};

static const int16_t h_ramp_in[]   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static const int16_t h_ramp_want[] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1};

static const int16_t h_ext_in[] = {INT16_MIN, INT16_MAX, -1, 0, INT16_MIN, INT16_MAX, INT16_MAX,
                                   -5, INT16_MIN, INT16_MIN, 1, INT16_MIN, INT16_MAX};
static const int16_t h_ext_want[] = {INT16_MIN, INT16_MIN, -1, -1, -1, 0, 0,
                                     0, -5, -5, -5, INT16_MIN, INT16_MIN};

static const int16_t h_dup_in[]   = {-7, -7, -7, -7, -7, -7, -7, -7, -7, -7,
                                     3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
static const int16_t h_dup_want[] = {-7, -7, -7, -7, -7, -7, -7, -7, -7, -7,
                                     -7, -7, -7, 3, 3, 3, 3, 3, 3, 3};

static const int16_t h_slide_in[]   = {1000, 1, 2, 3, 4, 5, 6, 7, 8, -1000, 9, 10, 11, 12, 13, 14};
static const int16_t h_slide_want[] = {1000, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12};

static const int16_t h_n15_in[] = {
    -99, 47, -14, 132, 71, 10, 156, 95, 241, 180, 119, 265, 204, 350, 289, 228, 374, 313, 459, 398,
    -30000, 30000, 422, 361, 507, 446, 592, 32767, 470, 616, 555, 701, 640, 579, 725, 664, 810, 749, 688, 834,
};
static const int16_t h_n15_want[] = {
    -99, -99, -14, -14, 47, 10, 47, 47, 71, 71, 95, 95, 119, 119, 132, 156, 180, 204, 228, 241,
    241, 265, 289, 313, 350, 361, 374, 398, 422, 446, 459, 470, 507, 555, 579, 592, 592, 616, 640, 664,
};

/* Two filters fed alternately: a, b, a, b, ... */
static const int16_t h_a_in[]   = {1, 100, 2, 99, 3, 98, 4};
static const int16_t h_a_want[] = {1, 1, 2, 99, 3, 98, 4};
static const int16_t h_b_in[]   = {-50, 50, -40, 40, -30, 30, -20};
static const int16_t h_b_want[] = {-50, -50, -40, -40, -30, 30, -20};

static bool h_two_body(const void *arg) {
    (void)arg;
    median_t a, b;
    int16_t ga[H_MAXSEQ], gb[H_MAXSEQ];
    h_fresh(&a, "a", 3);
    h_fresh(&b, "b", 5);
    for (int i = 0; i < H_LEN(h_a_in); i++) {
        ga[i] = h_update(&a, "a", h_a_in[i], i + 1);
        gb[i] = h_update(&b, "b", h_b_in[i], i + 1);
    }
    size_t off = (size_t)snprintf(h_got, sizeof h_got, "a: ");
    bool ok = h_report(&off, "a", " of a", h_a_in, ga, h_a_want, H_LEN(h_a_in));
    off += (size_t)snprintf(h_got + off, sizeof h_got - off, "; b: ");
    if (off >= sizeof h_got) off = sizeof h_got - 1;
    return h_report(&off, "b", " of b", h_b_in, gb, h_b_want, H_LEN(h_b_in)) && ok;
}

/* One filter re-initialised twice, with a different n each time. */
static const int16_t h_r1_in[]   = {1000, 1000, 1000, 1000, 1000, 1000};
static const int16_t h_r1_want[] = {1000, 1000, 1000, 1000, 1000, 1000};
static const int16_t h_r2_in[]   = {1, 2, 3, 4};
static const int16_t h_r2_want[] = {1, 1, 2, 3};
static const int16_t h_r3_in[]   = {5, -5, 6};
static const int16_t h_r3_want[] = {5, -5, 5};

static bool h_reinit_body(const void *arg) {
    (void)arg;
    median_t m;
    int16_t g1[H_MAXSEQ], g2[H_MAXSEQ], g3[H_MAXSEQ];
    h_fresh(&m, "m", 5);
    for (int i = 0; i < H_LEN(h_r1_in); i++) g1[i] = h_update(&m, "m", h_r1_in[i], i + 1);
    h_reinit(&m, "m", 3);
    for (int i = 0; i < H_LEN(h_r2_in); i++) g2[i] = h_update(&m, "m", h_r2_in[i], i + 1);
    h_reinit(&m, "m", 7);
    for (int i = 0; i < H_LEN(h_r3_in); i++) g3[i] = h_update(&m, "m", h_r3_in[i], i + 1);
    size_t off = (size_t)snprintf(h_got, sizeof h_got, "n=5: ");
    bool ok = h_report(&off, "m", " after median_init(m, 5)", h_r1_in, g1, h_r1_want, H_LEN(h_r1_in));
    off += (size_t)snprintf(h_got + off, sizeof h_got - off, "; n=3: ");
    if (off >= sizeof h_got) off = sizeof h_got - 1;
    ok = h_report(&off, "m", " after median_init(m, 3)", h_r2_in, g2, h_r2_want, H_LEN(h_r2_in)) && ok;
    off += (size_t)snprintf(h_got + off, sizeof h_got - off, "; n=7: ");
    if (off >= sizeof h_got) off = sizeof h_got - 1;
    return h_report(&off, "m", " after median_init(m, 7)", h_r3_in, g3, h_r3_want, H_LEN(h_r3_in)) && ok;
}

/* Long pseudo-random run checked against a slow model that keeps the whole
 * history: copy the last min(k, n) samples, insertion sort, take the lower
 * middle. */
static uint32_t h_rng;
static int16_t h_hist[H_SWEEP_LEN];

static uint32_t h_next(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

/* 1/16 INT16_MIN, 1/16 INT16_MAX, 6/16 full range, 8/16 small values -8..8 (many repeats). */
static int16_t h_rand_sample(void) {
    uint32_t r = h_next();
    uint32_t sel = r & 15u;
    if (sel == 0) return INT16_MIN;
    if (sel == 1) return INT16_MAX;
    if (sel < 8) return (int16_t)((int32_t)((r >> 16) & 0xFFFFu) - 32768);
    return (int16_t)((int32_t)((r >> 8) % 17u) - 8);
}

static int h_window(const int16_t *hist, int k, int n, int16_t *win) {
    int w = k < n ? k : n;
    memcpy(win, hist + k - w, (size_t)w * sizeof win[0]);
    return w;
}

static int16_t h_model(const int16_t *hist, int k, int n) {
    int16_t win[MEDIAN_MAX];
    int w = h_window(hist, k, n, win);
    for (int i = 1; i < w; i++) {
        int16_t v = win[i];
        int j = i - 1;
        while (j >= 0 && win[j] > v) {
            win[j + 1] = win[j];
            j--;
        }
        win[j + 1] = v;
    }
    return win[(w - 1) / 2];
}

static bool h_sweep_body(const void *arg) {
    uint8_t n = *(const uint8_t *)arg;
    median_t m;
    int wrong = 0, fidx = 0;
    int16_t fgot = 0, fexp = 0;
    h_rng = 0x2545F491u + n;
    h_fresh(&m, "m", n);
    for (int i = 0; i < H_SWEEP_LEN; i++) {
        h_hist[i] = h_rand_sample();
        int16_t got = h_update(&m, "m", h_hist[i], i + 1);
        int16_t exp = h_model(h_hist, i + 1, n);
        if (got != exp) {
            if (!wrong) { fidx = i + 1; fgot = got; fexp = exp; }
            wrong++;
        }
    }
    if (!wrong) {
        snprintf(h_got, sizeof h_got, "all %d correct", H_SWEEP_LEN);
        return true;
    }
    int16_t win[MEDIAN_MAX];
    int w = h_window(h_hist, fidx, n, win);
    char wbuf[160];
    h_fmt16(wbuf, sizeof wbuf, win, w);
    snprintf(h_got, sizeof h_got,
             "sample %d: median_update(m, %d) returned %d, expected %d, the median of the last %d samples "
             "%s (oldest first); %d of %d wrong",
             fidx, h_hist[fidx - 1], fgot, fexp, w, wbuf, wrong, H_SWEEP_LEN);
    return false;
}

static void h_sweep_case(const uint8_t *n) {
    char name[64], input[256], expected[64];
    snprintf(name, sizeof name, "%d random samples, n = %u", H_SWEEP_LEN, (unsigned)*n);
    snprintf(input, sizeof input,
             "median_init(m, %u); %d seeded pseudo-random samples: full-range int16 values, "
             "INT16_MIN, INT16_MAX, and small values -8..8 with many repeats",
             (unsigned)*n, H_SWEEP_LEN);
    snprintf(expected, sizeof expected, "all %d correct", H_SWEEP_LEN);
    h_run(name, input, expected, h_sweep_body, n);
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h_on_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    nlc_begin();

    H_SEQ("example 1: spike rejected", NULL, 5, h_ex1_in, h_ex1_want);
    H_SEQ("example 2: warm-up takes the lower middle", NULL, 5, h_ex2_in, h_ex2_want);
    H_SEQ("example 3: n = 3", NULL, 3, h_ex3_in, h_ex3_want);
    H_SEQ("spikes in a flat signal", "single and double spikes, both signs", 5, h_spike_in, h_spike_want);
    H_SEQ("n = 1 returns each sample", NULL, 1, h_n1_in, h_n1_want);
    H_SEQ("n = 3 on a ramp up and down", NULL, 3, h_ramp_in, h_ramp_want);
    H_SEQ("negatives, INT16_MIN and INT16_MAX", NULL, 5, h_ext_in, h_ext_want);
    H_SEQ("all duplicates, then a step", NULL, 7, h_dup_in, h_dup_want);
    H_SEQ("old values leave the window", "1000 and -1000 must stop counting 5 samples later",
          5, h_slide_in, h_slide_want);
    H_SEQ("n = 15 (MEDIAN_MAX)", "a noisy rising signal with spikes at samples 21, 22 and 28",
          15, h_n15_in, h_n15_want);

    {
        char input[512], expected[512];
        int off = snprintf(input, sizeof input,
                           "median_init(a, 3); median_init(b, 5); then alternate median_update(a, x), "
                           "median_update(b, y): a gets ");
        off += (int)h_fmt16(input + off, sizeof input - (size_t)off, h_a_in, H_LEN(h_a_in));
        off += snprintf(input + off, sizeof input - (size_t)off, ", b gets ");
        h_fmt16(input + off, sizeof input - (size_t)off, h_b_in, H_LEN(h_b_in));
        off = snprintf(expected, sizeof expected, "a: ");
        off += (int)h_fmt16(expected + off, sizeof expected - (size_t)off, h_a_want, H_LEN(h_a_want));
        off += snprintf(expected + off, sizeof expected - (size_t)off, "; b: ");
        h_fmt16(expected + off, sizeof expected - (size_t)off, h_b_want, H_LEN(h_b_want));
        h_run("two filters interleaved", input, expected, h_two_body, NULL);
    }

    {
        char input[512], expected[512];
        int off = snprintf(input, sizeof input, "median_init(m, 5); feed ");
        off += (int)h_fmt16(input + off, sizeof input - (size_t)off, h_r1_in, H_LEN(h_r1_in));
        off += snprintf(input + off, sizeof input - (size_t)off, "; median_init(m, 3); feed ");
        off += (int)h_fmt16(input + off, sizeof input - (size_t)off, h_r2_in, H_LEN(h_r2_in));
        off += snprintf(input + off, sizeof input - (size_t)off, "; median_init(m, 7); feed ");
        h_fmt16(input + off, sizeof input - (size_t)off, h_r3_in, H_LEN(h_r3_in));
        off = snprintf(expected, sizeof expected, "n=5: ");
        off += (int)h_fmt16(expected + off, sizeof expected - (size_t)off, h_r1_want, H_LEN(h_r1_want));
        off += snprintf(expected + off, sizeof expected - (size_t)off, "; n=3: ");
        off += (int)h_fmt16(expected + off, sizeof expected - (size_t)off, h_r2_want, H_LEN(h_r2_want));
        off += snprintf(expected + off, sizeof expected - (size_t)off, "; n=7: ");
        h_fmt16(expected + off, sizeof expected - (size_t)off, h_r3_want, H_LEN(h_r3_want));
        h_run("re-init a used filter", input, expected, h_reinit_body, NULL);
    }

    static const uint8_t h_sweep_ns[] = {1, 3, 5, 7, 15};
    for (int i = 0; i < H_LEN(h_sweep_ns); i++) h_sweep_case(&h_sweep_ns[i]);

    nlc_end();
    return 0;
}

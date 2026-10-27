#define _XOPEN_SOURCE 700

#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "nlc.h"
#include "solution.c"

_Static_assert(BM_IDS == 128, "keep BM_IDS at 128: the tests are sized for it");
_Static_assert(sizeof(bitmap_t) == 4 * sizeof(uint32_t),
               "keep bitmap_t as given: uint32_t bits[BM_IDS / 32] and nothing else");

/* Every test runs under a 0.5 s timer. A call that never returns is cut off by
 * SIGALRM, reported as a failed test naming the call, and the remaining tests
 * still run. */
#define H_LIMIT_US 500000
#define H_WORDS 4
#define H_GUARD 0x5AFEC0DEu
#define H_CHURN_OPS 20000
#define H_MAX_RANGES 16

/* The bitmap under test sits between guard words, so a write past either end
 * of bits[] is reported instead of silently corrupting the harness. `used` is
 * the harness's own model of which IDs are in use. */
struct h_bm {
    uint32_t pre[2];
    bitmap_t b;
    uint32_t post[2];
    bool used[BM_IDS];
    const char *name;
};

static sigjmp_buf h_jmp;
static unsigned h_op;
static char h_step[160];
static char h_got[1024];
static const char *h_when;
static struct h_bm *h_tab[2];
static int h_ntab;

static const uint32_t h_junk[H_WORDS] = {0xDEADBEEFu, 0xFFFFFFFFu, 0x0000FFFEu, 0x80000001u};

static void h_on_alarm(int sig) {
    (void)sig;
    siglongjmp(h_jmp, 1);
}

static const char *h_tf(bool b) { return b ? "true" : "false"; }

__attribute__((format(printf, 1, 2)))
static bool h_fail(const char *fmt, ...) {
    int n = snprintf(h_got, sizeof h_got, "%s ", h_step);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(h_got + n, sizeof h_got - (size_t)n, fmt, ap);
    va_end(ap);
    return false;
}

__attribute__((format(printf, 1, 2)))
static void h_append(const char *fmt, ...) {
    size_t n = strlen(h_got);
    if (n + 1 >= sizeof h_got) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(h_got + n, sizeof h_got - n, fmt, ap);
    va_end(ap);
}

static bool h_check_one(const struct h_bm *m) {
    for (int g = 0; g < 2; g++) {
        if (m->pre[g] != H_GUARD)
            return h_fail("wrote outside %s.bits: memory just before bits[0] changed from 0x%08X to 0x%08X",
                          m->name, H_GUARD, m->pre[g]);
        if (m->post[g] != H_GUARD)
            return h_fail("wrote outside %s.bits: memory just after bits[%d] changed from 0x%08X to 0x%08X",
                          m->name, H_WORDS - 1, H_GUARD, m->post[g]);
    }
    for (int w = 0; w < H_WORDS; w++) {
        uint32_t want = 0;
        for (int k = 0; k < 32; k++)
            if (m->used[w * 32 + k]) want |= 1u << k;
        uint32_t got = m->b.bits[w];
        if (got != want) {
            int k = 0;
            while ((((got ^ want) >> k) & 1u) == 0) k++;
            int id = w * 32 + k;
            return h_fail("left %s.bits[%d] = 0x%08X; expected 0x%08X (ID %d should be %s)",
                          m->name, w, got, want, id, m->used[id] ? "in use" : "free");
        }
    }
    return true;
}

/* Every call is followed by a check of the raw words of every live bitmap. */
static bool h_check(void) {
    h_when = "expected after this op";
    for (int i = 0; i < h_ntab; i++)
        if (!h_check_one(h_tab[i])) return false;
    return true;
}

/* Fill the struct with garbage, then init it. */
static bool h_fresh(struct h_bm *m, const char *name) {
    m->pre[0] = m->pre[1] = m->post[0] = m->post[1] = H_GUARD;
    for (int w = 0; w < H_WORDS; w++) m->b.bits[w] = h_junk[w];
    for (int i = 0; i < BM_IDS; i++) m->used[i] = false;
    m->name = name;
    if (h_ntab < 2) h_tab[h_ntab++] = m;
    snprintf(h_step, sizeof h_step, "bm_init(%s) on a garbage-filled struct", name);
    bm_init(&m->b);
    return h_check();
}

static bool h_reinit(struct h_bm *m, const char *what) {
    snprintf(h_step, sizeof h_step, "op %u: bm_init(%s) %s", ++h_op, m->name, what);
    bm_init(&m->b);
    for (int i = 0; i < BM_IDS; i++) m->used[i] = false;
    return h_check();
}

static bool h_alloc(struct h_bm *m, int want) {
    snprintf(h_step, sizeof h_step, "op %u: bm_alloc(%s)", ++h_op, m->name);
    h_when = "before this op";
    int got = bm_alloc(&m->b);
    if (got != want) return h_fail("returned %d; expected %d", got, want);
    if (want >= 0) m->used[want] = true;
    return h_check();
}

static bool h_arun(struct h_bm *m, unsigned n, int want) {
    snprintf(h_step, sizeof h_step, "op %u: bm_alloc_run(%s, %u)", ++h_op, m->name, n);
    h_when = "before this op";
    int got = bm_alloc_run(&m->b, n);
    if (got != want) return h_fail("returned %d; expected %d", got, want);
    for (unsigned i = 0; want >= 0 && i < n; i++) m->used[want + (int)i] = true;
    return h_check();
}

static bool h_free(struct h_bm *m, int id, bool want) {
    snprintf(h_step, sizeof h_step, "op %u: bm_free(%s, %d)", ++h_op, m->name, id);
    h_when = "before this op";
    bool got = bm_free(&m->b, id);
    if (got != want) return h_fail("returned %s; expected %s", h_tf(got), h_tf(want));
    if (want) m->used[id] = false;
    return h_check();
}

/* Allocs that must return first, first+1, ..., first+count-1. */
static bool h_alloc_seq(struct h_bm *m, int first, int count) {
    for (int i = 0; i < count; i++)
        if (!h_alloc(m, first + i)) return false;
    return true;
}

static bool h_free_seq(struct h_bm *m, int first, int last) {
    for (int id = first; id <= last; id++)
        if (!h_free(m, id, true)) return false;
    return true;
}

static void h_run(const char *name, const char *input, const char *expected, bool (*body)(void)) {
    struct itimerval arm = {{0, 0}, {0, H_LIMIT_US}};
    struct itimerval disarm = {{0, 0}, {0, 0}};
    bool ok = false;
    h_op = 0;
    h_ntab = 0;
    snprintf(h_step, sizeof h_step, "(setup)");
    snprintf(h_got, sizeof h_got, "(no result)");
    if (sigsetjmp(h_jmp, 1) == 0) {
        setitimer(ITIMER_REAL, &arm, NULL);
        ok = body();
        setitimer(ITIMER_REAL, &disarm, NULL);
        if (ok) snprintf(h_got, sizeof h_got, "%s", expected);
    } else {
        ok = false;
        snprintf(h_got, sizeof h_got, "%s did not return within %d ms (infinite loop?)",
                 h_step, H_LIMIT_US / 1000);
    }
    nlc_case(name, ok, input, expected, h_got, "");
}

static bool h_case_first(void) {
    struct h_bm t;
    return h_fresh(&t, "t") && h_alloc(&t, 0) && h_alloc(&t, 1) && h_alloc(&t, 2);
}

static bool h_case_hole(void) {
    struct h_bm t;
    return h_fresh(&t, "t") && h_alloc_seq(&t, 0, 10)
        && h_free(&t, 7, true) && h_free(&t, 3, true)
        && h_alloc(&t, 3) && h_alloc(&t, 7) && h_alloc(&t, 10);
}

static bool h_case_fill(void) {
    struct h_bm t;
    return h_fresh(&t, "t") && h_alloc_seq(&t, 0, BM_IDS)
        && h_alloc(&t, -1) && h_alloc(&t, -1)
        && h_free(&t, 100, true) && h_alloc(&t, 100) && h_alloc(&t, -1);
}

static bool h_case_boundaries(void) {
    static const int ids[] = {31, 32, 63, 64, 127};
    struct h_bm t;
    if (!(h_fresh(&t, "t") && h_alloc_seq(&t, 0, BM_IDS))) return false;
    for (int i = 0; i < 5; i++)
        if (!(h_free(&t, ids[i], true) && h_alloc(&t, ids[i]))) return false;
    for (int i = 4; i >= 0; i--)
        if (!h_free(&t, ids[i], true)) return false;
    for (int i = 0; i < 5; i++)
        if (!h_alloc(&t, ids[i])) return false;
    return h_alloc(&t, -1);
}

static bool h_case_bad_free(void) {
    struct h_bm t;
    return h_fresh(&t, "t")
        && h_free(&t, 0, false) && h_free(&t, 127, false)
        && h_free(&t, -1, false) && h_free(&t, 128, false)
        && h_alloc_seq(&t, 0, BM_IDS)
        && h_free(&t, -1, false) && h_free(&t, 128, false) && h_free(&t, 129, false)
        && h_free(&t, 5, true) && h_free(&t, 5, false)
        && h_free(&t, 127, true) && h_free(&t, 127, false)
        && h_free(&t, 0, true) && h_free(&t, 0, false)
        && h_alloc(&t, 0) && h_alloc(&t, 5) && h_alloc(&t, 127) && h_alloc(&t, -1);
}

static bool h_case_run1(void) {
    struct h_bm t;
    if (!(h_fresh(&t, "t")
          && h_arun(&t, 1, 0) && h_arun(&t, 1, 1) && h_alloc(&t, 2) && h_arun(&t, 1, 3)
          && h_free(&t, 1, true) && h_arun(&t, 1, 1) && h_alloc(&t, 4)))
        return false;
    for (int id = 5; id < BM_IDS; id++)
        if (!h_arun(&t, 1, id)) return false;
    return h_arun(&t, 1, -1) && h_alloc(&t, -1);
}

static bool h_case_cross(void) {
    struct h_bm t;
    return h_fresh(&t, "t") && h_alloc_seq(&t, 0, 30)
        && h_arun(&t, 4, 30) && h_alloc(&t, 34)
        && h_arun(&t, 40, 35) && h_alloc(&t, 75)
        && h_arun(&t, 53, -1) && h_arun(&t, 52, 76) && h_alloc(&t, -1);
}

static bool h_case_run32(void) {
    struct h_bm t;
    return h_fresh(&t, "t")
        && h_arun(&t, 32, 0) && h_arun(&t, 32, 32) && h_alloc(&t, 64)
        && h_arun(&t, 32, 65) && h_arun(&t, 32, -1) && h_arun(&t, 31, 97) && h_alloc(&t, -1)
        && h_free_seq(&t, 64, 95)
        && h_arun(&t, 33, -1) && h_arun(&t, 32, 64) && h_alloc(&t, -1);
}

static bool h_case_run128(void) {
    struct h_bm t;
    return h_fresh(&t, "t")
        && h_arun(&t, 128, 0) && h_alloc(&t, -1) && h_arun(&t, 1, -1)
        && h_free(&t, 70, true) && h_arun(&t, 128, -1) && h_arun(&t, 2, -1) && h_arun(&t, 1, 70)
        && h_free_seq(&t, 0, BM_IDS - 1) && h_arun(&t, 128, 0)
        && h_reinit(&t, "on a full bitmap")
        && h_alloc(&t, 0) && h_arun(&t, 128, -1) && h_arun(&t, 127, 1)
        && h_reinit(&t, "on a full bitmap")
        && h_arun(&t, 127, 0) && h_arun(&t, 2, -1) && h_alloc(&t, 127);
}

static bool h_case_gaps(void) {
    struct h_bm t;
    return h_fresh(&t, "t") && h_arun(&t, 128, 0)
        && h_free(&t, 2, true) && h_free_seq(&t, 5, 6) && h_free_seq(&t, 10, 12)
        && h_free_seq(&t, 20, 25) && h_free_seq(&t, 28, 35) && h_free_seq(&t, 40, 43)
        && h_free_seq(&t, 60, 67)
        && h_arun(&t, 4, 20) && h_arun(&t, 6, 28) && h_arun(&t, 4, 40) && h_arun(&t, 4, 60)
        && h_arun(&t, 3, 10) && h_arun(&t, 4, 64) && h_arun(&t, 2, 5) && h_arun(&t, 2, 24)
        && h_arun(&t, 2, 34) && h_arun(&t, 2, -1) && h_alloc(&t, 2) && h_alloc(&t, -1);
}

static bool h_case_bad_n(void) {
    struct h_bm t;
    return h_fresh(&t, "t")
        && h_arun(&t, 0, -1) && h_arun(&t, 129, -1) && h_arun(&t, 1000, -1) && h_arun(&t, UINT_MAX, -1)
        && h_alloc(&t, 0)
        && h_arun(&t, 0, -1) && h_arun(&t, 128, -1) && h_arun(&t, 127, 1)
        && h_arun(&t, 1, -1) && h_arun(&t, 0, -1);
}

static bool h_case_init(void) {
    static const uint32_t pattern[H_WORDS] = {0x55555555u, 0xAAAAAAAAu, 0x0F0F0F0Fu, 0xF0F0F0F0u};
    struct h_bm t;
    if (!(h_fresh(&t, "t") && h_alloc(&t, 0) && h_arun(&t, 127, 1)
          && h_reinit(&t, "on a full bitmap") && h_arun(&t, 128, 0)))
        return false;
    for (int w = 0; w < H_WORDS; w++) t.b.bits[w] = pattern[w];
    return h_reinit(&t, "on bits = {0x55555555, 0xAAAAAAAA, 0x0F0F0F0F, 0xF0F0F0F0}")
        && h_alloc(&t, 0) && h_alloc(&t, 1) && h_arun(&t, 126, 2) && h_alloc(&t, -1);
}

static bool h_case_two(void) {
    struct h_bm a, b;
    return h_fresh(&a, "a") && h_fresh(&b, "b")
        && h_alloc_seq(&a, 0, 3) && h_alloc(&b, 0)
        && h_arun(&a, 40, 3) && h_arun(&b, 40, 1)
        && h_free(&a, 1, true) && h_free(&b, 1, true) && h_free(&b, 50, false) && h_free(&a, 42, true)
        && h_alloc(&a, 1) && h_alloc(&b, 1)
        && h_arun(&a, 128, -1) && h_arun(&b, 87, 41) && h_alloc(&b, -1)
        && h_alloc(&a, 42) && h_alloc(&a, 43);
}

/* Long random workload checked against the model after every call. The PRNG
 * and the op mix match the Python harness, so both languages see the same
 * operations. */
static uint32_t h_rng;

static uint32_t h_rand(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

static int h_model_alloc(const struct h_bm *m) {
    for (int i = 0; i < BM_IDS; i++)
        if (!m->used[i]) return i;
    return -1;
}

static int h_model_run(const struct h_bm *m, unsigned n) {
    if (n == 0 || n > BM_IDS) return -1;
    unsigned len = 0;
    for (int i = 0; i < BM_IDS; i++) {
        len = m->used[i] ? 0 : len + 1;
        if (len == n) return i + 1 - (int)n;
    }
    return -1;
}

static const int h_bad_ids[4] = {-1, 128, 129, 159};
static const unsigned h_odd_ns[4] = {0, 128, 129, UINT_MAX};

/* Mostly in-use IDs, some random (possibly already free) IDs, a few out of range. */
static int h_pick_id(const struct h_bm *m) {
    uint32_t x = h_rand() % 100u;
    if (x < 4) return h_bad_ids[h_rand() % 4u];
    if (x < 50) return (int)(h_rand() % BM_IDS);
    int start = (int)(h_rand() % BM_IDS);
    for (int k = 0; k < BM_IDS; k++)
        if (m->used[(start + k) % BM_IDS]) return (start + k) % BM_IDS;
    return start;
}

static unsigned h_pick_n(void) {
    uint32_t y = h_rand() % 100u;
    if (y < 3) return h_odd_ns[h_rand() % 4u];
    if (y < 60) return 1u + h_rand() % 4u;
    if (y < 90) return 1u + h_rand() % 16u;
    return 1u + h_rand() % 64u;
}

static bool h_churn_fail(const struct h_bm *m) {
    int in_use = 0, shown = 0;
    for (int i = 0; i < BM_IDS; i++) in_use += m->used[i];
    h_append(" [model %s: %d of %d IDs in use; free IDs:", h_when, in_use, BM_IDS);
    for (int i = 0; i < BM_IDS;) {
        if (m->used[i]) {
            i++;
            continue;
        }
        int j = i;
        while (j + 1 < BM_IDS && !m->used[j + 1]) j++;
        if (shown == H_MAX_RANGES) {
            h_append(", ...");
            break;
        }
        if (i == j) h_append("%s%d", shown ? ", " : " ", i);
        else h_append("%s%d-%d", shown ? ", " : " ", i, j);
        shown++;
        i = j + 1;
    }
    h_append("%s]", shown ? "" : " none");
    return false;
}

static bool h_case_churn(void) {
    struct h_bm t;
    if (!h_fresh(&t, "t")) return false;
    h_rng = 0x9E3779B9u;
    for (int k = 0; k < H_CHURN_OPS; k++) {
        uint32_t free_pct = (k / 1000) % 2 ? 85u : 40u;
        uint32_t r = h_rand() % 100u;
        bool ok;
        if (r < free_pct) {
            int id = h_pick_id(&t);
            ok = h_free(&t, id, id >= 0 && id < BM_IDS && t.used[id]);
        } else if (r < free_pct + (100u - free_pct) / 2u) {
            ok = h_alloc(&t, h_model_alloc(&t));
        } else {
            unsigned n = h_pick_n();
            ok = h_arun(&t, n, h_model_run(&t, n));
        }
        if (!ok) return h_churn_fail(&t);
    }
    return true;
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h_on_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    nlc_begin();

    h_run("first allocs return 0, 1, 2",
          "bm_init on a garbage-filled struct; alloc x3",
          "init leaves every bit 0; the allocs return 0, 1, 2, leaving bits[0] = 0x00000007",
          h_case_first);
    h_run("alloc reuses the lowest hole",
          "alloc x10 (IDs 0..9); free 7; free 3; alloc x3",
          "both frees return true; the allocs return 3, 7, 10",
          h_case_hole);
    h_run("fill all 128",
          "alloc x128; alloc x2; free 100; alloc x2",
          "allocs return 0..127, then -1 twice with bits unchanged; free 100 returns true; "
          "then alloc returns 100, then -1",
          h_case_fill);
    h_run("word boundary IDs 31, 32, 63, 64, 127",
          "alloc x128; for each of 31, 32, 63, 64, 127: free it, then alloc; "
          "then free 127, 64, 63, 32, 31; alloc x6",
          "each alloc after a single free returns the ID just freed; "
          "the last six allocs return 31, 32, 63, 64, 127, -1",
          h_case_boundaries);
    h_run("free out of range and double free",
          "empty: free 0, 127, -1, 128; alloc x128; free -1, 128, 129; "
          "free 5 twice, free 127 twice, free 0 twice; alloc x4",
          "frees of free or out-of-range IDs return false and leave bits unchanged; "
          "the first free of 5, 127 and 0 returns true; the allocs return 0, 5, 127, -1",
          h_case_bad_free);
    h_run("run of 1 behaves like bm_alloc",
          "run(1), run(1), alloc, run(1); free 1; run(1), alloc; run(1) x123; run(1), alloc",
          "0, 1, 2, 3; free returns true; 1, 4; the 123 runs return 5..127; then -1, -1",
          h_case_run1);
    h_run("run crossing word boundaries",
          "alloc x30 (IDs 0..29); run(4); alloc; run(40); alloc; run(53); run(52); alloc",
          "run(4) returns 30 (IDs 30..33 span bits[0] and bits[1]); alloc 34; run(40) returns 35; "
          "alloc 75; run(53) returns -1 with bits unchanged; run(52) returns 76; alloc -1",
          h_case_cross);
    h_run("runs of exactly 32",
          "run(32), run(32), alloc, run(32), run(32), run(31), alloc; free 64..95; "
          "run(33), run(32), alloc",
          "0, 32, 64, 65, -1 (only 31 IDs free), 97, -1; the frees return true; "
          "run(33) -1, run(32) 64, alloc -1",
          h_case_run32);
    h_run("run of all 128",
          "run(128), alloc, run(1); free 70; run(128), run(2), run(1); free 0..127; run(128); "
          "bm_init; alloc, run(128), run(127); bm_init; run(127), run(2), alloc",
          "0, -1, -1; free true; -1, -1 (bits unchanged), 70; frees true; 0; "
          "after init: 0, -1, 1; after init: 0, -1, 127",
          h_case_run128);
    h_run("lowest start that fits",
          "run(128); free 2, 5..6, 10..12, 20..25, 28..35, 40..43, 60..67; "
          "run(4), run(6), run(4), run(4), run(3), run(4), run(2), run(2), run(2), run(2); alloc x2",
          "20, 28, 40, 60, 10, 64, 5, 24, 34, -1; alloc 2, -1. Each run starts at the lowest ID "
          "where it fits, not in the smallest gap and not only within one word",
          h_case_gaps);
    h_run("n of 0 or above BM_IDS",
          "empty: run(0), run(129), run(1000), run(4294967295); alloc; run(0), run(128), run(127); "
          "run(1), run(0)",
          "-1 four times with bits unchanged; alloc 0; -1, -1, 1; -1, -1",
          h_case_bad_n);
    h_run("init clears any previous state",
          "bm_init on a garbage-filled struct; alloc, run(127); bm_init; run(128); "
          "overwrite bits with {0x55555555, 0xAAAAAAAA, 0x0F0F0F0F, 0xF0F0F0F0}; bm_init; "
          "alloc x2, run(126), alloc",
          "every bm_init leaves all four words 0; alloc 0, run(127) 1; run(128) 0; "
          "alloc 0, 1, run(126) 2, alloc -1",
          h_case_init);
    h_run("two bitmaps are independent",
          "bitmaps a and b: a alloc x3; b alloc; a run(40); b run(40); free a 1, b 1, b 50, a 42; "
          "a alloc; b alloc; a run(128); b run(87); b alloc; a alloc x2",
          "each call changes only its own bitmap: a 0, 1, 2; b 0; a 3; b 1; "
          "true, true, false, true; a 1; b 1; a -1; b 41; b -1; a 42, 43",
          h_case_two);
    h_run("20000 random operations",
          "seeded random bm_alloc, bm_alloc_run (n mostly 1..16, some up to 64, a few of "
          "0, 128, 129, 4294967295) and bm_free (in-use IDs, random IDs, and -1, 128, 129, 159), "
          "switching every 1000 ops between mostly allocating and mostly freeing",
          "every return value and the raw bits after every call match a reference model",
          h_case_churn);

    nlc_end();
    return 0;
}

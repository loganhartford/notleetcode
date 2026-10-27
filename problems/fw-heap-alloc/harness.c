#define _XOPEN_SOURCE 700

#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "nlc.h"
#include "solution.c"

/* Every test runs under a 0.4 s timer. A call that never returns (a walk
 * that loops back on itself) is cut off by SIGALRM, reported as a failed test
 * naming the call, and the remaining tests still run.
 *
 * Each arena sits between two 64-byte guard zones that must never change, and
 * every live block is filled with its own byte pattern. After every call the
 * harness checks the guards, the contents of every live block, and (for a
 * returned pointer) alignment, bounds and overlap with the live blocks. */
#define H_LIMIT_US 400000
#define H_GUARD 64
#define H_MAX_ARENA 65536
#define H_MAX_LIVE (H_MAX_ARENA / 8)
#define H_PAT_LEN (H_MAX_ARENA + 256)
#define H_ROUNDS 4
#define H_ROUND_OPS 5000

enum { H_JUNK_MIXED, H_JUNK_ZERO, H_JUNK_ONES };
enum { H_FORWARD, H_REVERSE, H_ALTERNATE, H_SHUFFLE };

typedef struct {
    uint8_t *p;
    size_t n;
    unsigned seed;
    unsigned born;
} h_blk;

typedef struct {
    const char *name;
    heap_t h;
    uint8_t *arena;
    size_t size;
    size_t nlive;
    h_blk live[H_MAX_LIVE];
} h_heap;

_Alignas(16) static uint8_t h_mem[2][H_GUARD + H_MAX_ARENA + H_GUARD];
static h_heap h_heaps[2];
static h_heap *h_used[2];
static int h_nused;
static uint8_t h_pat[H_PAT_LEN];
static uint8_t h_canary[H_GUARD];

static sigjmp_buf h_jmp;
static unsigned h_op;
static char h_step[200];
static char h_got[900];
static uint32_t h_rng;

static void h_on_alarm(int sig) {
    (void)sig;
    siglongjmp(h_jmp, 1);
}

static uint32_t h_rand(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

static size_t h_r8(size_t n) { return (n + 7) & ~(size_t)7; }

__attribute__((format(printf, 1, 2)))
static bool h_fail(const char *fmt, ...) {
    int n = snprintf(h_got, sizeof h_got, "%s ", h_step);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(h_got + n, sizeof h_got - (size_t)n, fmt, ap);
    va_end(ap);
    return false;
}

/* Pointers print as offsets from the start of their heap's arena. */
static const char *h_ptr(const h_heap *hh, const void *p) {
    static char buf[6][48];
    static int next;
    char *s = buf[next++ % 6];
    if (!p) {
        snprintf(s, sizeof buf[0], "NULL");
    } else {
        uintptr_t a = (uintptr_t)p, base = (uintptr_t)hh->arena;
        if (a >= base) snprintf(s, sizeof buf[0], "arena+%llu", (unsigned long long)(a - base));
        else snprintf(s, sizeof buf[0], "arena-%llu", (unsigned long long)(base - a));
    }
    return s;
}

static void h_junk(uint8_t *a, size_t size, int kind) {
    if (kind == H_JUNK_ZERO) { memset(a, 0x00, size); return; }
    if (kind == H_JUNK_ONES) { memset(a, 0xFF, size); return; }
    for (size_t i = 0; i < size; i += 8) {
        uint64_t w = ((i / 8) % 2) ? ((uint64_t)h_rand() << 32 | h_rand())
                                   : (uint64_t)((i * 3) % 200 + 8) | 1u;
        memcpy(a + i, &w, 8);
    }
}

/* Prepare a heap's arena, guards and struct; no user code runs here. */
static h_heap *h_setup(int slot, const char *name, size_t size, int junk) {
    h_heap *hh = &h_heaps[slot];
    hh->name = name;
    hh->size = size;
    hh->nlive = 0;
    hh->arena = h_mem[slot] + H_GUARD;
    memcpy(hh->arena - H_GUARD, h_canary, H_GUARD);
    h_junk(hh->arena, size, junk);
    memcpy(hh->arena + size, h_canary, H_GUARD);
    memset(&hh->h, 0xA5, sizeof hh->h);
    bool known = false;
    for (int i = 0; i < h_nused; i++) known = known || h_used[i] == hh;
    if (!known) h_used[h_nused++] = hh;
    return hh;
}

static bool h_check_heap(const h_heap *hh) {
    const uint8_t *lo = hh->arena - H_GUARD, *hi = hh->arena + hh->size;
    for (int i = 0; i < H_GUARD; i++) {
        if (lo[i] != h_canary[i])
            return h_fail("then byte %s (before the start of %s's arena) changed from 0x%02X to 0x%02X; "
                          "the heap must not write outside the arena",
                          h_ptr(hh, lo + i), hh->name, h_canary[i], lo[i]);
        if (hi[i] != h_canary[i])
            return h_fail("then byte %s (past the end of %s's %zu-byte arena) changed from 0x%02X to 0x%02X; "
                          "the heap must not write outside the arena",
                          h_ptr(hh, hi + i), hh->name, hh->size, h_canary[i], hi[i]);
    }
    for (size_t k = 0; k < hh->nlive; k++) {
        const h_blk *b = &hh->live[k];
        const uint8_t *want = h_pat + b->seed;
        if (memcmp(b->p, want, b->n) != 0) {
            size_t i = 0;
            while (b->p[i] == want[i]) i++;
            return h_fail("then live block %s (%zu bytes, from op %u) had byte %zu changed from 0x%02X to 0x%02X; "
                          "the heap must not write inside a live block",
                          h_ptr(hh, b->p), b->n, b->born, i, want[i], b->p[i]);
        }
    }
    return true;
}

static bool h_check_all(void) {
    for (int i = 0; i < h_nused; i++)
        if (!h_check_heap(h_used[i])) return false;
    return true;
}

static bool h_init(h_heap *hh) {
    snprintf(h_step, sizeof h_step, "op %u: heap_init(%s, arena, %zu) on a garbage-filled struct and arena",
             ++h_op, hh->name, hh->size);
    hh->nlive = 0;
    heap_init(&hh->h, hh->arena, hh->size);
    return h_check_all();
}

/* Call heap_alloc and validate whatever comes back; NULL is accepted here. */
static bool h_alloc(h_heap *hh, size_t n, uint8_t **out) {
    snprintf(h_step, sizeof h_step, "op %u: heap_alloc(%s, %zu)", ++h_op, hh->name, n);
    uint8_t *p = heap_alloc(&hh->h, n);
    *out = p;
    if (p) {
        uintptr_t a = (uintptr_t)p, base = (uintptr_t)hh->arena;
        if (n == 0) return h_fail("returned %s; expected NULL for n == 0", h_ptr(hh, p));
        if (a < base || a - base > hh->size || n > hh->size - (a - base))
            return h_fail("returned %s, but %zu bytes there do not fit inside the %zu-byte arena",
                          h_ptr(hh, p), n, hh->size);
        if (a % 8) return h_fail("returned %s, which is not 8-byte aligned", h_ptr(hh, p));
        for (size_t k = 0; k < hh->nlive; k++) {
            const h_blk *b = &hh->live[k];
            if (p < b->p + b->n && b->p < p + n)
                return h_fail("returned %s, but its %zu bytes overlap live block %s (%zu bytes, from op %u)",
                              h_ptr(hh, p), n, h_ptr(hh, b->p), b->n, b->born);
        }
        if (hh->nlive == H_MAX_LIVE) return h_fail("returned a block, but the heap already has %d live blocks", H_MAX_LIVE);
        h_blk *b = &hh->live[hh->nlive++];
        *b = (h_blk){p, n, h_rand() % 256u, h_op};
        memcpy(p, h_pat + b->seed, n);
    }
    return h_check_all();
}

static bool h_alloc_ok(h_heap *hh, size_t n, uint8_t **out, const char *why) {
    if (!h_alloc(hh, n, out)) return false;
    if (!*out) return h_fail("returned NULL; expected a block (%s)", why);
    return true;
}

static bool h_alloc_at(h_heap *hh, size_t n, uint8_t **out, const uint8_t *want, const char *why) {
    if (!h_alloc(hh, n, out)) return false;
    if (*out != want) return h_fail("returned %s; expected %s (%s)", h_ptr(hh, *out), h_ptr(hh, want), why);
    return true;
}

static bool h_alloc_null(h_heap *hh, size_t n, const char *why) {
    uint8_t *p;
    if (!h_alloc(hh, n, &p)) return false;
    if (p) return h_fail("returned %s; expected NULL (%s)", h_ptr(hh, p), why);
    return true;
}

static bool h_free(h_heap *hh, uint8_t *p) {
    snprintf(h_step, sizeof h_step, "op %u: heap_free(%s, %s)", ++h_op, hh->name, h_ptr(hh, p));
    if (p) {
        size_t k = 0;
        while (k < hh->nlive && hh->live[k].p != p) k++;
        if (k == hh->nlive) return h_fail("(harness error: freeing a block that is not live)");
        memmove(&hh->live[k], &hh->live[k + 1], (hh->nlive - k - 1) * sizeof hh->live[0]);
        hh->nlive--;
    }
    heap_free(&hh->h, p);
    return h_check_all();
}

static int h_cmp_ptr(const void *x, const void *y) {
    uintptr_t a = (uintptr_t)*(uint8_t *const *)x, b = (uintptr_t)*(uint8_t *const *)y;
    return (a > b) - (a < b);
}

static bool h_free_all(h_heap *hh, int order) {
    static uint8_t *ps[H_MAX_LIVE], *seq[H_MAX_LIVE];
    size_t n = hh->nlive;
    for (size_t k = 0; k < n; k++) ps[k] = hh->live[k].p;
    qsort(ps, n, sizeof ps[0], h_cmp_ptr);
    size_t m = 0;
    if (order == H_FORWARD || order == H_SHUFFLE) {
        for (size_t k = 0; k < n; k++) seq[m++] = ps[k];
    } else if (order == H_REVERSE) {
        for (size_t k = n; k-- > 0;) seq[m++] = ps[k];
    } else {
        for (size_t k = 0; k < n; k += 2) seq[m++] = ps[k];
        for (size_t k = 1; k < n; k += 2) seq[m++] = ps[k];
    }
    if (order == H_SHUFFLE) {
        for (size_t k = m; k > 1; k--) {
            size_t j = h_rand() % k;
            uint8_t *t = seq[k - 1];
            seq[k - 1] = seq[j];
            seq[j] = t;
        }
    }
    for (size_t k = 0; k < m; k++)
        if (!h_free(hh, seq[k])) return false;
    return true;
}

static bool h_big(h_heap *hh) {
    uint8_t *p;
    return h_alloc_ok(hh, hh->size - 16, &p, "with no live blocks, size - 16 bytes must fit");
}

static void h_run(const char *name, const char *input, const char *expected, bool (*body)(void)) {
    struct itimerval arm = {{0, 0}, {0, H_LIMIT_US}};
    struct itimerval disarm = {{0, 0}, {0, 0}};
    bool ok = false;
    h_op = 0;
    h_nused = 0;
    h_rng = 0x9E3779B9u;
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

static const char *const H_FRESH = "an empty heap fits any sequence of requests whose r(n) + 16 sum to at most size";
static const char *const H_HOLE = "a freed block's space holds any request up to that block's size";

static bool h_case_basic(void) {
    h_heap *hh = h_setup(0, "h", 1024, H_JUNK_MIXED);
    uint8_t *a, *b, *c, *d, *e;
    return h_init(hh)
        && h_alloc_ok(hh, 10, &a, H_FRESH) && h_alloc_ok(hh, 100, &b, H_FRESH)
        && h_alloc_ok(hh, 1, &c, H_FRESH) && h_alloc_ok(hh, 250, &d, H_FRESH)
        && h_free(hh, b) && h_alloc_ok(hh, 40, &e, H_HOLE)
        && h_free(hh, a) && h_free(hh, d)
        && h_alloc_ok(hh, 200, &d, H_HOLE)
        && h_free(hh, c) && h_free(hh, e) && h_free(hh, d);
}

static bool h_case_align(void) {
    h_heap *hh = h_setup(0, "h", 4096, H_JUNK_MIXED);
    static uint8_t *p[41];
    if (!h_init(hh)) return false;
    for (size_t n = 1; n <= 40; n++)
        if (!h_alloc_ok(hh, n, &p[n], H_FRESH)) return false;
    for (size_t n = 1; n <= 40; n += 3)
        if (!h_free(hh, p[n])) return false;
    for (size_t n = 1; n <= 40; n += 3)
        if (!h_alloc_ok(hh, n == 1 ? 1 : n - 1, &p[n], H_HOLE))
            return false;
    return true;
}

static bool h_case_zero(void) {
    h_heap *hh = h_setup(0, "h", 256, H_JUNK_MIXED);
    uint8_t *a;
    return h_init(hh)
        && h_alloc_null(hh, 0, "n == 0") && h_alloc_ok(hh, 8, &a, H_FRESH)
        && h_alloc_null(hh, 0, "n == 0") && h_free(hh, NULL) && h_free(hh, a) && h_free(hh, NULL)
        && h_big(hh);
}

static bool h_case_too_big(void) {
    h_heap *hh = h_setup(0, "h", 512, H_JUNK_MIXED);
    uint8_t *a;
    return h_init(hh)
        && h_alloc_null(hh, 513, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, 520, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, 100000, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, SIZE_MAX, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, SIZE_MAX - 6, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, SIZE_MAX / 2 + 1, "n is larger than the 512-byte arena")
        && h_big(hh) && h_free(hh, hh->live[0].p)
        && h_alloc_ok(hh, 300, &a, H_FRESH)
        && h_alloc_null(hh, 513, "n is larger than the 512-byte arena")
        && h_alloc_null(hh, SIZE_MAX - 6, "n is larger than the 512-byte arena");
}

static bool h_case_first_fit(void) {
    h_heap *hh = h_setup(0, "h", 1024, H_JUNK_MIXED);
    uint8_t *a, *b, *c, *d, *e, *x;
    return h_init(hh)
        && h_alloc_ok(hh, 64, &a, H_FRESH) && h_alloc_ok(hh, 128, &b, H_FRESH)
        && h_alloc_ok(hh, 64, &c, H_FRESH) && h_alloc_ok(hh, 64, &d, H_FRESH)
        && h_alloc_ok(hh, 64, &e, H_FRESH)
        && h_free(hh, b) && h_free(hh, d)
        && h_alloc_at(hh, 64, &x, b, "b's hole is the lowest free space that fits, even though d's is a tighter fit")
        && h_alloc_at(hh, 64, &x, d, "the rest of b's hole is too small for 64 bytes; d's hole is next")
        && h_free(hh, a)
        && h_alloc_ok(hh, 200, &x, "the space above e fits 200 bytes")
        && (x > e || h_fail("returned %s; expected an address above e = %s (a's hole is too small for 200 bytes)",
                            h_ptr(hh, x), h_ptr(hh, e)))
        && h_alloc_at(hh, 16, &x, a, "a's hole is now the lowest free space that fits")
        && h_alloc_ok(hh, 24, &x, "the rest of a's hole fits 24 bytes")
        && ((x > a && x < b) || h_fail("returned %s; expected an address between a = %s and b = %s "
                                       "(the rest of a's hole is the lowest free space that fits)",
                                       h_ptr(hh, x), h_ptr(hh, a), h_ptr(hh, b)));
}

static bool h_fill_count(h_heap *hh, size_t n, size_t want) {
    uint8_t *p;
    size_t count = 0;
    for (;;) {
        if (!h_alloc(hh, n, &p)) return false;
        if (!p) break;
        count++;
    }
    if (count < want)
        return h_fail("returned NULL after only %zu blocks of %zu bytes; at least %zu = %zu / (%zu + 16) must fit",
                      count, n, want, hh->size, h_r8(n));
    return true;
}

static bool h_case_fill16(void) {
    h_heap *hh = h_setup(0, "h", 4096, H_JUNK_MIXED);
    return h_init(hh) && h_fill_count(hh, 16, 4096 / 32) && h_free_all(hh, H_FORWARD) && h_big(hh);
}

static bool h_case_pack(void) {
    static const size_t sizes[] = {1, 8, 13, 24, 40, 64, 100, 7, 250, 33};
    h_heap *hh = h_setup(0, "h", 2048, H_JUNK_MIXED);
    if (!(h_init(hh) && h_fill_count(hh, 1, 2048 / 24))) return false;
    hh = h_setup(0, "h", 2048, H_JUNK_MIXED);
    if (!h_init(hh)) return false;
    size_t used = 0;
    for (int i = 0;; i++) {
        size_t n = sizes[i % 10];
        if (used + h_r8(n) + 16 > hh->size) break;
        used += h_r8(n) + 16;
        uint8_t *p;
        if (!h_alloc_ok(hh, n, &p, H_FRESH)) return false;
    }
    return true;
}

static bool h_fill_mixed(h_heap *hh) {
    static const size_t sizes[] = {24, 1, 100, 48, 8, 200, 16, 64, 33, 120};
    uint8_t *p;
    size_t used = 0, sure = 0;
    for (int i = 0;; i++) {
        size_t n = sizes[i % 10];
        if (used + h_r8(n) + 16 <= hh->size) {
            used += h_r8(n) + 16;
            sure++;
        } else {
            used = hh->size;
        }
        if (!h_alloc(hh, n, &p)) return false;
        if (!p) break;
    }
    if (hh->nlive < sure)
        return h_fail("returned NULL after %zu blocks; the first %zu requests of this sequence must fit (%s)",
                      hh->nlive, sure, H_FRESH);
    return true;
}

static bool h_free_all_case(int order) {
    h_heap *hh = h_setup(0, "h", 4096, H_JUNK_MIXED);
    uint8_t *p;
    if (!h_init(hh)) return false;
    for (int round = 0; round < 2; round++)
        if (!(h_fill_mixed(hh) && h_free_all(hh, order) && h_big(hh) && h_free(hh, hh->live[0].p)))
            return false;
    return h_alloc_ok(hh, 2048, &p, H_FRESH) && h_alloc_ok(hh, 2000, &p, H_FRESH);
}

static bool h_case_free_forward(void) { return h_free_all_case(H_FORWARD); }
static bool h_case_free_reverse(void) { return h_free_all_case(H_REVERSE); }
static bool h_case_free_alternate(void) { return h_free_all_case(H_ALTERNATE); }
static bool h_case_free_shuffle(void) { return h_free_all_case(H_SHUFFLE); }

static bool h_case_merge(void) {
    static const int orders[4][3] = {{0, 1, -1}, {1, 0, -1}, {0, 2, 1}, {1, 2, 0}};
    static const size_t two[] = {37, 20}, three[] = {16, 40, 24};
    for (int s = 0; s < 4; s++) {
        const size_t *sz = s < 2 ? two : three;
        int k = s < 2 ? 2 : 3;
        uint8_t *p[3], *g, *x;
        size_t want = 0;
        h_heap *hh = h_setup(0, "h", 512, H_JUNK_MIXED);
        if (!h_init(hh)) return false;
        for (int i = 0; i < k; i++) {
            if (!h_alloc_ok(hh, sz[i], &p[i], H_FRESH)) return false;
            want += h_r8(sz[i]);
        }
        if (!h_alloc_ok(hh, 8, &g, H_FRESH)) return false;
        for (int i = 0; i < k; i++)
            if (!h_free(hh, p[orders[s][i]])) return false;
        if (!h_alloc_at(hh, want, &x, p[0], "the freed neighbours form one space starting at the first of them"))
            return false;
    }
    return true;
}

#define H_A(n) (n)
#define H_F(k) (-(k) - 1)

static bool h_case_contents(void) {
    static const int script[] = {
        H_A(24), H_A(7), H_A(100), H_A(64), H_A(1), H_A(33), H_A(200), H_A(16), H_A(48), H_A(9), H_A(130), H_A(72),
        H_F(1), H_F(3), H_F(2), H_A(50), H_A(60), H_A(8),
        H_F(6), H_F(8), H_F(7), H_A(250),
        H_F(0), H_F(4), H_F(5), H_A(1), H_A(2), H_A(3), H_A(4),
        H_F(15), H_F(10), H_F(11), H_F(9), H_A(300),
    };
    static uint8_t *blk[32];
    int nb = 0;
    h_heap *hh = h_setup(0, "h", 2048, H_JUNK_MIXED);
    if (!h_init(hh)) return false;
    for (size_t i = 0; i < sizeof script / sizeof script[0]; i++) {
        int v = script[i];
        if (v > 0) {
            bool ok = nb < 12 ? h_alloc_ok(hh, (size_t)v, &blk[nb], H_FRESH) : h_alloc(hh, (size_t)v, &blk[nb]);
            if (!ok) return false;
            nb++;
        } else {
            if (!h_free(hh, blk[-v - 1])) return false;
            blk[-v - 1] = NULL;
        }
    }
    for (int k = nb; k-- > 0;)
        if (blk[k] && !h_free(hh, blk[k])) return false;
    return h_big(hh);
}

static bool h_case_edges(void) {
    h_heap *hh = h_setup(0, "h", 256, H_JUNK_MIXED);
    uint8_t *p;
    if (!(h_init(hh) && h_alloc_ok(hh, 240, &p, H_FRESH) && h_free(hh, p))) return false;
    for (int i = 0; i < 8; i++)
        if (!h_alloc_ok(hh, 16, &p, H_FRESH)) return false;
    if (!(h_free_all(hh, H_REVERSE) && h_big(hh) && h_free(hh, hh->live[0].p))) return false;
    hh = h_setup(0, "h", 256, H_JUNK_MIXED);
    if (!h_init(hh)) return false;
    for (int i = 0; i < 10; i++)
        if (!h_alloc_ok(hh, 5, &p, H_FRESH)) return false;
    return h_free_all(hh, H_FORWARD) && h_big(hh);
}

static bool h_case_min_max(void) {
    uint8_t *p, *q;
    h_heap *hh = h_setup(0, "h", 64, H_JUNK_MIXED);
    if (!(h_init(hh) && h_alloc_ok(hh, 48, &p, H_FRESH) && h_free(hh, p)
          && h_alloc_ok(hh, 8, &p, H_FRESH) && h_alloc_ok(hh, 8, &q, H_FRESH)
          && h_free(hh, p) && h_free(hh, q) && h_big(hh)))
        return false;
    hh = h_setup(0, "h", 65536, H_JUNK_MIXED);
    return h_init(hh) && h_big(hh) && h_free(hh, hh->live[0].p)
        && h_alloc_ok(hh, 30000, &p, H_FRESH) && h_alloc_ok(hh, 30000, &q, H_FRESH)
        && h_alloc_ok(hh, 5000, &p, H_FRESH)
        && h_free_all(hh, H_SHUFFLE) && h_big(hh);
}

static bool h_case_garbage(void) {
    static const int kinds[] = {H_JUNK_ZERO, H_JUNK_ONES, H_JUNK_MIXED};
    uint8_t *a, *b, *c;
    for (int i = 0; i < 3; i++) {
        h_heap *hh = h_setup(0, "h", 512, kinds[i]);
        if (!(h_init(hh) && h_alloc_ok(hh, 100, &a, H_FRESH) && h_alloc_ok(hh, 50, &b, H_FRESH)
              && h_alloc_ok(hh, 8, &c, H_FRESH) && h_free(hh, b)
              && h_alloc_ok(hh, 40, &b, H_HOLE)
              && h_free_all(hh, H_SHUFFLE) && h_big(hh)))
            return false;
    }
    h_heap *hh = &h_heaps[0];
    return h_free(hh, hh->live[0].p)
        && h_alloc_ok(hh, 200, &a, H_FRESH) && h_alloc_ok(hh, 100, &b, H_FRESH)
        && h_alloc_ok(hh, 64, &c, H_FRESH) && h_init(hh) && h_big(hh);
}

static bool h_case_two_heaps(void) {
    h_heap *ha = h_setup(0, "a", 512, H_JUNK_MIXED);
    h_heap *hb = h_setup(1, "b", 1024, H_JUNK_ONES);
    uint8_t *a1, *a2, *a3, *b1, *b2, *b3, *x;
    return h_init(ha) && h_init(hb)
        && h_alloc_ok(ha, 64, &a1, H_FRESH) && h_alloc_ok(hb, 64, &b1, H_FRESH)
        && h_alloc_ok(ha, 100, &a2, H_FRESH) && h_alloc_ok(hb, 200, &b2, H_FRESH)
        && h_free(ha, a1) && h_alloc_ok(hb, 32, &b3, H_FRESH)
        && h_alloc_at(ha, 32, &a3, a1, "a1's hole is a's lowest free space that fits")
        && h_free(hb, b1) && h_alloc_at(hb, 64, &x, b1, "b1's hole is b's lowest free space that fits")
        && h_free_all(ha, H_FORWARD) && h_big(ha)
        && h_free_all(hb, H_SHUFFLE) && h_big(hb)
        && h_free(ha, ha->live[0].p) && h_alloc_ok(ha, 8, &x, H_FRESH);
}

static bool h_case_random(void) {
    h_heap *hh = h_setup(0, "h", 8192, H_JUNK_MIXED);
    if (!h_init(hh)) return false;
    unsigned fails = 0;
    for (int round = 0; round < H_ROUNDS; round++) {
        for (int i = 0; i < H_ROUND_OPS; i++) {
            bool ok;
            if (hh->nlive == 0 || h_rand() % 100u < 55u) {
                uint8_t *p;
                size_t n = 1 + h_rand() % 300u;
                ok = h_alloc(hh, n, &p);
                if (ok && !p) fails++;
            } else {
                ok = h_free(hh, hh->live[h_rand() % hh->nlive].p);
            }
            if (!ok) {
                size_t len = strlen(h_got), bytes = 0;
                for (size_t k = 0; k < hh->nlive; k++) bytes += hh->live[k].n;
                snprintf(h_got + len, sizeof h_got - len, " [workload round %d: %zu live blocks holding %zu bytes]",
                         round + 1, hh->nlive, bytes);
                return false;
            }
        }
        if (!(h_free_all(hh, H_SHUFFLE) && h_big(hh) && h_free(hh, hh->live[0].p))) return false;
    }
    if (fails == 0) return h_fail("(harness error: the workload never filled the heap)");
    return true;
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h_on_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);

    h_rng = 0x2545F491u;
    for (size_t i = 0; i < H_PAT_LEN; i++) h_pat[i] = (uint8_t)(h_rand() >> 11);
    for (int i = 0; i < H_GUARD; i++) h_canary[i] = (i % 8 == 0) ? 0x21 : 0x00;

    nlc_begin();

    h_run("alloc, write, read back",
          "size 1024: alloc 10, 100, 1, 250; free the 100; alloc 40; free the 10 and the 250; alloc 200; free all",
          "every alloc returns a usable block; bytes written to each block read back unchanged",
          h_case_basic);
    h_run("8-byte alignment",
          "size 4096: alloc n = 1, 2, ..., 40; free n = 1, 4, 7, ..., 40; alloc 1, 3, 6, ..., 39 (one less each)",
          "every returned pointer is a multiple of 8",
          h_case_align);
    h_run("n == 0 and heap_free(NULL)",
          "size 256: alloc 0; alloc 8; alloc 0; free NULL; free the 8; free NULL; alloc 240",
          "alloc 0 returns NULL; free NULL does nothing; the empty heap still fits 240 bytes",
          h_case_zero);
    h_run("requests larger than the arena",
          "size 512: alloc 513, 520, 100000, SIZE_MAX, SIZE_MAX - 6, SIZE_MAX / 2 + 1; alloc 496; free it; "
          "alloc 300; alloc 513, SIZE_MAX - 6",
          "every request larger than the arena returns NULL (no overflow when rounding n up); "
          "they use no space, so 496 still fits",
          h_case_too_big);
    h_run("first fit by address",
          "size 1024: a = alloc 64, b = alloc 128, c = alloc 64, d = alloc 64, e = alloc 64; free b, free d; "
          "alloc 64; alloc 64; free a; alloc 200; alloc 16; alloc 24",
          "alloc 64 -> b (lowest hole that fits, though d's is tighter); alloc 64 -> d (rest of b's hole is too "
          "small); alloc 200 -> above e; alloc 16 -> a; alloc 24 -> between a and b",
          h_case_first_fit);
    h_run("pack 16-byte blocks",
          "size 4096: alloc 16 until NULL; free all in address order; alloc 4080",
          "at least 4096 / (16 + 16) = 128 blocks fit; once all are freed, 4080 fits",
          h_case_fill16);
    h_run("pack small and mixed blocks",
          "size 2048: alloc 1 until NULL. Then a fresh 2048 heap: alloc 1, 8, 13, 24, 40, 64, 100, 7, 250, 33, "
          "1, 8, ... while the running total of r(n) + 16 stays <= 2048",
          "at least 2048 / (8 + 16) = 85 one-byte blocks fit; every request in the mixed sequence succeeds",
          h_case_pack);
    h_run("free all in address order",
          "size 4096, twice: alloc 24, 1, 100, 48, 8, 200, 16, 64, 33, 120, 24, 1, ... until NULL; "
          "free every block, lowest address first; alloc 4080; free it. Then alloc 2048, alloc 2000",
          "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits",
          h_case_free_forward);
    h_run("free all in reverse order",
          "same as above, but free every block highest address first",
          "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits",
          h_case_free_reverse);
    h_run("free all alternating",
          "same as above, but free the 1st, 3rd, 5th, ... block by address, then the 2nd, 4th, ...",
          "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits",
          h_case_free_alternate);
    h_run("free all shuffled",
          "same as above, but free the blocks in a seeded random order",
          "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits",
          h_case_free_shuffle);
    h_run("freed neighbours act as one space",
          "four fresh 512-byte heaps. p = alloc 37, q = alloc 20, guard alloc 8: free p, q, then alloc 64; "
          "again freeing q, p. p = alloc 16, q = alloc 40, r = alloc 24, guard alloc 8: free p, r, q, then "
          "alloc 80; again freeing q, r, p",
          "each final alloc returns p (40 + 24 = 64 and 16 + 40 + 24 = 80 fit where the neighbours were)",
          h_case_merge);
    h_run("live blocks keep their contents",
          "size 2048: alloc 24, 7, 100, 64, 1, 33, 200, 16, 48, 9, 130, 72 (blocks 0-11); free 1, 3, 2; "
          "alloc 50, 60, 8; free 6, 8, 7; alloc 250; free 0, 4, 5; alloc 1, 2, 3, 4; free 15, 10, 11, 9; "
          "alloc 300; free the rest; alloc 2032",
          "after every call, every live block still holds exactly the bytes written to it; 2032 fits at the end",
          h_case_contents);
    h_run("blocks that reach the end of the arena",
          "size 256: alloc 240, write all 240 bytes, free it; alloc 16 eight times (8 x 32 = 256); free all; "
          "alloc 240. ha fresh 256 heap: alloc 5 ten times; free all; alloc 240",
          "every request fits and nothing outside the arena changes (guard bytes on both sides are checked)",
          h_case_edges);
    h_run("smallest and largest arenas",
          "size 64: alloc 48; free; alloc 8, alloc 8; free both; alloc 48. "
          "Size 65536: alloc 65520; free; alloc 30000, 30000, 5000; free all; alloc 65520",
          "all succeed: sizes up to 65536 must work, and 64 bytes fit a 48-byte block or two 8-byte blocks",
          h_case_min_max);
    h_run("garbage before init, and re-init",
          "three 512-byte arenas filled with 0x00, 0xFF, then random bytes and fake size words, struct filled "
          "with 0xA5: init; alloc 100, 50, 8; free the 50; alloc 40; free all; alloc 496. Then free; "
          "alloc 200, 100, 64; heap_init again; alloc 496",
          "init ignores whatever the memory held; re-init discards the live blocks so 496 fits",
          h_case_garbage);
    h_run("two heaps interleaved",
          "heap a (512 bytes) and heap b (1024 bytes): a1 = alloc(a, 64), b1 = alloc(b, 64), alloc(a, 100), "
          "alloc(b, 200), free a1, alloc(b, 32), alloc(a, 32), free b1, alloc(b, 64); free all of a; "
          "alloc(a, 496); free all of b; alloc(b, 1008)",
          "each heap only hands out its own arena: alloc(a, 32) -> a1, alloc(b, 64) -> b1, and each heap "
          "fits size - 16 once emptied",
          h_case_two_heaps);
    h_run("20000 random operations",
          "size 8192, 4 rounds of 5000 seeded random calls: alloc 55% (n = 1..300) / free a random live block "
          "45%; after each round free every block in random order and alloc 8176",
          "every call keeps the rules (aligned, inside the arena, no overlap, live contents and guard bytes "
          "untouched) and 8176 fits after each round",
          h_case_random);

    nlc_end();
    return 0;
}

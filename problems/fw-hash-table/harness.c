#define _XOPEN_SOURCE 700

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

_Static_assert(HT_CAP == 64, "keep HT_CAP at 64: the tests are sized for it");

/* Every test runs under a 0.5 s timer. A call that never returns (a probe
 * loop with no exit) is cut off by SIGALRM, reported as a failed test naming
 * the call, and the remaining tests still run. */
#define H_LIMIT_US 500000
#define H_UNTOUCHED 0xDEADBEEFu
#define H_POOL 96
#define H_CHURN_OPS 20000

static sigjmp_buf h_jmp;
static unsigned h_op;
static char h_step[160];
static char h_got[640];
static const ht_t *h_tab[2];
static const char *h_tab_name[2];
static int h_ntab;

static void h_on_alarm(int sig) {
    (void)sig;
    siglongjmp(h_jmp, 1);
}

static const char *h_tf(bool b) { return b ? "true" : "false"; }

/* Small keys print in decimal, large ones in hex; the test inputs follow the
 * same rule so a failing call can be matched to its description. */
static const char *h_key(uint32_t k) {
    static char buf[4][16];
    static int next;
    char *s = buf[next++ % 4];
    if (k < 65536u) snprintf(s, sizeof buf[0], "%u", k);
    else snprintf(s, sizeof buf[0], "0x%X", k);
    return s;
}

static const char *h_name(const ht_t *t) {
    for (int i = 0; i < h_ntab; i++)
        if (h_tab[i] == t) return h_tab_name[i];
    return "t";
}

__attribute__((format(printf, 1, 2)))
static bool h_fail(const char *fmt, ...) {
    int n = snprintf(h_got, sizeof h_got, "%s ", h_step);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(h_got + n, sizeof h_got - (size_t)n, fmt, ap);
    va_end(ap);
    return false;
}

/* Fill the struct with garbage that looks like live data, then init it. */
static void h_fresh(ht_t *t, const char *name) {
    memset(t, 0xA5, sizeof *t);
    for (int i = 0; i < HT_CAP; i++) {
        t->keys[i] = (uint32_t)i * 64u;
        t->vals[i] = 0xBAD00000u + (uint32_t)i;
        t->state[i] = (uint8_t)(i % 3);
    }
    if (h_ntab < 2) {
        h_tab[h_ntab] = t;
        h_tab_name[h_ntab] = name;
        h_ntab++;
    }
    snprintf(h_step, sizeof h_step, "ht_init(%s) on a garbage-filled struct", name);
    ht_init(t);
}

static bool h_put(ht_t *t, uint32_t key, uint32_t val, bool want) {
    snprintf(h_step, sizeof h_step, "op %u: ht_put(%s, %s, %u)", ++h_op, h_name(t), h_key(key), val);
    bool got = ht_put(t, key, val);
    if (got != want) return h_fail("returned %s; expected %s", h_tf(got), h_tf(want));
    return true;
}

static bool h_del(ht_t *t, uint32_t key, bool want) {
    snprintf(h_step, sizeof h_step, "op %u: ht_del(%s, %s)", ++h_op, h_name(t), h_key(key));
    bool got = ht_del(t, key);
    if (got != want) return h_fail("returned %s; expected %s", h_tf(got), h_tf(want));
    return true;
}

static bool h_get_hit(const ht_t *t, uint32_t key, uint32_t want) {
    snprintf(h_step, sizeof h_step, "op %u: ht_get(%s, %s, &v)", ++h_op, h_name(t), h_key(key));
    uint32_t v = H_UNTOUCHED;
    bool got = ht_get(t, key, &v);
    if (!got) return h_fail("returned false; expected true with v = %u", want);
    if (v != want) return h_fail("returned true with v = %u; expected v = %u", v, want);
    return true;
}

static bool h_get_miss(const ht_t *t, uint32_t key) {
    snprintf(h_step, sizeof h_step, "op %u: ht_get(%s, %s, &v)", ++h_op, h_name(t), h_key(key));
    uint32_t v = H_UNTOUCHED;
    bool got = ht_get(t, key, &v);
    if (got) return h_fail("returned true with v = %u; expected false", v);
    if (v != H_UNTOUCHED)
        return h_fail("returned false but changed v from 0x%X to 0x%X; a miss must leave *val untouched",
                      H_UNTOUCHED, v);
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

/* 0, 1, 64, 2, 128, 3, ...: multiples of 64 interleaved with small keys. */
static uint32_t h_mixed(int i) {
    return (i % 2 == 0) ? (uint32_t)(i / 2) * 64u : (uint32_t)(i / 2) + 1u;
}

static bool h_case_basic(void) {
    static const uint32_t k[] = {1, 2, 3, 1000000, 64};
    static const uint32_t v[] = {100, 200, 300, 4, 5};
    ht_t t;
    h_fresh(&t, "t");
    for (int i = 0; i < 5; i++)
        if (!h_put(&t, k[i], v[i], true)) return false;
    for (int i = 0; i < 5; i++)
        if (!h_get_hit(&t, k[i], v[i])) return false;
    return true;
}

static bool h_case_get_missing(void) {
    ht_t t;
    h_fresh(&t, "t");
    return h_get_miss(&t, 42) && h_get_miss(&t, 0)
        && h_put(&t, 1, 10, true) && h_put(&t, 65, 20, true)
        && h_get_miss(&t, 129) && h_get_miss(&t, 2) && h_get_hit(&t, 65, 20);
}

static bool h_case_update(void) {
    ht_t t;
    h_fresh(&t, "t");
    if (!(h_put(&t, 7, 1, true) && h_put(&t, 7, 2, true) && h_get_hit(&t, 7, 2)
          && h_put(&t, 7, 3, true) && h_get_hit(&t, 7, 3)))
        return false;
    for (uint32_t j = 0; j < HT_CAP - 1; j++)
        if (!h_put(&t, 100 + j, j, true)) return false;
    return h_put(&t, 999, 9, false) && h_get_hit(&t, 7, 3) && h_get_miss(&t, 999);
}

static bool h_case_extreme_keys(void) {
    const uint32_t max = 0xFFFFFFFFu;
    ht_t t;
    h_fresh(&t, "t");
    return h_get_miss(&t, 0) && h_get_miss(&t, max)
        && h_put(&t, 0, 0, true) && h_get_hit(&t, 0, 0) && h_get_miss(&t, max)
        && h_put(&t, max, max, true) && h_get_hit(&t, max, max) && h_get_hit(&t, 0, 0)
        && h_del(&t, 0, true) && h_get_miss(&t, 0) && h_get_hit(&t, max, max)
        && h_del(&t, max, true) && h_get_miss(&t, max);
}

static bool h_case_fill(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (uint32_t j = 0; j < HT_CAP; j++)
        if (!h_put(&t, j * 64u, j, true)) return false;
    if (!(h_put(&t, 4096, 1, false) && h_get_miss(&t, 4096)
          && h_put(&t, 64, 777, true) && h_get_hit(&t, 64, 777)
          && h_put(&t, 4096, 1, false)))
        return false;
    for (uint32_t j = 0; j < HT_CAP; j++)
        if (!h_get_hit(&t, j * 64u, j == 1 ? 777u : j)) return false;
    return true;
}

static bool h_case_del_get(void) {
    ht_t t;
    h_fresh(&t, "t");
    return h_put(&t, 10, 1, true) && h_put(&t, 74, 2, true) && h_put(&t, 138, 3, true)
        && h_del(&t, 74, true) && h_get_miss(&t, 74)
        && h_get_hit(&t, 10, 1) && h_get_hit(&t, 138, 3);
}

static bool h_case_del_missing(void) {
    ht_t t;
    h_fresh(&t, "t");
    return h_del(&t, 5, false) && h_put(&t, 5, 50, true)
        && h_del(&t, 69, false) && h_del(&t, 6, false) && h_get_hit(&t, 5, 50);
}

static bool h_case_del_twice(void) {
    ht_t t;
    h_fresh(&t, "t");
    return h_put(&t, 42, 1, true) && h_del(&t, 42, true) && h_del(&t, 42, false)
        && h_get_miss(&t, 42) && h_put(&t, 42, 2, true) && h_get_hit(&t, 42, 2);
}

static bool h_case_del_every_other(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (int i = 0; i < 60; i++)
        if (!h_put(&t, h_mixed(i), 1000u + (uint32_t)i, true)) return false;
    for (int i = 0; i < 60; i += 2)
        if (!h_del(&t, h_mixed(i), true)) return false;
    for (int i = 0; i < 60; i++) {
        bool ok = (i % 2) ? h_get_hit(&t, h_mixed(i), 1000u + (uint32_t)i) : h_get_miss(&t, h_mixed(i));
        if (!ok) return false;
    }
    return true;
}

static bool h_case_reput(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (int i = 0; i < 60; i++)
        if (!h_put(&t, h_mixed(i), 1000u + (uint32_t)i, true)) return false;
    for (int i = 0; i < 30; i++)
        if (!h_del(&t, h_mixed(i), true)) return false;
    for (int i = 30; i < 60; i++)
        if (!h_put(&t, h_mixed(i), 2000u + (uint32_t)i, true)) return false;
    for (int i = 30; i < 60; i++)
        if (!h_get_hit(&t, h_mixed(i), 2000u + (uint32_t)i)) return false;
    for (int i = 30; i < 60; i++)
        if (!h_del(&t, h_mixed(i), true)) return false;
    for (int i = 30; i < 60; i++)
        if (!h_get_miss(&t, h_mixed(i))) return false;
    for (int i = 30; i < 60; i++)
        if (!h_del(&t, h_mixed(i), false)) return false;
    for (uint32_t j = 0; j < HT_CAP; j++)
        if (!h_put(&t, 0x10000u + j * 64u, j, true)) return false;
    return h_put(&t, 0x10000u + 64u * 64u, 0, false);
}

static bool h_case_full_del_insert(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (int j = 0; j < HT_CAP; j++)
        if (!h_put(&t, h_mixed(j), (uint32_t)j, true)) return false;
    if (!(h_del(&t, h_mixed(30), true) && h_put(&t, 6400, 1, true) && h_get_hit(&t, 6400, 1)))
        return false;
    for (int j = 0; j < HT_CAP; j++)
        if (j != 30 && !h_get_hit(&t, h_mixed(j), (uint32_t)j)) return false;
    return h_put(&t, 6464, 2, false) && h_get_miss(&t, h_mixed(30)) && h_get_miss(&t, 6464);
}

static bool h_case_rounds(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (uint32_t r = 0; r < 20; r++) {
        uint32_t base = (r + 1) * 0x100000u;
        for (uint32_t j = 0; j < HT_CAP; j++)
            if (!h_put(&t, base + j * 64u, r * 100u + j, true)) return false;
        if (!h_put(&t, base + 64u * 64u, 0, false)) return false;
        for (uint32_t j = 0; j < HT_CAP; j++)
            if (!h_get_hit(&t, base + j * 64u, r * 100u + j)) return false;
        for (uint32_t j = 0; j < HT_CAP; j++)
            if (!h_del(&t, base + j * 64u, true)) return false;
        for (uint32_t j = 0; j < HT_CAP; j++)
            if (!h_get_miss(&t, base + j * 64u)) return false;
    }
    return true;
}

static bool h_case_reinit(void) {
    ht_t t;
    h_fresh(&t, "t");
    for (uint32_t j = 0; j < 10; j++)
        if (!h_put(&t, j * 64u + 3u, j, true)) return false;
    snprintf(h_step, sizeof h_step, "op %u: ht_init(t) on a table holding 10 entries", ++h_op);
    ht_init(&t);
    for (uint32_t j = 0; j < 10; j++)
        if (!h_get_miss(&t, j * 64u + 3u)) return false;
    for (uint32_t j = 0; j < HT_CAP; j++)
        if (!h_put(&t, 5000u + j, j, true)) return false;
    return h_put(&t, 5000u + HT_CAP, 0, false);
}

static bool h_case_two_tables(void) {
    ht_t a, b;
    h_fresh(&a, "a");
    h_fresh(&b, "b");
    if (!(h_put(&a, 5, 1, true) && h_put(&b, 5, 2, true) && h_put(&b, 69, 3, true)
          && h_get_hit(&a, 5, 1) && h_get_hit(&b, 5, 2) && h_get_miss(&a, 69)
          && h_del(&a, 5, true) && h_get_miss(&a, 5) && h_get_hit(&b, 5, 2)))
        return false;
    for (uint32_t j = 0; j < HT_CAP; j++)
        if (!h_put(&a, 3000u + j, j, true)) return false;
    return h_put(&b, 3000, 9, true) && h_get_hit(&b, 3000, 9) && h_get_hit(&a, 3000, 0);
}

/* Long random workload checked against a shadow model of the map. */
struct h_slot {
    uint32_t key;
    uint32_t val;
    bool live;
    bool by_put;
    unsigned changed;
};

static struct h_slot h_model[H_POOL];
static unsigned h_live;
static uint32_t h_rng;

static uint32_t h_rand(void) {
    h_rng ^= h_rng << 13;
    h_rng ^= h_rng >> 17;
    h_rng ^= h_rng << 5;
    return h_rng;
}

static uint32_t h_pool_key(int i) {
    if (i < 32) return (uint32_t)i * 64u;
    if (i < 48) return (uint32_t)(i - 31);
    if (i < 64) return 0xFFFFFFFFu - (uint32_t)(i - 48) * 64u;
    if (i < 80) return (uint32_t)(i - 63) * 4096u;
    return (uint32_t)(i - 79) << 24;
}

static bool h_churn_fail(const struct h_slot *m) {
    size_t n = strlen(h_got);
    if (!m->changed)
        snprintf(h_got + n, sizeof h_got - n, " [model: %u live entries; key %s was never stored]",
                 h_live, h_key(m->key));
    else if (m->by_put)
        snprintf(h_got + n, sizeof h_got - n, " [model: %u live entries; key %s last put with v = %u at op %u]",
                 h_live, h_key(m->key), m->val, m->changed);
    else
        snprintf(h_got + n, sizeof h_got - n, " [model: %u live entries; key %s last deleted at op %u]",
                 h_live, h_key(m->key), m->changed);
    return false;
}

static bool h_case_churn(void) {
    ht_t t;
    h_fresh(&t, "t");
    h_rng = 0x2545F491u;
    h_live = 0;
    for (int i = 0; i < H_POOL; i++)
        h_model[i] = (struct h_slot){h_pool_key(i), 0, false, false, 0};
    for (int n = 0; n < H_CHURN_OPS; n++) {
        uint32_t r = h_rand() % 100u;
        struct h_slot *m = &h_model[h_rand() % H_POOL];
        bool ok;
        if (r < 45) {
            uint32_t val = h_rand();
            bool want = m->live || h_live < HT_CAP;
            ok = h_put(&t, m->key, val, want);
            if (ok && want) {
                if (!m->live) h_live++;
                *m = (struct h_slot){m->key, val, true, true, h_op};
            }
        } else if (r < 75) {
            ok = h_del(&t, m->key, m->live);
            if (ok && m->live) {
                h_live--;
                *m = (struct h_slot){m->key, 0, false, false, h_op};
            }
        } else {
            ok = m->live ? h_get_hit(&t, m->key, m->val) : h_get_miss(&t, m->key);
        }
        if (!ok) return h_churn_fail(m);
    }
    for (int i = 0; i < H_POOL; i++) {
        const struct h_slot *m = &h_model[i];
        bool ok = m->live ? h_get_hit(&t, m->key, m->val) : h_get_miss(&t, m->key);
        if (!ok) return h_churn_fail(m);
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

    h_run("put and get a few keys",
          "put (1,100) (2,200) (3,300) (1000000,4) (64,5); get each key",
          "every put returns true; every get returns true with that key's value",
          h_case_basic);
    h_run("get a missing key",
          "empty table: get 42, get 0; put (1,10) (65,20); get 129, get 2, get 65",
          "gets of absent keys return false and leave v untouched; get 65 returns 20",
          h_case_get_missing);
    h_run("update an existing key",
          "put (7,1) (7,2); get 7; put (7,3); get 7; put 63 new keys 100..162 (value j); "
          "put one more new key (999,9); get 7, get 999",
          "updates return true and take no extra room: the 63 new keys fit, 999 is rejected "
          "and absent, get 7 returns 3",
          h_case_update);
    h_run("keys 0 and 0xFFFFFFFF",
          "empty table: get 0, get 0xFFFFFFFF; put (0,0), get 0, get 0xFFFFFFFF; "
          "put (0xFFFFFFFF,4294967295), get both; del 0, get both; del 0xFFFFFFFF, get it",
          "0 and 0xFFFFFFFF behave like any other key: absent until put, present after, gone after del",
          h_case_extreme_keys);
    h_run("fill to HT_CAP",
          "put 64 keys 0, 64, 128, ..., 4032 (key j*64, value j); put (4096,1); get 4096; "
          "put (64,777); get 64; put (4096,1) again; get all 64 keys",
          "all 64 puts succeed; key 4096 is rejected and absent; updating 64 on the full table succeeds",
          h_case_fill);
    h_run("delete then get",
          "put (10,1) (74,2) (138,3); del 74; get 74, get 10, get 138",
          "del 74 returns true; get 74 misses; get 10 and get 138 return 1 and 3",
          h_case_del_get);
    h_run("delete a missing key",
          "empty table: del 5; put (5,50); del 69, del 6; get 5",
          "deleting absent keys returns false and changes nothing; get 5 returns 50",
          h_case_del_missing);
    h_run("delete twice",
          "put (42,1); del 42; del 42; get 42; put (42,2); get 42",
          "first del true, second del false, get misses, re-put works and get returns 2",
          h_case_del_twice);
    h_run("delete every other of 60 keys",
          "put 60 keys 0, 1, 64, 2, 128, 3, ... (key i is (i/2)*64 for even i, i/2+1 for odd i; "
          "value 1000+i); del the even-i keys 0, 64, 128, ...; get all 60",
          "the 30 odd-i keys still return 1000+i; the 30 deleted keys are absent",
          h_case_del_every_other);
    h_run("re-put after earlier deletes",
          "put the same 60 keys (value 1000+i); del keys i=0..29; put keys i=30..59 again with "
          "value 2000+i; get them; del each once; get each; del each again; "
          "then put 64 new keys 0x10000 + j*64 and one more",
          "re-puts update in place: gets return 2000+i, one del removes each key for good "
          "(no stale 1000+i copy), the table ends empty so 64 new keys fit and the 65th is rejected",
          h_case_reput);
    h_run("full table, delete one, insert new",
          "put 64 keys 0, 1, 64, 2, ... (same pattern, value i); del 960 (i=30); put (6400,1); get 6400; "
          "get the other 63 keys; put (6464,2); get 960, get 6464",
          "the new key fits in the freed room and the rest are intact; the table is full again, "
          "so 6464 is rejected",
          h_case_full_del_insert);
    h_run("fill and empty 20 times",
          "20 rounds on one table: put 64 keys base + j*64 (base = 0x100000 * round number); "
          "put one more; get all 64; del all 64; get all 64",
          "every round: 64 keys fit, the 65th is rejected, all are found, all delete, all then miss",
          h_case_rounds);
    h_run("init clears a used table",
          "put 10 keys 3, 67, 131, ..., 579; ht_init(t) again; get those 10; "
          "put 64 new keys 5000..5063 and one more (5064)",
          "after re-init the old keys are absent and all 64 slots are available",
          h_case_reinit);
    h_run("two tables are independent",
          "tables a and b: put a(5,1), b(5,2), b(69,3); get a 5, b 5, a 69; del a 5; get a 5, b 5; "
          "fill a with keys 3000..3063 (value j); put b(3000,9); get b 3000, a 3000",
          "each table keeps its own entries and its own capacity",
          h_case_two_tables);
    h_run("20000 random operations",
          "seeded random put 45% / del 30% / get 25% over 96 keys that collide under simple hashes "
          "(multiples of 64 and 4096, 1..16, 0xFFFFFFFF downward in steps of 64, 0x1000000 multiples); "
          "puts of new keys on a full table expect false; then get all 96",
          "every result matches a reference model of the map",
          h_case_churn);

    nlc_end();
    return 0;
}

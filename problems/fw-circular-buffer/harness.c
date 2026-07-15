#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nlc.h"
#include "solution.c"

static void chk_int(const char *name, const char *input, int got, int exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

static void chk_bool(const char *name, const char *input, bool got, bool exp) {
    nlc_case(name, got == exp, input,
             exp ? "true" : "false",
             got ? "true" : "false", "");
}

int main(void) {
    CircularBuffer cb;
    int v;

    nlc_begin();

    /* --- fresh buffer --- */
    cb_init(&cb);
    chk_int ("init: size == 0",  "cb_init(); cb_size()",    cb_size(&cb),     0);
    chk_bool("init: is_empty",   "cb_init(); cb_is_empty()", cb_is_empty(&cb), true);
    chk_bool("init: !is_full",   "cb_init(); cb_is_full()",  cb_is_full(&cb),  false);

    /* --- ops on empty buffer --- */
    chk_int("empty: pop returns -1",  "cb_pop() on empty",  cb_pop(&cb, &v),  -1);
    chk_int("empty: peek returns -1", "cb_peek() on empty", cb_peek(&cb, &v), -1);

    /* --- push → peek → size (peek must not consume) --- */
    chk_int("push 10 succeeds", "cb_push(10)", cb_push(&cb, 10), 0);
    chk_int("size is 1", "cb_size()", cb_size(&cb), 1);
    {
        int pv = 0;
        int pr = cb_peek(&cb, &pv);
        char got[32];
        if (pr == 0) snprintf(got, sizeof got, "0, *out=%d", pv);
        else         snprintf(got, sizeof got, "-1");
        nlc_case("peek returns 0, *out=10", pr == 0 && pv == 10,
                 "cb_peek(&v)", "0, *out=10", got, "");
    }
    chk_int("peek didn't consume (size still 1)", "cb_size() after cb_peek()", cb_size(&cb), 1);

    /* --- FIFO ordering --- */
    cb_push(&cb, 20);
    cb_push(&cb, 30);
    {
        int v1 = 0, v2 = 0, v3 = 0;
        int r1 = cb_pop(&cb, &v1);
        int r2 = cb_pop(&cb, &v2);
        int r3 = cb_pop(&cb, &v3);
        int ok = r1 == 0 && v1 == 10 && r2 == 0 && v2 == 20 && r3 == 0 && v3 == 30;
        const char *got = ok ? "10 20 30"
                        : (r1 == 0 && r2 == 0 && r3 == 0) ? "wrong values" : "pop failed";
        nlc_case("FIFO: pop order 10, 20, 30", ok,
                 "push(10,20,30); pop x3", "10 20 30", got, "");
    }
    chk_bool("empty after 3 pops", "cb_is_empty()", cb_is_empty(&cb), true);

    /* --- fill to capacity; push must reject when full --- */
    cb_init(&cb);
    for (int i = 0; i < CB_CAPACITY; i++) cb_push(&cb, i);
    chk_bool("full after CB_CAPACITY pushes", "push x8; is_full()", cb_is_full(&cb), true);
    chk_int ("push when full returns -1", "cb_push(999) when full", cb_push(&cb, 999), -1);
    chk_int ("size unchanged after rejected push", "cb_size()", cb_size(&cb), CB_CAPACITY);

    /* --- contents intact after rejected push --- */
    {
        int ok = 1;
        for (int i = 0; i < CB_CAPACITY; i++) {
            int pv = 0;
            if (cb_pop(&cb, &pv) != 0 || pv != i) { ok = 0; break; }
        }
        nlc_case("contents intact after rejected push", ok,
                 "pop all after failed push; expect 0..7",
                 "0 1 2 3 4 5 6 7", ok ? "0 1 2 3 4 5 6 7" : "wrong", "");
    }
    chk_bool("empty after drain", "cb_is_empty()", cb_is_empty(&cb), true);

    /* --- wrap-around: head and tail must wrap correctly --- */
    cb_init(&cb);
    for (int i = 0; i < 5; i++) cb_push(&cb, i);
    for (int i = 0; i < 3; i++) { int pv; cb_pop(&cb, &pv); }
    for (int i = 5; i < 5 + (CB_CAPACITY - 2); i++) cb_push(&cb, i);
    chk_bool("wrap: full after wrap-around", "push5/pop3/push6 (tail wraps); is_full()", cb_is_full(&cb), true);
    {
        int ok = 1;
        for (int i = 3; i < 3 + CB_CAPACITY; i++) {
            int pv = 0;
            if (cb_pop(&cb, &pv) != 0 || pv != i) { ok = 0; break; }
        }
        nlc_case("wrap: FIFO correct after wrap-around", ok,
                 "pop all; expect 3..10",
                 "3 4 5 6 7 8 9 10", ok ? "3 4 5 6 7 8 9 10" : "wrong", "");
    }

    /* --- stress: 1000 rounds of push-3 / pop-2 --- */
    cb_init(&cb);
    {
        int next_push = 0, next_expect = 0, ok = 1;
        for (int round = 0; round < 1000 && ok; round++) {
            for (int k = 0; k < 3; k++)
                if (cb_push(&cb, next_push) == 0) next_push++;
            for (int k = 0; k < 2; k++) {
                int pv;
                if (cb_pop(&cb, &pv) == 0) {
                    if (pv != next_expect) { ok = 0; break; }
                    next_expect++;
                }
            }
        }
        nlc_case("stress: 1000×push-3/pop-2 FIFO holds", ok,
                 "1000 rounds of push-3/pop-2",
                 "FIFO holds", ok ? "FIFO holds" : "FIFO broken", "");
    }

    nlc_end();
    return 0;
}

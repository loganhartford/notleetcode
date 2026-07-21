#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nlc.h"
#include "solution.c"

/* Run one case: build inputs, call balanceCurrent, compare limits[] to expected.
 * Also independently verifies the safety invariant (sum <= site_limit, each in
 * [0, max_charger], ineligible chargers == 0) so a wrong-but-lucky array fails. */
static void run(const char *name, int n, const bool *car, const bool *online,
                int site_limit, int max_charger, const int *expected) {
    int limits[64];
    for (int i = 0; i < n; i++) limits[i] = -999;

    balanceCurrent(n, car, online, site_limit, max_charger, limits);

    int ok = 1, sum = 0;
    for (int i = 0; i < n; i++) {
        if (limits[i] != expected[i]) ok = 0;
        sum += limits[i];
        if (limits[i] < 0 || limits[i] > max_charger) ok = 0;
        if (!(online[i] && car[i]) && limits[i] != 0) ok = 0;
    }
    if (sum > site_limit) ok = 0;

    char gbuf[256], ebuf[256], input[256];
    nlc_fmt_ints(gbuf, sizeof gbuf, limits, n);
    nlc_fmt_ints(ebuf, sizeof ebuf, expected, n);
    snprintf(input, sizeof input, "n=%d, site_limit=%d, max_charger=%d",
             n, site_limit, max_charger);
    nlc_case(name, ok, input, ebuf, gbuf, "");
}

int main(void) {
    nlc_begin();

    /* Example 1: 10 chargers, all eligible, splits evenly. */
    {
        bool car[10], on[10];
        int exp[10];
        for (int i = 0; i < 10; i++) { car[i] = true; on[i] = true; exp[i] = 40; }
        run("example 1: 400 A split 10 ways", 10, car, on, 400, 50, exp);
    }

    /* Example 2: offline chargers excluded, breaker never hit. */
    {
        bool car[4] = {true, true, true, true};
        bool on[4]  = {true, false, true, false};
        int  exp[4] = {50, 0, 50, 0};
        run("example 2: offline chargers get 0", 4, car, on, 200, 50, exp);
    }

    /* Example 3: remainder amps go to lowest-indexed eligible chargers. */
    {
        bool car[4] = {true, true, true, true};
        bool on[4]  = {true, true, true, true};
        int  exp[4] = {3, 3, 2, 2};
        run("example 3: leftover amps to low index", 4, car, on, 10, 50, exp);
    }

    /* Per-charger cap: 3 chargers, big budget -> each capped at max_charger. */
    {
        bool car[3] = {true, true, true};
        bool on[3]  = {true, true, true};
        int  exp[3] = {50, 50, 50};
        run("per-charger cap: each limited to 50", 3, car, on, 400, 50, exp);
    }

    /* No cars: everything zero. */
    {
        bool car[5] = {false, false, false, false, false};
        bool on[5]  = {true, true, true, true, true};
        int  exp[5] = {0, 0, 0, 0, 0};
        run("no cars: all zero", 5, car, on, 400, 50, exp);
    }

    /* Mixed car + online: eligible = online AND car. */
    {
        bool car[6] = {true, false, true, true, false, true};
        bool on[6]  = {true, true,  true, false, true, true};
        /* eligible: 0, 2, 5 -> k=3, total=min(90,150)=90, base=30, rem=0 */
        int  exp[6] = {30, 0, 30, 0, 0, 30};
        run("mixed car/online eligibility", 6, car, on, 90, 50, exp);
    }

    /* Single eligible charger: gets min(site_limit, max_charger). */
    {
        bool car[3] = {false, true, false};
        bool on[3]  = {true, true, true};
        int  exp[3] = {0, 50, 0};
        run("single eligible: min(site,max)", 3, car, on, 400, 50, exp);
    }

    /* site_limit smaller than eligible count: everyone rounds down, some 0. */
    {
        bool car[5], on[5];
        int  exp[5] = {1, 1, 0, 0, 0};  /* total=2, base=0, rem=2 -> C0,C1 get 1 */
        for (int i = 0; i < 5; i++) { car[i] = true; on[i] = true; }
        run("tiny budget: remainder only", 5, car, on, 2, 50, exp);
    }

    /* site_limit == 0: all zero even with eligible chargers. */
    {
        bool car[3] = {true, true, true};
        bool on[3]  = {true, true, true};
        int  exp[3] = {0, 0, 0};
        run("zero site budget", 3, car, on, 0, 50, exp);
    }

    /* n == 0: nothing to do, no crash. */
    {
        int exp[1] = {0};
        run("empty site (n=0)", 0, NULL, NULL, 400, 50, exp);
    }

    /* Uneven split with cars and offline mixed. */
    {
        bool car[8], on[8];
        for (int i = 0; i < 8; i++) { car[i] = true; on[i] = true; }
        on[3] = false; on[6] = false;   /* eligible: 6 chargers */
        /* total=min(100,300)=100, base=16, rem=4 -> C0,C1,C2,C4 get 17 */
        int exp[8] = {17, 17, 17, 0, 17, 16, 0, 16};
        run("uneven split, two offline", 8, car, on, 100, 50, exp);
    }

    nlc_end();
    return 0;
}

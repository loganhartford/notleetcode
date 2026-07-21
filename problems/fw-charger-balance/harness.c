#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nlc.h"
#include "solution.c"

/* Run one case: build inputs, call balanceCurrent, compare limits[] to expected.
 * Also independently verifies the safety invariants (sum <= site_limit, each in
 * [0, max_charger], offline chargers == safe_min, online-but-no-car == 0) so a
 * wrong-but-lucky array still fails.
 *
 * An offline charger's status read fails, so car_present is forced to false here —
 * you can never observe an offline charger reporting a car. */
static void run(const char *name, int n, const bool *car, const bool *online,
                int site_limit, int max_charger, const int *expected) {
    bool car_read[64];
    for (int i = 0; i < n; i++) car_read[i] = online[i] && car[i];

    int limits[64];
    for (int i = 0; i < n; i++) limits[i] = -999;

    balanceCurrent(n, car_read, online, site_limit, max_charger, limits);

    int safe_min = n > 0 ? site_limit / n : 0;
    if (safe_min > max_charger) safe_min = max_charger;

    int ok = 1, sum = 0;
    for (int i = 0; i < n; i++) {
        if (limits[i] != expected[i]) ok = 0;
        sum += limits[i];
        if (limits[i] < 0 || limits[i] > max_charger) ok = 0;
        if (!online[i] && limits[i] != safe_min) ok = 0;         /* offline -> safe_min */
        if (online[i] && !car_read[i] && limits[i] != 0) ok = 0; /* online, no car -> 0 */
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

    /* Example 1: all reachable, splits evenly, no reservation. */
    {
        bool car[10], on[10];
        int exp[10];
        for (int i = 0; i < 10; i++) { car[i] = true; on[i] = true; exp[i] = 40; }
        run("example 1: 400 A split 10 ways", 10, car, on, 400, 50, exp);
    }

    /* Example 2: one offline charger reserved at safe_min, one online idle. */
    {
        bool car[4] = {true, true, false, false};   /* C3 offline -> car forced false */
        bool on[4]  = {true, true, true, false};
        /* safe_min=30, reserve 30 for offline C3, budget 90 -> C0,C1 get 45; C2 no car -> 0 */
        int  exp[4] = {45, 45, 0, 30};
        run("example 2: offline reserved at safe_min", 4, car, on, 120, 50, exp);
    }

    /* Example 3: remainder amps go to lowest-indexed eligible chargers. */
    {
        bool car[4] = {true, true, true, true};
        bool on[4]  = {true, true, true, true};
        int  exp[4] = {3, 3, 2, 2};
        run("example 3: leftover amps to low index", 4, car, on, 10, 50, exp);
    }

    /* Offline reservation frees less for eligible: safe_min=30, budget shared. */
    {
        bool car[6] = {true, true, false, false, true, false};
        bool on[6]  = {true, true, false, false, true, true};
        /* offline C2,C3 -> reserve 30 each (60). budget=120. eligible C0,C1,C4 -> 40.
         * C5 online no car -> 0. */
        int  exp[6] = {40, 40, 30, 30, 40, 0};
        run("offline reservation shrinks budget", 6, car, on, 180, 50, exp);
    }

    /* Per-charger cap: 3 chargers, big budget -> each capped at max_charger. */
    {
        bool car[3] = {true, true, true};
        bool on[3]  = {true, true, true};
        int  exp[3] = {50, 50, 50};
        run("per-charger cap: each limited to 50", 3, car, on, 400, 50, exp);
    }

    /* No cars, all online: everything zero (safe_min never applied). */
    {
        bool car[5] = {false, false, false, false, false};
        bool on[5]  = {true, true, true, true, true};
        int  exp[5] = {0, 0, 0, 0, 0};
        run("no cars: all zero", 5, car, on, 400, 50, exp);
    }

    /* All offline: every charger falls back to safe_min, breaker not exceeded. */
    {
        bool car[4] = {false, false, false, false};
        bool on[4]  = {false, false, false, false};
        int  exp[4] = {40, 40, 40, 40};   /* safe_min = 160/4 = 40 */
        run("all offline: each at safe_min", 4, car, on, 160, 50, exp);
    }

    /* Mixed car + online: eligibility is online AND car; offline still reserved. */
    {
        bool car[6] = {true, false, true, false, false, true};  /* C3 offline -> false */
        bool on[6]  = {true, true,  true, false, true,  true};
        /* safe_min=15, offline C3 -> 15, budget=75, eligible C0,C2,C5 -> 25 each */
        int  exp[6] = {25, 0, 25, 15, 0, 25};
        run("mixed car/online eligibility", 6, car, on, 90, 50, exp);
    }

    /* Single eligible charger, all online: gets min(site_limit, max_charger). */
    {
        bool car[3] = {false, true, false};
        bool on[3]  = {true, true, true};
        int  exp[3] = {0, 50, 0};
        run("single eligible: min(site,max)", 3, car, on, 400, 50, exp);
    }

    /* Tiny budget: safe_min=0, remainder amps only. */
    {
        bool car[5], on[5];
        int  exp[5] = {1, 1, 0, 0, 0};  /* budget=2, base=0, rem=2 -> C0,C1 get 1 */
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

    /* Uneven split with two offline chargers reserved at safe_min. */
    {
        bool car[8], on[8];
        for (int i = 0; i < 8; i++) { car[i] = true; on[i] = true; }
        on[3] = false; on[6] = false;
        car[3] = false; car[6] = false;   /* offline -> car forced false */
        /* safe_min=12, offline C3,C6 -> 12 each (24), budget=76, eligible=6 chargers.
         * base=12, rem=4 -> C0,C1,C2,C4 get 13; C5,C7 get 12. */
        int exp[8] = {13, 13, 13, 12, 13, 12, 12, 12};
        run("uneven split, two offline reserved", 8, car, on, 100, 50, exp);
    }

    nlc_end();
    return 0;
}

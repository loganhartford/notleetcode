#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TS_MAX_TIMERS 8
typedef void (*timer_cb_t)(void *ctx);
typedef struct {
    bool       active;
    bool       periodic;
    uint32_t   deadline;
    uint32_t   period;
    timer_cb_t cb;
    void      *ctx;
} timer_slot_t;
typedef struct {
    timer_slot_t slots[TS_MAX_TIMERS];
} timer_sched_t;

#include "nlc.h"
#include "solution.c"

static int g_fired;
static void bump_cb(void *ctx) { (void)ctx; g_fired++; }

static int g_ctx_val;
static void ctx_cb(void *ctx) { g_ctx_val = *(int*)ctx; }

static void chk_int(const char *nm, const char *in, int got, int exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    timer_sched_t ts;

    /* single one-shot timer */
    ts_init(&ts);
    g_fired = 0;
    int id = ts_start(&ts, 1000, 50, false, bump_cb, NULL);
    chk_int("ts_start returns id>=0", "ts_start", id >= 0 ? 0 : -1, 0);
    ts_poll(&ts, 1049); chk_int("not yet", "poll 1049", g_fired, 0);
    ts_poll(&ts, 1050); chk_int("fires at 1050", "poll 1050", g_fired, 1);
    ts_poll(&ts, 1100); chk_int("no refire", "poll 1100", g_fired, 1);

    /* periodic timer */
    ts_init(&ts);
    g_fired = 0;
    id = ts_start(&ts, 0, 100, true, bump_cb, NULL);
    ts_poll(&ts, 100); chk_int("periodic fire 1", "poll 100", g_fired, 1);
    ts_poll(&ts, 200); chk_int("periodic fire 2", "poll 200", g_fired, 2);
    ts_poll(&ts, 250); chk_int("periodic no extra", "poll 250", g_fired, 2);
    ts_poll(&ts, 300); chk_int("periodic fire 3", "poll 300", g_fired, 3);

    /* cancel prevents fire */
    ts_init(&ts);
    g_fired = 0;
    id = ts_start(&ts, 0, 100, false, bump_cb, NULL);
    ts_cancel(&ts, id);
    ts_poll(&ts, 200); chk_int("cancelled no fire", "cancel then poll", g_fired, 0);

    /* cancel invalid id */
    chk_bool("cancel -1 false", "ts_cancel(-1)", ts_cancel(&ts, -1), false);
    chk_bool("cancel oob false", "ts_cancel(99)", ts_cancel(&ts, 99), false);

    /* multiple simultaneous timers */
    ts_init(&ts);
    g_fired = 0;
    ts_start(&ts, 0, 10, false, bump_cb, NULL);
    ts_start(&ts, 0, 20, false, bump_cb, NULL);
    ts_start(&ts, 0, 30, false, bump_cb, NULL);
    ts_poll(&ts, 10); chk_int("multi: 1 fired", "poll 10", g_fired, 1);
    ts_poll(&ts, 20); chk_int("multi: 2 fired", "poll 20", g_fired, 2);
    ts_poll(&ts, 30); chk_int("multi: 3 fired", "poll 30", g_fired, 3);

    /* wraparound near UINT32_MAX */
    ts_init(&ts);
    g_fired = 0;
    ts_start(&ts, 0xFFFFFFF0u, 32u, false, bump_cb, NULL);
    /* deadline = 0xFFFFFFF0 + 32 = 0x00000010 */
    ts_poll(&ts, 0x0000000Fu); chk_int("wrap: not yet", "wrap poll 0x0F", g_fired, 0);
    ts_poll(&ts, 0x00000010u); chk_int("wrap: fires", "wrap poll 0x10", g_fired, 1);

    nlc_end();
    return 0;
}

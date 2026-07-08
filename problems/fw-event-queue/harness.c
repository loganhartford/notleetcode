#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}
static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_event(const char *nm, const char *in,
                       event_t got, uint16_t eid, uint32_t earg) {
    char g[32], e[32];
    snprintf(g, sizeof g, "{%u,%u}", got.id, (unsigned)got.arg);
    snprintf(e, sizeof e, "{%u,%u}", eid,    (unsigned)earg);
    nlc_case(nm, got.id == eid && got.arg == earg, in, e, g, "");
}

int main(void) {
    nlc_begin();

    event_t storage[8];
    event_queue_t q;

    /* FIFO order */
    eq_init(&q, storage, 4);
    chk_bool("push E1", "eq_push {1,100}", eq_push(&q, (event_t){1,100}), true);
    chk_bool("push E2", "eq_push {2,200}", eq_push(&q, (event_t){2,200}), true);
    chk_sz("count=2", "eq_count after 2 push", eq_count(&q), 2);
    event_t e;
    chk_bool("pop ok", "eq_pop", eq_pop(&q, &e), true);
    chk_event("FIFO first", "popped event", e, 1, 100);
    chk_bool("pop ok2", "eq_pop", eq_pop(&q, &e), true);
    chk_event("FIFO second", "popped event2", e, 2, 200);
    chk_bool("pop empty", "eq_pop empty", eq_pop(&q, &e), false);

    /* fill and overflow */
    eq_init(&q, storage, 3);
    eq_push(&q, (event_t){10,1}); eq_push(&q, (event_t){20,2}); eq_push(&q, (event_t){30,3});
    chk_bool("overflow false", "eq_push when full", eq_push(&q, (event_t){40,4}), false);
    chk_sz("count still 3", "eq_count after overflow", eq_count(&q), 3);

    /* wraparound */
    eq_init(&q, storage, 4);
    eq_push(&q, (event_t){1,0}); eq_push(&q, (event_t){2,0}); eq_push(&q, (event_t){3,0});
    eq_pop(&q, &e);  /* remove 1 */
    eq_push(&q, (event_t){4,0}); /* wrap */
    eq_pop(&q, &e); chk_event("wrap pop B", "pop after wrap", e, 2, 0);
    eq_pop(&q, &e); chk_event("wrap pop C", "pop after wrap", e, 3, 0);
    eq_pop(&q, &e); chk_event("wrap pop D", "pop after wrap", e, 4, 0);

    /* interleaved push/pop */
    eq_init(&q, storage, 4);
    eq_push(&q, (event_t){7, 77});
    eq_pop(&q, &e);
    eq_push(&q, (event_t){8, 88});
    chk_sz("interleaved count=1", "eq_count interleaved", eq_count(&q), 1);
    eq_pop(&q, &e);
    chk_event("interleaved event", "interleaved pop", e, 8, 88);

    nlc_end();
    return 0;
}

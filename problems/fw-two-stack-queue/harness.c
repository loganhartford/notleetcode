#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TSQ_CAP 16
typedef struct {
    int32_t inbox[TSQ_CAP];
    int32_t outbox[TSQ_CAP];
    size_t  in_top;
    size_t  out_top;
} two_stack_queue_t;

#include "nlc.h"
#include "solution.c"

static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}
static void chk_i32(const char *nm, const char *in, int32_t got, int32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", (int)got);
    snprintf(e, sizeof e, "%d", (int)exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    two_stack_queue_t q;
    memset(&q, 0, sizeof q);
    int32_t v;

    /* FIFO order */
    tsq_push(&q, 1); tsq_push(&q, 2); tsq_push(&q, 3);
    chk_bool("pop ok 1", "tsq_pop", tsq_pop(&q, &v), true);
    chk_i32("FIFO pop=1", "v", v, 1);
    chk_bool("pop ok 2", "tsq_pop", tsq_pop(&q, &v), true);
    chk_i32("FIFO pop=2", "v", v, 2);
    chk_bool("pop ok 3", "tsq_pop", tsq_pop(&q, &v), true);
    chk_i32("FIFO pop=3", "v", v, 3);

    /* interleaved push/pop */
    memset(&q, 0, sizeof q);
    tsq_push(&q, 10); tsq_push(&q, 20);
    tsq_pop(&q, &v); chk_i32("interleave pop=10", "pop after 10,20", v, 10);
    tsq_push(&q, 30);
    tsq_pop(&q, &v); chk_i32("interleave pop=20", "pop", v, 20);
    tsq_pop(&q, &v); chk_i32("interleave pop=30", "pop", v, 30);

    /* drain outbox then refill */
    memset(&q, 0, sizeof q);
    tsq_push(&q, 100); tsq_push(&q, 200);
    tsq_pop(&q, &v);  /* transfer, pop 100 */
    tsq_pop(&q, &v);  /* pop 200 from outbox */
    tsq_push(&q, 300);
    tsq_pop(&q, &v);  /* transfer, pop 300 */
    chk_i32("drain refill pop=300", "pop after drain+refill", v, 300);

    /* overflow */
    memset(&q, 0, sizeof q);
    for (int i = 0; i < TSQ_CAP; i++) tsq_push(&q, i);
    chk_bool("overflow false", "tsq_push when full", tsq_push(&q, 999), false);

    /* underflow */
    memset(&q, 0, sizeof q);
    chk_bool("underflow false", "tsq_pop when empty", tsq_pop(&q, &v), false);

    nlc_end();
    return 0;
}

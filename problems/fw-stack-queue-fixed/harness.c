#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int32_t *buf; size_t cap; size_t count; } fwstack_t;
#define FW_QUEUE_CAP 16
typedef struct { int32_t buf[FW_QUEUE_CAP]; size_t head; size_t count; } queue32_t;

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
static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* Stack LIFO */
    int32_t sbuf[4];
    fwstack_t s = { sbuf, 4, 0 };
    int32_t v;
    chk_bool("push 1", "stack_push(1)", stack_push(&s, 1), true);
    chk_bool("push 2", "stack_push(2)", stack_push(&s, 2), true);
    chk_bool("push 3", "stack_push(3)", stack_push(&s, 3), true);
    chk_bool("pop ok", "stack_pop", stack_pop(&s, &v), true);
    chk_i32("LIFO pop=3", "popped", v, 3);
    chk_bool("pop 2", "stack_pop", stack_pop(&s, &v), true);
    chk_i32("LIFO pop=2", "popped", v, 2);

    /* Stack overflow */
    s.count = 0;
    stack_push(&s, 1); stack_push(&s, 2); stack_push(&s, 3); stack_push(&s, 4);
    chk_bool("stack overflow", "stack_push when full", stack_push(&s, 5), false);

    /* Stack underflow */
    s.count = 0;
    chk_bool("stack underflow", "stack_pop when empty", stack_pop(&s, &v), false);

    /* Queue FIFO */
    queue32_t q = {{0}, 0, 0};
    chk_bool("q push 10", "q32_push(10)", q32_push(&q, 10), true);
    chk_bool("q push 20", "q32_push(20)", q32_push(&q, 20), true);
    chk_bool("q push 30", "q32_push(30)", q32_push(&q, 30), true);
    chk_sz("q count=3", "q32_count", q32_count(&q), 3);
    chk_bool("q pop ok", "q32_pop", q32_pop(&q, &v), true);
    chk_i32("FIFO pop=10", "q32_pop val", v, 10);
    chk_bool("q pop ok2", "q32_pop", q32_pop(&q, &v), true);
    chk_i32("FIFO pop=20", "q32_pop val", v, 20);

    /* Queue wraparound */
    queue32_t q2 = {{0}, 0, 0};
    for (int i = 0; i < FW_QUEUE_CAP - 2; i++) q32_push(&q2, i);
    for (int i = 0; i < FW_QUEUE_CAP - 2; i++) q32_pop(&q2, &v);
    q32_push(&q2, 99); q32_push(&q2, 100);
    q32_pop(&q2, &v); chk_i32("wraparound pop 99", "wraparound q pop", v, 99);

    /* Queue overflow */
    queue32_t q3 = {{0}, 0, 0};
    for (int i = 0; i < FW_QUEUE_CAP; i++) q32_push(&q3, i);
    chk_bool("q overflow", "q32_push when full", q32_push(&q3, 999), false);

    /* Queue underflow */
    queue32_t q4 = {{0}, 0, 0};
    chk_bool("q underflow", "q32_pop when empty", q32_pop(&q4, &v), false);

    nlc_end();
    return 0;
}

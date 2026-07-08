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
static void chk_byte(const char *nm, const char *in, uint8_t got, uint8_t exp) {
    char g[8], e[8];
    snprintf(g, sizeof g, "0x%02X", got);
    snprintf(e, sizeof e, "0x%02X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    uint8_t storage[8];
    ringbuf_t rb;

    /* empty pop fails */
    rb_init(&rb, storage, 4);
    uint8_t v = 0;
    chk_bool("empty pop false", "rb_pop on empty", rb_pop(&rb, &v), false);
    chk_sz("empty available", "rb_available empty", rb_available(&rb), 0);
    chk_sz("empty free_space", "rb_free_space empty", rb_free_space(&rb), 4);

    /* push and pop */
    chk_bool("push 0xAA", "rb_push(0xAA)", rb_push(&rb, 0xAA), true);
    chk_bool("push 0xBB", "rb_push(0xBB)", rb_push(&rb, 0xBB), true);
    chk_sz("available=2", "rb_available after 2 pushes", rb_available(&rb), 2);
    chk_sz("free=2", "rb_free_space after 2 pushes", rb_free_space(&rb), 2);
    chk_bool("pop ok", "rb_pop", rb_pop(&rb, &v), true);
    chk_byte("FIFO 0xAA", "popped value", v, 0xAA);

    /* fill to capacity */
    rb_init(&rb, storage, 4);
    rb_push(&rb, 1); rb_push(&rb, 2); rb_push(&rb, 3); rb_push(&rb, 4);
    chk_sz("full available=4", "rb_available full", rb_available(&rb), 4);
    chk_sz("full free=0", "rb_free_space full", rb_free_space(&rb), 0);
    chk_bool("overflow fails", "rb_push when full", rb_push(&rb, 5), false);

    /* wraparound */
    rb_init(&rb, storage, 4);
    rb_push(&rb, 10); rb_push(&rb, 20); rb_push(&rb, 30);
    rb_pop(&rb, &v);  /* remove 10 */
    rb_push(&rb, 40); /* wraps */
    rb_pop(&rb, &v); chk_byte("wraparound FIFO 20", "rb_pop after wrap", v, 20);
    rb_pop(&rb, &v); chk_byte("wraparound FIFO 30", "rb_pop after wrap", v, 30);
    rb_pop(&rb, &v); chk_byte("wraparound FIFO 40", "rb_pop after wrap", v, 40);
    chk_bool("empty after wrap", "rb_pop empty after wrap", rb_pop(&rb, &v), false);

    nlc_end();
    return 0;
}

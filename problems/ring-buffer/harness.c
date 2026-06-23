#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void case_bool(const char* name, const char* input, bool got, bool exp, const char* out) {
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
}
static void case_int(const char* name, const char* input, long got, long exp, const char* out) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%ld", got);
    snprintf(e, sizeof e, "%ld", exp);
    nlc_case(name, got == exp, input, e, g, out);
}

int main(void) {
    uint8_t storage[4];
    ring_buffer_t rb;
    uint8_t p[6] = {0};
    bool popok[6] = {false};

    nlc_capture_begin();
    rb_init(&rb, storage, 4);
    bool empty0 = rb_is_empty(&rb);
    bool e1 = rb_push(&rb, 10), e2 = rb_push(&rb, 20),
         e3 = rb_push(&rb, 30), e4 = rb_push(&rb, 40);
    bool e5 = rb_push(&rb, 50);          /* full -> false */
    bool full = rb_is_full(&rb);
    size_t cnt = rb_count(&rb);
    popok[0] = rb_pop(&rb, &p[0]);       /* 10 */
    popok[1] = rb_pop(&rb, &p[1]);       /* 20 */
    size_t cnt2 = rb_count(&rb);
    bool e6 = rb_push(&rb, 50);          /* wrap */
    bool e7 = rb_push(&rb, 60);          /* wrap, now full */
    bool e8 = rb_push(&rb, 70);          /* full -> false */
    popok[2] = rb_pop(&rb, &p[2]);       /* 30 */
    popok[3] = rb_pop(&rb, &p[3]);       /* 40 */
    popok[4] = rb_pop(&rb, &p[4]);       /* 50 */
    popok[5] = rb_pop(&rb, &p[5]);       /* 60 */
    bool emptyEnd = rb_is_empty(&rb);
    uint8_t junk = 0;
    bool popEmpty = rb_pop(&rb, &junk);  /* empty -> false */
    char* out = nlc_capture_end();

    nlc_begin();
    case_bool("empty at start", "rb_init(cap=4); rb_is_empty()", empty0, true, out);
    case_bool("push 10", "rb_push(10)", e1, true, "");
    case_bool("push 20", "rb_push(20)", e2, true, "");
    case_bool("push 30", "rb_push(30)", e3, true, "");
    case_bool("push 40", "rb_push(40)", e4, true, "");
    case_bool("push 50 when full", "rb_push(50)", e5, false, "");
    case_bool("is_full", "rb_is_full()", full, true, "");
    case_int("count is 4", "rb_count()", (long)cnt, 4, "");
    case_bool("pop ok #1", "rb_pop()", popok[0], true, "");
    case_int("pop value 10", "rb_pop() -> *out", p[0], 10, "");
    case_int("pop value 20", "rb_pop() -> *out", p[1], 20, "");
    case_int("count is 2", "rb_count()", (long)cnt2, 2, "");
    case_bool("push 50 wrap", "rb_push(50)", e6, true, "");
    case_bool("push 60 wrap", "rb_push(60)", e7, true, "");
    case_bool("push 70 when full", "rb_push(70)", e8, false, "");
    case_int("pop value 30", "rb_pop() -> *out", p[2], 30, "");
    case_int("pop value 40", "rb_pop() -> *out", p[3], 40, "");
    case_int("pop value 50", "rb_pop() -> *out", p[4], 50, "");
    case_int("pop value 60", "rb_pop() -> *out", p[5], 60, "");
    case_bool("empty at end", "rb_is_empty()", emptyEnd, true, "");
    case_bool("pop on empty", "rb_pop()", popEmpty, false, "");
    nlc_end();
    free(out);
    return 0;
}

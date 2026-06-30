#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

int main(void) {
    nlc_begin();

    stack_dir_t dir = detect_stack_direction();

    /* Must return a definite answer (not UNKNOWN). */
    nlc_case("returns a definite direction",
             dir != STACK_UNKNOWN,
             "detect_stack_direction()",
             "GROWS_UP or GROWS_DOWN",
             dir == STACK_GROWS_UP ? "GROWS_UP"
             : dir == STACK_GROWS_DOWN ? "GROWS_DOWN"
             : "UNKNOWN",
             "");

    /* On every platform this test suite runs on, the stack grows down.
       Mark as informational — not a failure if somehow different. */
    char got[32], exp[32];
    snprintf(got, sizeof got, "%s",
             dir == STACK_GROWS_UP ? "GROWS_UP"
             : dir == STACK_GROWS_DOWN ? "GROWS_DOWN" : "UNKNOWN");
    snprintf(exp, sizeof exp, "GROWS_DOWN");
    nlc_case("direction matches platform (informational)",
             dir == STACK_GROWS_DOWN,
             "detect_stack_direction() on x86-64 / ARM",
             exp, got, "");

    nlc_end();
    return 0;
}

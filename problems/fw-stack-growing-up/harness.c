#include <stdio.h>
#include <stdbool.h>
#include "nlc.h"
#include "solution.c"

/* Independent reference measurement using the same across-frames technique,
   so grading is platform-independent instead of hardcoding a direction. */
__attribute__((noinline))
static bool ref_inner_is_higher(const void *outer_addr) {
    volatile char x = 0;
    (void)x;
    return (const void *)&x > outer_addr;
}
static bool ref_grows_up(void) {
    volatile char x = 0;
    (void)x;
    return ref_inner_is_higher((const void *)&x);
}

int main(void) {
    nlc_begin();

    bool ref = ref_grows_up();
    bool got = is_stack_growing_up();

    /* Correctness: must agree with an independent measurement on this machine. */
    nlc_case("matches independent frame measurement",
             got == ref,
             "is_stack_growing_up()",
             ref ? "true (grows up)" : "false (grows down)",
             got ? "true" : "false",
             "");

    /* Determinism: repeated calls give the same answer. */
    bool a = is_stack_growing_up();
    bool b = is_stack_growing_up();
    nlc_case("stable across repeated calls",
             a == b,
             "call twice",
             "same answer both times",
             a == b ? "stable" : "changed",
             "");

    /* Informational: mainstream targets (x86-64, ARM, RISC-V) grow down. */
    nlc_case("grows down on mainstream targets (informational)",
             got == false,
             "is_stack_growing_up() on x86-64 / ARM",
             "false",
             got ? "true" : "false",
             "");

    nlc_end();
    return 0;
}

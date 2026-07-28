#include <stdio.h>
#include <stdbool.h>

/* Must not be inlined so it gets its own stack frame. */
__attribute__((noinline))
static bool inner_is_higher(const void *outer_addr) {
    volatile char inner_local = 0;
    (void)inner_local;
    return (const void *)&inner_local > outer_addr;
}

bool is_stack_growing_up(void) {
    volatile char outer_local = 0;
    (void)outer_local;
    return inner_is_higher((const void *)&outer_local);
}

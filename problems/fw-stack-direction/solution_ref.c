// Time: O(1) — two address comparisons across call frames
#include <stddef.h>

typedef enum {
    STACK_UNKNOWN  = 0,
    STACK_GROWS_UP,
    STACK_GROWS_DOWN
} stack_dir_t;

/* Must not be inlined so it gets its own stack frame. */
__attribute__((noinline))
static stack_dir_t _check_inner(const void *outer_addr) {
    volatile char inner_local = 0;
    (void)inner_local;
    const void *inner_addr = (const void *)&inner_local;
    if (inner_addr > outer_addr) return STACK_GROWS_UP;
    if (inner_addr < outer_addr) return STACK_GROWS_DOWN;
    return STACK_UNKNOWN;
}

stack_dir_t detect_stack_direction(void) {
    volatile char outer_local = 0;
    (void)outer_local;
    return _check_inner((const void *)&outer_local);
}

#include <stdint.h>

/* Bug 1: fix the operator-precedence error so this returns 1 for powers of two. */
int fix_bug1(unsigned n) {
    return n ? (n & (n - 1)) == 0 : 0;   /* BUG here */
}

/* Bug 2: fix the toggle expression so bit j is toggled (not bit-NOR'd). */
uint32_t fix_bug2(uint32_t reg, unsigned i, unsigned j) {
    reg &= ~(1u << i);
    reg ^= (1u << j);   /* BUG here */
    return reg;
}

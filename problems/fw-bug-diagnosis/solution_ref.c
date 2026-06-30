// Time: O(1) — each bug fix is a single expression change
#include <stdint.h>

/* Bug 1 fix: add parentheses so & binds before == */
int fix_bug1(unsigned n) {
    return (n & (n - 1)) == 0 && n != 0;
}

/* Bug 2 fix: use ^ (1u << j) not ^ ~(1u << j) */
uint32_t fix_bug2(uint32_t reg, unsigned i, unsigned j) {
    reg &= ~(1u << i);
    reg ^= (1u << j);
    return reg;
}

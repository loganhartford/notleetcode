// Time: O(1)  — single bitwise AND for check; O(1) for round-up (5 OR+shift steps)
#include <stdbool.h>
#include <stdint.h>

bool is_power_of_two(uint32_t x) {
    return x != 0 && (x & (x - 1)) == 0;
}

uint32_t round_up_pow2(uint32_t x) {
    if (x <= 1) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

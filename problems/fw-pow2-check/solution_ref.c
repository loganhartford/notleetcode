// Time: O(1) — single AND operation
#include <stdint.h>
#include <stdbool.h>

bool is_power_of_two_u32(uint32_t x) {
    return x && !(x & (x - 1));
}

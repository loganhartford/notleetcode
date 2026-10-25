#include <stdint.h>
#include <stdbool.h>

bool is_power_of_two_u32(uint32_t x) {
    if (!x) return false;
    
    return !(x & (x-1));
}

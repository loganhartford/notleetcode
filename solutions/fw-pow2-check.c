#include <stdint.h>
#include <stdbool.h>

bool is_power_of_two_u32(uint32_t x) {
    // Powers of two are represented by single bits in binary
    // Want to know when only one bit is set
    
    // 0100 & 0011
    if (!x) return false;
    
    // For a power of two nubmer, it will only have the MSB set, but 1 less will be all LSB set
    // If you and them they should be zero. In all other cases, this will be non-zero.
    return !(x & (x-1));
}

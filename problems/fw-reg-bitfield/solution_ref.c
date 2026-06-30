// Time: O(1) — single mask and shift operation per function
#include <stdint.h>

uint32_t reg_set_bits(uint32_t reg, uint32_t mask) {
    return reg | mask;
}

uint32_t reg_clear_bits(uint32_t reg, uint32_t mask) {
    return reg & ~mask;
}

uint32_t reg_write_field(uint32_t reg, uint32_t mask, unsigned shift, uint32_t value) {
    return (reg & ~mask) | ((value << shift) & mask);
}

uint32_t reg_read_field(uint32_t reg, uint32_t mask, unsigned shift) {
    return (reg & mask) >> shift;
}

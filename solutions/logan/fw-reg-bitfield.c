#include <stdint.h>

uint32_t reg_set_bits(uint32_t reg, uint32_t mask) {
    return reg | mask;
}

uint32_t reg_clear_bits(uint32_t reg, uint32_t mask) {
    return reg & ~mask;
}

uint32_t reg_write_field(uint32_t reg, uint32_t mask, unsigned shift, uint32_t value) {
    reg = reg_clear_bits(reg, mask);
    return reg_set_bits(reg, (mask & (value << shift)));
}

uint32_t reg_read_field(uint32_t reg, uint32_t mask, unsigned shift) {
    reg = reg_clear_bits(reg, ~mask);
    return reg >> shift;
}

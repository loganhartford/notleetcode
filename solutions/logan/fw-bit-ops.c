#include <stdint.h>
#include <stdbool.h>

uint32_t bit_set(uint32_t x, unsigned n) {
    if (n >= 32) return x;
    return (x | (1 << n));
}

uint32_t bit_clear(uint32_t x, unsigned n) {
    return (x & ~(1 << n));
}

uint32_t bit_toggle(uint32_t x, unsigned n) {
    if (n >= 32) return x;
    return (x ^ (1 << n));
}

bool bit_test(uint32_t x, unsigned n) {
    if (n >= 32) return false;
    return (x & (1 << n));
}

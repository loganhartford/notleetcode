// Time: O(k) popcount (k=set bits, Brian Kernighan); O(32) reverse; O(1) rotate
#include <stdint.h>

unsigned popcount32(uint32_t x) {
    unsigned count = 0;
    while (x) { x &= x - 1; count++; }
    return count;
}

uint32_t reverse_bits32(uint32_t x) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (x & 1u);
        x >>= 1;
    }
    return result;
}

uint32_t rotl32(uint32_t x, unsigned n) {
    n %= 32;
    if (n == 0) return x;
    return (x << n) | (x >> (32u - n));
}

uint32_t rotr32(uint32_t x, unsigned n) {
    n %= 32;
    if (n == 0) return x;
    return (x >> n) | (x << (32u - n));
}

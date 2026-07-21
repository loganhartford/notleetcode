#include <stdint.h>

unsigned popcount32(uint32_t x) {
    unsigned count = 0;
    while (x) {
        x &= (x - 1);
        count++;
    }
    return count;
}

uint32_t reverse_bits32(uint32_t x) {
    uint32_t reversed = 0;
    for (int i = 0; i < 32; i++) {
        // Select the next bit and shift it down to the first bit position
        uint32_t down = ((x & (1 << i)) >> i);
        // Shift it up to its not position and or it in
        reversed |= down << (31 - i);
    }
    return reversed;
}

uint32_t rotl32(uint32_t x, unsigned n) {
    uint64_t temp = (uint64_t)x << n;
    x = (uint32_t)(temp & 0xFFFFFFFF) | ((temp & 0xFFFFFFFF00000000) >> 32);
    return x;
}

uint32_t rotr32(uint32_t x, unsigned n) {
    uint64_t temp = (uint64_t)x << 32;
    temp >> n;
    x = (uint32_t)(temp & 0xFFFFFFFF) | ((temp & 0xFFFFFFFF00000000) >> 32);
    return x;
}

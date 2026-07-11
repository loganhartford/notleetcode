#include <stdint.h>
#include <stdbool.h>

uint32_t bit_set(uint32_t x, unsigned n) {
    
    if (n > 31) {
        return x;
    }
    
    uint32_t mask = 1 << n;
    
    x = x | mask;
    
    return x;
}

uint32_t bit_clear(uint32_t x, unsigned n) {
    
    if (n > 31) {
        return x;
    }
    
    uint32_t mask = 1 << n;
    mask = ~mask;
    
    x = x & mask;
    
    return x;
}

uint32_t bit_toggle(uint32_t x, unsigned n) {
    if (n > 31) {
        return x;
    }
    
    uint32_t mask = 1 << n;
    
    x = x ^ mask;
    return x;
}

bool bit_test(uint32_t x, unsigned n) {
    if (n > 31) {
        return false;
    }
    
    uint32_t mask = 1 << n;
    
    return (x & mask);
}

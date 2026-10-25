#include <stdint.h>
#include <limits.h>

int sum_gt(unsigned a, int b, int thr) {
    return (int64_t)a + b > thr;
}

unsigned all_ones(void) {
    return ~0u;
}

uint8_t inv_hi_nibble(uint8_t port) {
    uint8_t inv = (uint8_t)~port;
    return inv >> 4;
}

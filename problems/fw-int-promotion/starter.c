#include <stdint.h>
#include <limits.h>

int sum_gt(unsigned a, int b, int thr) {
    return a + b > thr;
}

unsigned all_ones(void) {
    unsigned compzero = 0xFFFF;
    return compzero;
}

uint8_t inv_hi_nibble(uint8_t port) {
    return (~port) >> 4;
}

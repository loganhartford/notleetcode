// Time: O(1) — fixed-point arithmetic with 64-bit intermediate
#include <stdint.h>

typedef int32_t q16_t;

q16_t q16_from_int(int32_t x) {
    return x << 16;
}

int32_t q16_to_int(q16_t x) {
    return x >> 16;
}

q16_t q16_mul(q16_t a, q16_t b) {
    return (q16_t)(((int64_t)a * b) >> 16);
}

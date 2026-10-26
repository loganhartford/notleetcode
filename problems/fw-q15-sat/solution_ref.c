#include <stdint.h>

typedef int16_t q15_t;

static q15_t q15_sat(int32_t x) {
    if (x > INT16_MAX) return INT16_MAX;
    if (x < INT16_MIN) return INT16_MIN;
    return (q15_t)x;
}

q15_t q15_add_sat(q15_t a, q15_t b) {
    return q15_sat((int32_t)a + b);
}

q15_t q15_sub_sat(q15_t a, q15_t b) {
    return q15_sat((int32_t)a - b);
}

q15_t q15_mul(q15_t a, q15_t b) {
    return q15_sat(((int32_t)a * b + (1 << 14)) >> 15);
}

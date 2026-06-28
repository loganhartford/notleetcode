// Time: O(1)  — single 64-bit multiply and right-shift
#include <stdint.h>

int32_t qmul(int32_t a, int32_t b, int frac_bits) {
    return (int32_t)(((int64_t)a * (int64_t)b) >> frac_bits);
}

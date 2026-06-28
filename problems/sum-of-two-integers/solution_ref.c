// Time: O(1)  — at most 32 carry-propagation iterations
#include <stdint.h>

int getSum(int a, int b) {
    uint32_t x = (uint32_t)a;
    uint32_t y = (uint32_t)b;
    while (y != 0) {
        uint32_t carry = (x & y) << 1;
        x = x ^ y;
        y = carry;
    }
    return (int)x;
}

// Time: O(n) — sliding window, each pointer advances at most n times
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool subarray_sum_nonneg(const uint32_t *a, size_t n, uint32_t target,
                          size_t *start_out, size_t *end_out) {
    if (target == 0) {
        *start_out = 0;
        *end_out   = 0;
        return true;
    }
    size_t  lo  = 0, hi = 0;
    uint64_t sum = 0;
    while (hi < n) {
        sum += a[hi++];
        while (sum > target && lo < hi) sum -= a[lo++];
        if (sum == target) {
            *start_out = lo;
            *end_out   = hi;
            return true;
        }
    }
    return false;
}

// Time: O(log n) — binary search for lower bracket
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool lower_bracket_index(const int32_t *xs, size_t n,
                          int32_t target, size_t *idx_out) {
    if (n == 0) return false;
    if (target < xs[0]) return false;
    if (target >= xs[n-1]) { *idx_out = n - 1; return true; }

    size_t lo = 0, hi = n - 1;
    while (lo + 1 < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (xs[mid] <= target) lo = mid;
        else                   hi = mid;
    }
    *idx_out = lo;
    return true;
}

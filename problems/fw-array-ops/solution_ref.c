// Time: O(n) — two pointers for reverse; three passes for rotate
#include <stdint.h>
#include <stddef.h>

void reverse_i32(int32_t *a, size_t n) {
    size_t lo = 0, hi = n;
    if (hi == 0) return;
    hi--;
    while (lo < hi) {
        int32_t tmp = a[lo];
        a[lo++] = a[hi];
        a[hi--] = tmp;
    }
}

void rotate_left_i32(int32_t *a, size_t n, size_t k) {
    if (n <= 1) return;
    k %= n;
    if (k == 0) return;
    /* three-reversal trick */
    reverse_i32(a, k);
    reverse_i32(a + k, n - k);
    reverse_i32(a, n);
}

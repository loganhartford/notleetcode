// Time: O(n) — cyclic sort; each element moves at most once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool find_duplicate_u32(uint32_t *a, size_t n, uint32_t *dup_out) {
    for (size_t i = 0; i < n; i++) {
        while (a[i] != (uint32_t)i) {
            uint32_t v = a[i];
            if (a[v] == v) {
                *dup_out = v;
                return true;
            }
            /* swap a[i] and a[v] */
            uint32_t tmp = a[i];
            a[i] = a[v];
            a[v] = tmp;
        }
    }
    return false;
}

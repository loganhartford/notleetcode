// Time: O(n) — single pass through string digits
#include <stdint.h>
#include <stdbool.h>

bool parse_i32(const char *s, int32_t *out) {
    if (!s || !*s) return false;

    bool neg = false;
    if (*s == '+') { s++; }
    else if (*s == '-') { neg = true; s++; }

    if (*s == '\0') return false; /* sign only */

    int64_t val = 0;
    while (*s) {
        if (*s < '0' || *s > '9') return false;
        val = val * 10 + (*s - '0');
        if (neg && -val < (int64_t)INT32_MIN) return false;
        if (!neg && val > (int64_t)INT32_MAX) return false;
        s++;
    }
    *out = neg ? (int32_t)(-val) : (int32_t)val;
    return true;
}

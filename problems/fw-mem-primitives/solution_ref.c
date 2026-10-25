// Time: O(n) — single byte-by-byte pass
#include <stddef.h>
#include <stdint.h>

void *c_memcpy(void *dst, const void *src, size_t n) {
    unsigned char       *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

void *c_memmove(void *dst, const void *src, size_t n) {
    unsigned char       *d = dst;
    const unsigned char *s = src;
    if (n == 0 || d == s) return dst;
    size_t offset = 1;
    while (offset < n && d != s + offset) ++offset;
    if (offset < n) {
        while (n > 0) { --n; d[n] = s[n]; }
    } else {
        for (size_t i = 0; i < n; ++i) d[i] = s[i];
    }
    return dst;
}

void *c_memset(void *dst, int value, size_t n) {
    unsigned char *d = dst;
    unsigned char v = (unsigned char)value;
    while (n--) *d++ = v;
    return dst;
}

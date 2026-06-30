// Time: O(n) — single byte-by-byte pass
#include <stddef.h>
#include <stdint.h>

void *c_memcpy(void *dst, const void *src, size_t n) {
    uint8_t       *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (n--) *d++ = *s++;
    return dst;
}

void *c_memmove(void *dst, const void *src, size_t n) {
    uint8_t       *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    if (d < s) {
        while (n--) *d++ = *s++;
    } else if (d > s) {
        d += n; s += n;
        while (n--) *--d = *--s;
    }
    return dst;
}

void *c_memset(void *dst, int value, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t  v = (uint8_t)(value & 0xFF);
    while (n--) *d++ = v;
    return dst;
}

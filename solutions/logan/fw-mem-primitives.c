#include <stddef.h>
#include <stdint.h>

void *c_memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s = (uint8_t *)src;
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dst;
}

void *c_memmove(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t *s = (uint8_t *)src;
    
    if (d < s) {
        c_memcpy(dst, src, n);
    } else {
        for (int i = n-1; i >= 0; i--) {
            d[i] = s[i];
        }
    }
    
    return dst;
}

void *c_memset(void *dst, int value, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    
    for (int i = 0; i < n; i++) {
        d[i] = (0xFF & value);
    }
    
    return dst;
}

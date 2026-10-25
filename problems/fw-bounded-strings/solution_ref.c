#include <stddef.h>

size_t c_strlcpy(char *dst, const char *src, size_t dstsize) {
    size_t n = 0;
    while (src[n] != '\0') {
        if (dstsize > 0 && n < dstsize - 1) dst[n] = src[n];
        ++n;
    }
    if (dstsize > 0) dst[n < dstsize ? n : dstsize - 1] = '\0';
    return n;
}

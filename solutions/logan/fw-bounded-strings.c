#include <stddef.h>

size_t c_strlcpy(char *dst, const char *src, size_t dstsize) {
    size_t src_len = 0;
    while(src[src_len]) src_len++;
    
    if (!dstsize) return src_len;
    
    size_t dst_len = 0;
    while (dst_len < (dstsize - 1) && src[dst_len]) {
        dst[dst_len] = src[dst_len];
        dst_len++;        
    }
    dst[dst_len] = '\0';
    
    return src_len;
}

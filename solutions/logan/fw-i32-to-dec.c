#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool i32_to_dec(char *out, size_t out_len, int32_t value) {
    bool neg = value < 0;
    uint32_t u = (uint32_t)value;
    if (neg) u = 0U - u;
    
    char tmp[11]; //-2147483648 - 11 chars
    size_t n = 0;
    do {
        tmp[n++] = '0' + u % 10;
        u /= 10;
    } while (u != 0);
    if (neg) tmp[n++] = '-';
    
    // Length check
    if (n + 1 > out_len) return false;
    
    for (size_t i = 0; i < n; i++) {
        out[i] = tmp[n-1-i];
    }
    out[n] = '\0';
    
    return true;
    
}

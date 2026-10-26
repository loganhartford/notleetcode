#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool parse_i32(const char *s, int32_t *out) {
    size_t len = strlen(s);
    uint8_t nums[len];
    bool neg = false;
    for (int i = 0; i < len; i++) {
        if (s[i] == '-') {
            neg = true;
        } else {
            nums[i] = (uint8_t)s[i] - (uint8_t)'0'; 
        }
    }
    
    int64_t num = 0;
    if (neg) len -= 1;
    for (int i = 0; i < len; i++) {
        num += nums[i];
        num *= 10;
    }
    
    if (neg) num *= -1;
    
    if (neg && num < INT32_MIN) return false;
    if (!neg && num > INT32_MAX) return false;
    
    *out = (int32_t)num;
    
    return true;
}

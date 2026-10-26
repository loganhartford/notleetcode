#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool parse_i32(const char *s, int32_t *out) {
    if (!*s || !s) return false;
    
    size_t len = strlen(s);
    uint8_t nums[len];
    bool neg = false;
    size_t n = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '-') {
            neg = true;
        } else if (s[i] == '+') {
            continue;
        } else if (s[i] < '0' || s[i] > '9') {
            return false;
        }
        else {
            nums[n] = (uint8_t)s[i] - (uint8_t)'0';
            n++;
        }
    }
    
    if (!n) return false;
    
    int64_t num = 0;
    for (int i = 0; i < n-1; i++) {
        num += nums[i];
        num *= 10;
    }
    num += nums[n-1];
    
    if (neg) num *= -1;
    
    if (neg && num < INT32_MIN) return false;
    if (!neg && num > INT32_MAX) return false;
    
    *out = (int32_t)num;
    
    return true;
}

// Time: O(log n) — one digit extracted per division
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

bool i32_to_dec(char *out, size_t out_len, int32_t value) {
    if (out_len == 0) return false;

    char tmp[12]; /* enough for -2147483648\0 */
    int  pos = 0;
    bool neg = false;

    uint32_t uval;
    if (value < 0) {
        neg  = true;
        uval = (value == INT32_MIN) ? (uint32_t)2147483648u : (uint32_t)(-value);
    } else {
        uval = (uint32_t)value;
    }

    if (uval == 0) {
        tmp[pos++] = '0';
    } else {
        while (uval) {
            tmp[pos++] = '0' + (char)(uval % 10);
            uval /= 10;
        }
    }
    if (neg) tmp[pos++] = '-';

    /* tmp holds digits in reverse; check length */
    if ((size_t)pos + 1 > out_len) return false;

    for (int i = 0; i < pos; i++) {
        out[i] = tmp[pos - 1 - i];
    }
    out[pos] = '\0';
    return true;
}

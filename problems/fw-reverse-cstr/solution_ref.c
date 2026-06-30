// Time: O(n) — single pass with two pointers
#include <stddef.h>

void reverse_cstr(char *s) {
    if (!s || !*s) return;
    char *lo = s;
    char *hi = s;
    while (*hi) hi++;
    hi--;
    while (lo < hi) {
        char tmp = *lo;
        *lo++ = *hi;
        *hi-- = tmp;
    }
}

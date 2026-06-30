// Time: O(n) — single pointer walk to null terminator
#include <stddef.h>

size_t c_strlen(const char *s) {
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

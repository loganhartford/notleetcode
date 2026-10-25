#include <stddef.h>

int my_strcmp(const char *a, const char *b) {
    const unsigned char *p = (const unsigned char *)a;
    const unsigned char *q = (const unsigned char *)b;
    while (*p && *p == *q) {
        p++;
        q++;
    }
    return *p - *q;
}

char *my_strstr(const char *hay, const char *needle) {
    if (!*needle) return (char *)hay;
    for (; *hay; hay++) {
        const char *h = hay;
        const char *n = needle;
        while (*h && *h == *n) {
            h++;
            n++;
        }
        if (!*n) return (char *)hay;
        if (!*h) return NULL;
    }
    return NULL;
}

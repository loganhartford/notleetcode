#include <stddef.h>

int my_strcmp(const char *a, const char *b) {
    // whether or not char is signed or not is up to the compiler
    // so we need to define it as such to get consistent behavior
    // since 0x80 can be -128 or 128 depending on the sign.
    const unsigned char *c = (const unsigned char *) a;
    const unsigned char *d = (const unsigned char *) b;
    while(*c == *d && *c) {
        c++, d++;
    }
    
    return *c - *d;
}

#include <stdbool.h>

char *my_strstr(const char *hay, const char *needle) {
    
    if (!needle) return (char *)hay;
    
    while (*hay) {
        const char * h = hay;
        const char * n = needle;
        while (*h && *h == *n) {
            h++, n++;
        }
        if (!*n) return (char *)hay;
        if (!*h) return NULL;
        hay++;
    }
    
    return NULL;
}

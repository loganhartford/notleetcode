#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *run_length_encode(char *str) {
    size_t len = strlen(str);
    char *out = malloc(2 * len + 1);
    if (!out) return NULL;

    size_t oi = 0;
    while (*str != '\0') {
        char ch = *str;
        unsigned count = 0;
        while (*str == ch) { str++; count++; }

        out[oi++] = ch;
        oi += (size_t)sprintf(out + oi, "%u", count);
    }
    out[oi] = '\0';
    return out;
}

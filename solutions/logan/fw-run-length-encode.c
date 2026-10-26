#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Really did poor on this one
// mainly forgot about how to use string.h

char *run_length_encode(char *str) {
    size_t len = strlen(str);
    char *out = malloc(2 * len +1); // worst case is abc -> a1b1c1 plus null terminator
    if (!out) return NULL;
    
    size_t index = 0;
    while(*str) {
        char ch = *str;
        uint8_t count = 0;
        while(ch == *str) {count++; str++;}
        
        out[index++] = ch;
        index += (size_t)sprintf(out + index, "%u", count);
    }
    out[index] = '\0';
    return out;
}

#include <stddef.h>

size_t c_strlen(const char *s) {
    uint8_t count = 0;
    while(*s) {
        s++;
        count++;
    }
    
    return count;
}

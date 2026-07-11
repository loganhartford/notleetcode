#include <stddef.h>

size_t c_strlen(const char *s) {
    
    size_t i = 0;
    char ch = s[i];
    while (ch != '\0') {
        i++;
        ch = s[i];
    }
    return i;
}

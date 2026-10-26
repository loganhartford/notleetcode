#include <stddef.h>

void reverse_cstr(char *s) {
    char *low = s;
    char *high = s;
    
    while(*s) s++;
    high = s;
    high--;
    
    while (low < high) {
        char temp = *low;
        *low = *high;
        *high = temp;
        low++;
        high--;
    }
    
    return;
}

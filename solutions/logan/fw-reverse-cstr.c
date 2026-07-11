#include <stddef.h>

void reverse_cstr(char *s) {
    
    int j = strlen(s) - 1;
    int i = 0;
    
    while (i < j) {
        fprintf(stderr, "%d,%d", i, j);
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        i++;
        j--;
    }   
}

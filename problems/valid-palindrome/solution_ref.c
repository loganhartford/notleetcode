// Time: O(n)  — two-pointer single pass
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

bool isPalindrome(char* s) {
    int i = 0, j = (int)strlen(s) - 1;
    while (i < j) {
        if (!isalnum((unsigned char)s[i])) {
            i++;
        } else if (!isalnum((unsigned char)s[j])) {
            j--;
        } else if (tolower((unsigned char)s[i]) != tolower((unsigned char)s[j])) {
            return false;
        } else {
            i++;
            j--;
        }
    }
    return true;
}

#include <stdbool.h>
#include <string.h>

bool isAnagram(char* s, char* t) {
    if (strlen(s) != strlen(t)) return false;
    int cnt[256] = {0};
    for (int i = 0; s[i]; i++) cnt[(unsigned char)s[i]]++;
    for (int i = 0; t[i]; i++) {
        if (--cnt[(unsigned char)t[i]] < 0) return false;
    }
    return true;
}

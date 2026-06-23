#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, char* s, const char* input, int exp) {
    nlc_capture_begin();
    int got = lengthOfLongestSubstring(s);
    char* out = nlc_capture_end();
    char g[32], e[32];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("abcabcbb", "abcabcbb", "s=\"abcabcbb\"", 3);
    check("bbbbb", "bbbbb", "s=\"bbbbb\"", 1);
    check("pwwkew", "pwwkew", "s=\"pwwkew\"", 3);
    check("empty", "", "s=\"\"", 0);
    check("spaces", " ", "s=\" \"", 1);
    check("tmmzuxt", "tmmzuxt", "s=\"tmmzuxt\"", 5);
    check("dvdf", "dvdf", "s=\"dvdf\"", 3);
    nlc_end();
    return 0;
}

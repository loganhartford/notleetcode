#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *s, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    char in[64];
    snprintf(in, sizeof in, "longest(\"%s\")", s);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    chk("empty",   "",       longest_unique_substr_ascii(""),       0);
    chk("single a","a",      longest_unique_substr_ascii("a"),      1);
    chk("abcabc",  "abcabc", longest_unique_substr_ascii("abcabc"), 3);
    chk("aaaaaa",  "aaaaaa", longest_unique_substr_ascii("aaaaaa"), 1);
    chk("abcd",    "abcd",   longest_unique_substr_ascii("abcd"),   4);
    chk("pwwkew",  "pwwkew", longest_unique_substr_ascii("pwwkew"), 3);
    chk("dvdf",    "dvdf",   longest_unique_substr_ascii("dvdf"),   3);
    chk("abba",    "abba",   longest_unique_substr_ascii("abba"),   2);
    chk("tmmzuxt", "tmmzuxt",longest_unique_substr_ascii("tmmzuxt"),5);

    nlc_end();
    return 0;
}

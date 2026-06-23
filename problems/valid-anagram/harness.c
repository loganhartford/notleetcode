#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, char* s, char* t, const char* input, bool exp) {
    nlc_capture_begin();
    bool got = isAnagram(s, t);
    char* out = nlc_capture_end();
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("anagram", "anagram", "nagaram", "s=\"anagram\", t=\"nagaram\"", true);
    check("not anagram", "rat", "car", "s=\"rat\", t=\"car\"", false);
    check("different lengths", "a", "ab", "s=\"a\", t=\"ab\"", false);
    check("same string", "abc", "abc", "s=\"abc\", t=\"abc\"", true);
    check("repeated letters", "aacc", "ccac", "s=\"aacc\", t=\"ccac\"", false);
    check("single char", "a", "a", "s=\"a\", t=\"a\"", true);
    nlc_end();
    return 0;
}

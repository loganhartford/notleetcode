#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, char* s, const char* input, bool exp) {
    nlc_capture_begin();
    bool got = isPalindrome(s);
    char* out = nlc_capture_end();
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("panama", "A man, a plan, a canal: Panama", "s=\"A man, a plan, a canal: Panama\"", true);
    check("race a car", "race a car", "s=\"race a car\"", false);
    check("blank", " ", "s=\" \"", true);
    check("alphanumeric", "0P", "s=\"0P\"", false);
    check("digits palindrome", "1a2b2a1", "s=\"1a2b2a1\"", true);
    check("single char", "x", "s=\"x\"", true);
    nlc_end();
    return 0;
}

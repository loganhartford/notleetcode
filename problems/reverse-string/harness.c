#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, const char* init, int n, const char* input, const char* exp) {
    char buf[64];
    memcpy(buf, init, n);
    buf[n] = '\0';
    nlc_capture_begin();
    reverseString(buf, n);
    char* out = nlc_capture_end();
    buf[n] = '\0';
    int ok = memcmp(buf, exp, n) == 0;
    nlc_case(name, ok, input, exp, buf, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("hello", "hello", 5, "['h','e','l','l','o']", "olleh");
    check("Hannah", "Hannah", 6, "['H','a','n','n','a','h']", "hannaH");
    check("single", "x", 1, "['x']", "x");
    check("two", "ab", 2, "['a','b']", "ba");
    nlc_end();
    return 0;
}

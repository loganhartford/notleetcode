#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, size_t got, size_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    chk("empty string",      "c_strlen(\"\")",           c_strlen(""),           0);
    chk("single char",       "c_strlen(\"a\")",          c_strlen("a"),          1);
    chk("hello",             "c_strlen(\"hello\")",      c_strlen("hello"),      5);
    chk("notleetcode",       "c_strlen(\"notleetcode\")",c_strlen("notleetcode"),11);
    chk("12345",             "c_strlen(\"12345\")",      c_strlen("12345"),      5);
    chk("abcde",             "c_strlen(\"abcde\")",      c_strlen("abcde"),      5);
    chk("x",                 "c_strlen(\"x\")",          c_strlen("x"),          1);

    nlc_end();
    return 0;
}

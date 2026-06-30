#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_str(const char *nm, const char *in, const char *got, const char *exp) {
    nlc_case(nm, strcmp(got, exp) == 0, in, exp, got, "");
}

int main(void) {
    nlc_begin();

    char s1[] = "hello";
    reverse_cstr(s1);
    chk_str("hello reversed", "reverse_cstr(\"hello\")", s1, "olleh");

    char s2[] = "ab";
    reverse_cstr(s2);
    chk_str("ab reversed", "reverse_cstr(\"ab\")", s2, "ba");

    char s3[] = "";
    reverse_cstr(s3);
    chk_str("empty unchanged", "reverse_cstr(\"\")", s3, "");

    char s4[] = "x";
    reverse_cstr(s4);
    chk_str("single char unchanged", "reverse_cstr(\"x\")", s4, "x");

    char s5[] = "racecar";
    reverse_cstr(s5);
    chk_str("palindrome unchanged", "reverse_cstr(\"racecar\")", s5, "racecar");

    char s6[] = "abcde";
    reverse_cstr(s6);
    chk_str("abcde reversed", "reverse_cstr(\"abcde\")", s6, "edcba");

    char s7[] = "12345";
    reverse_cstr(s7);
    chk_str("12345 reversed", "reverse_cstr(\"12345\")", s7, "54321");

    nlc_end();
    return 0;
}

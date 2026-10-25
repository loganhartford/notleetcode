#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"

static void fmt_cstr(char *buf, size_t cap, const char *s) {
    size_t off = (size_t)snprintf(buf, cap, "\"");
    for (; *s && off + 6 < cap; s++) {
        unsigned char c = (unsigned char)*s;
        if (c < 0x20 || c >= 0x7f)
            off += (size_t)snprintf(buf + off, cap - off, "\\x%02x", c);
        else
            buf[off++] = (char)c;
    }
    snprintf(buf + off, cap - off, "\"");
}

static int sign_of(int x) { return (x > 0) - (x < 0); }

static const char *sign_word(int s) {
    return s < 0 ? "negative" : s > 0 ? "positive" : "0";
}

static void fmt_ptr(char *buf, size_t cap, const char *p, const char *hay) {
    uintptr_t lo = (uintptr_t)hay, hi = lo + strlen(hay), at = (uintptr_t)p;
    if (!p) snprintf(buf, cap, "NULL");
    else if (at >= lo && at <= hi) snprintf(buf, cap, "hay + %zu", (size_t)(at - lo));
    else snprintf(buf, cap, "pointer outside hay");
}

#pragma GCC poison strcmp strncmp strstr memcmp

#include "solution.c"

static void cmp_case(const char *name, const char *a, const char *b, int exp_sign) {
    char sa[64], sb[64], in[160], got[48];
    int r = my_strcmp(a, b);
    fmt_cstr(sa, sizeof sa, a);
    fmt_cstr(sb, sizeof sb, b);
    snprintf(in, sizeof in, "my_strcmp(%s, %s)", sa, sb);
    if (sign_of(r) == 0) snprintf(got, sizeof got, "0");
    else snprintf(got, sizeof got, "%s (%d)", sign_word(sign_of(r)), r);
    nlc_case(name, sign_of(r) == exp_sign, in, sign_word(exp_sign), got, "");
}

static void str_case(const char *name, const char *hay, const char *needle, int exp_off) {
    char sh[64], sn[64], in[160], exp[32], got[32];
    char *r = my_strstr(hay, needle);
    int ok = exp_off < 0 ? r == NULL : r == hay + exp_off;
    fmt_cstr(sh, sizeof sh, hay);
    fmt_cstr(sn, sizeof sn, needle);
    snprintf(in, sizeof in, "my_strstr(%s, %s)", sh, sn);
    if (exp_off < 0) snprintf(exp, sizeof exp, "NULL");
    else snprintf(exp, sizeof exp, "hay + %d", exp_off);
    fmt_ptr(got, sizeof got, r, hay);
    nlc_case(name, ok, in, exp, got, "");
}

int main(void) {
    nlc_begin();

    cmp_case("strcmp: equal strings", "abc", "abc", 0);
    cmp_case("strcmp: last char smaller", "abc", "abd", -1);
    cmp_case("strcmp: last char larger", "abd", "abc", 1);
    cmp_case("strcmp: longer string sorts after its prefix", "abc", "ab", 1);
    cmp_case("strcmp: prefix sorts before longer string", "ab", "abc", -1);
    cmp_case("strcmp: both empty", "", "", 0);
    cmp_case("strcmp: empty sorts first", "", "a", -1);
    cmp_case("strcmp: high byte compares as unsigned", "\x80", "a", 1);
    cmp_case("strcmp: high byte compares as unsigned (reversed)", "a", "\x80", -1);

    str_case("strstr: match in the middle", "hello", "ll", 2);
    str_case("strstr: partial match must restart", "aaab", "aab", 1);
    str_case("strstr: empty needle returns hay", "abc", "", 0);
    str_case("strstr: needle longer than hay", "abc", "abcd", -1);
    str_case("strstr: match at the end", "abc", "c", 2);
    str_case("strstr: empty hay", "", "a", -1);
    str_case("strstr: overlapping candidates", "mississippi", "issip", 4);
    str_case("strstr: needle equals hay", "abc", "abc", 0);
    str_case("strstr: not present", "abcabc", "abd", -1);

    nlc_end();
    return 0;
}

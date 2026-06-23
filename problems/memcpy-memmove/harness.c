#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void case_str(const char* name, const char* input, const char* got, const char* exp) {
    nlc_case(name, strcmp(got, exp) == 0, input, exp, got, "");
}
static void case_bool(const char* name, const char* input, int got, int exp) {
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    {
        char b[16] = "hello world";
        void* ret = my_memcpy(b, b + 6, 5);
        b[5] = '\0';
        case_str("memcpy copies bytes", "memcpy(b, b+6, 5) on 'hello world'", b, "world");
        case_bool("memcpy returns dst", "ret == dst", ret == b, 1);
    }
    {
        char c[16] = "abcdef";
        void* ret = my_memmove(c + 2, c, 4);  /* overlap, dst > src */
        case_str("memmove overlap forward", "memmove(c+2, c, 4) on 'abcdef'", c, "ababcd");
        case_bool("memmove returns dst", "ret == dst", ret == c + 2, 1);
    }
    {
        char c[16] = "abcdef";
        my_memmove(c, c + 2, 4);  /* overlap, dst < src */
        case_str("memmove overlap backward", "memmove(c, c+2, 4) on 'abcdef'", c, "cdefef");
    }
    {
        char src[8] = "12345";
        char dst[8] = {0};
        my_memmove(dst, src, 6);  /* non-overlapping */
        case_str("memmove non-overlap", "memmove(dst, '12345', 6)", dst, "12345");
    }
    {
        char z[8] = "keepme";
        my_memcpy(z, z, 0);  /* n == 0 is a no-op */
        case_str("zero length", "memcpy(z, z, 0)", z, "keepme");
    }
    nlc_end();
    return 0;
}

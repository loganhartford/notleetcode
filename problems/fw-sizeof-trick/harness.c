#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "nlc.h"
#include "solution.c"

static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    chk_sz("char",     "MY_SIZEOF_TYPE(char)",     MY_SIZEOF_TYPE(char),     sizeof(char));
    chk_sz("int",      "MY_SIZEOF_TYPE(int)",       MY_SIZEOF_TYPE(int),      sizeof(int));
    chk_sz("long",     "MY_SIZEOF_TYPE(long)",      MY_SIZEOF_TYPE(long),     sizeof(long));
    chk_sz("double",   "MY_SIZEOF_TYPE(double)",    MY_SIZEOF_TYPE(double),   sizeof(double));
    chk_sz("uint8_t",  "MY_SIZEOF_TYPE(uint8_t)",   MY_SIZEOF_TYPE(uint8_t),  sizeof(uint8_t));
    chk_sz("uint16_t", "MY_SIZEOF_TYPE(uint16_t)",  MY_SIZEOF_TYPE(uint16_t), sizeof(uint16_t));
    chk_sz("uint32_t", "MY_SIZEOF_TYPE(uint32_t)",  MY_SIZEOF_TYPE(uint32_t), sizeof(uint32_t));
    chk_sz("uint64_t", "MY_SIZEOF_TYPE(uint64_t)",  MY_SIZEOF_TYPE(uint64_t), sizeof(uint64_t));
    chk_sz("void *",   "MY_SIZEOF_TYPE(void *)",    MY_SIZEOF_TYPE(void *),   sizeof(void *));

    nlc_end();
    return 0;
}

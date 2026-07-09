#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

    chk_sz("char is 1",     "MY_SIZEOF(char)",     MY_SIZEOF(char),     sizeof(char));
    chk_sz("short is 2",    "MY_SIZEOF(short)",    MY_SIZEOF(short),    sizeof(short));
    chk_sz("int is 4",      "MY_SIZEOF(int)",      MY_SIZEOF(int),      sizeof(int));
    chk_sz("long is 8",     "MY_SIZEOF(long)",     MY_SIZEOF(long),     sizeof(long));
    chk_sz("float is 4",    "MY_SIZEOF(float)",    MY_SIZEOF(float),    sizeof(float));
    chk_sz("double is 8",   "MY_SIZEOF(double)",   MY_SIZEOF(double),   sizeof(double));
    chk_sz("uint8_t is 1",  "MY_SIZEOF(uint8_t)",  MY_SIZEOF(uint8_t),  sizeof(uint8_t));
    chk_sz("uint16_t is 2", "MY_SIZEOF(uint16_t)", MY_SIZEOF(uint16_t), sizeof(uint16_t));
    chk_sz("uint32_t is 4", "MY_SIZEOF(uint32_t)", MY_SIZEOF(uint32_t), sizeof(uint32_t));
    chk_sz("uint64_t is 8", "MY_SIZEOF(uint64_t)", MY_SIZEOF(uint64_t), sizeof(uint64_t));
    chk_sz("pointer",       "MY_SIZEOF(void*)",    MY_SIZEOF(void*),    sizeof(void*));

    typedef struct { int x; double y; } pair_t;
    chk_sz("struct",        "MY_SIZEOF(pair_t)",   MY_SIZEOF(pair_t),   sizeof(pair_t));

    nlc_end();
    return 0;
}

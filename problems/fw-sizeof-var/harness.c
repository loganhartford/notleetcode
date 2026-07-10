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

    int       vi  = 0;
    char      vc  = 0;
    double    vd  = 0;
    uint8_t   v8  = 0;
    uint16_t  v16 = 0;
    uint32_t  v32 = 0;
    uint64_t  v64 = 0;
    void     *vp  = 0;
    int       arr10[10];
    struct { char c; int i; double d; } vs = {0, 0, 0};

    chk_sz("int",      "MY_SIZEOF_VAR(vi)",    MY_SIZEOF_VAR(vi),    sizeof(vi));
    chk_sz("char",     "MY_SIZEOF_VAR(vc)",    MY_SIZEOF_VAR(vc),    sizeof(vc));
    chk_sz("double",   "MY_SIZEOF_VAR(vd)",    MY_SIZEOF_VAR(vd),    sizeof(vd));
    chk_sz("uint8_t",  "MY_SIZEOF_VAR(v8)",    MY_SIZEOF_VAR(v8),    sizeof(v8));
    chk_sz("uint16_t", "MY_SIZEOF_VAR(v16)",   MY_SIZEOF_VAR(v16),   sizeof(v16));
    chk_sz("uint32_t", "MY_SIZEOF_VAR(v32)",   MY_SIZEOF_VAR(v32),   sizeof(v32));
    chk_sz("uint64_t", "MY_SIZEOF_VAR(v64)",   MY_SIZEOF_VAR(v64),   sizeof(v64));
    chk_sz("void *",   "MY_SIZEOF_VAR(vp)",    MY_SIZEOF_VAR(vp),    sizeof(vp));
    chk_sz("int[10]",  "MY_SIZEOF_VAR(arr10)", MY_SIZEOF_VAR(arr10), sizeof(arr10));
    chk_sz("struct",   "MY_SIZEOF_VAR(vs)",    MY_SIZEOF_VAR(vs),    sizeof(vs));

    nlc_end();
    return 0;
}

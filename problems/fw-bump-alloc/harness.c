#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "nlc.h"
#include "solution.c"

static void chk_notnull(const char *nm, const char *in, void *p) {
    nlc_case(nm, p != NULL, in, "non-NULL", p ? "non-NULL" : "NULL", "");
}
static void chk_null(const char *nm, const char *in, void *p) {
    nlc_case(nm, p == NULL, in, "NULL", p ? "non-NULL" : "NULL", "");
}
static void chk_align(const char *nm, const char *in, void *p, size_t align) {
    int ok = ((uintptr_t)p % align) == 0;
    char g[32], e[32];
    snprintf(g, sizeof g, "ptr%%%" PRIuPTR "=%u", (uintptr_t)align, (unsigned)((uintptr_t)p % align));
    snprintf(e, sizeof e, "0");
    nlc_case(nm, ok, in, e, g, "");
}
static void chk_eq(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "same ptr", got == exp ? "same ptr" : "diff ptr", "");
}

int main(void) {
    nlc_begin();

    static uint8_t mem[128] __attribute__((aligned(16)));
    bump_alloc_t a;

    /* alloc with various alignments */
    bump_init(&a, mem, 128);
    void *p1 = bump_alloc_aligned(&a, 1, 1);
    chk_notnull("align1 ok",  "alloc(1,1)",  p1);
    chk_align("align1 mod1", "ptr%1", p1, 1);

    void *p4 = bump_alloc_aligned(&a, 4, 4);
    chk_notnull("align4 ok", "alloc(4,4)", p4);
    chk_align("align4 mod4", "ptr%4", p4, 4);

    void *p8 = bump_alloc_aligned(&a, 8, 8);
    chk_notnull("align8 ok", "alloc(8,8)", p8);
    chk_align("align8 mod8", "ptr%8", p8, 8);

    void *p16 = bump_alloc_aligned(&a, 16, 16);
    chk_notnull("align16 ok", "alloc(16,16)", p16);
    chk_align("align16 mod16", "ptr%16", p16, 16);

    /* exhaustion */
    bump_init(&a, mem, 16);
    bump_alloc_aligned(&a, 12, 1);
    void *px = bump_alloc_aligned(&a, 8, 1);
    chk_null("exhausted NULL", "alloc when not enough space", px);

    /* reset and reuse */
    bump_init(&a, mem, 32);
    void *r1 = bump_alloc_aligned(&a, 32, 1);
    bump_reset(&a);
    void *r2 = bump_alloc_aligned(&a, 32, 1);
    chk_eq("reset reuse same ptr", "ptr after reset", r2, r1);

    nlc_end();
    return 0;
}

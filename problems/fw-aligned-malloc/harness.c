#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static void chk_align(const char *nm, const char *in, void *ptr, size_t align) {
    int ok = ptr != NULL && ((uintptr_t)ptr % align) == 0;
    char g[32], e[32];
    snprintf(e, sizeof e, "non-NULL, ptr%%align==0 (align=%zu)", align);
    if (!ptr)
        snprintf(g, sizeof g, "NULL");
    else
        snprintf(g, sizeof g, "ptr%%%zu==%zu", align, (size_t)((uintptr_t)ptr % align));
    nlc_case(nm, ok, in, e, g, "");
}
static void chk_data(const char *nm, const char *in, int ok) {
    nlc_case(nm, ok, in, "data intact", ok ? "data intact" : "data corrupted", "");
}

int main(void) {
    nlc_begin();

    /* basic alignment checks */
    void *p4  = aligned_malloc(16, 4);
    chk_align("align 4",  "aligned_malloc(16, 4)",  p4,  4);

    void *p8  = aligned_malloc(16, 8);
    chk_align("align 8",  "aligned_malloc(16, 8)",  p8,  8);

    void *p16 = aligned_malloc(16, 16);
    chk_align("align 16", "aligned_malloc(16, 16)", p16, 16);

    void *p64 = aligned_malloc(128, 64);
    chk_align("align 64", "aligned_malloc(128, 64)", p64, 64);

    void *p256 = aligned_malloc(1, 256);
    chk_align("align 256", "aligned_malloc(1, 256)", p256, 256);

    /* data survives write + read */
    if (p16) {
        memset(p16, 0xAB, 16);
        unsigned char buf[16];
        memcpy(buf, p16, 16);
        int ok = 1;
        for (int i = 0; i < 16; i++) if (buf[i] != 0xAB) { ok = 0; break; }
        chk_data("data survives write/read", "memset 0xAB then memcpy", ok);
    }

    /* align=1 always succeeds (trivially aligned) */
    void *p1 = aligned_malloc(8, 1);
    chk_align("align 1", "aligned_malloc(8, 1)", p1, 1);

    /* free does not crash */
    aligned_free(p4);
    aligned_free(p8);
    aligned_free(p16);
    aligned_free(p64);
    aligned_free(p256);
    aligned_free(p1);
    nlc_case("aligned_free no crash", 1, "aligned_free all pointers", "ok", "ok", "");

    /* NULL free is safe */
    aligned_free(NULL);
    nlc_case("free NULL safe", 1, "aligned_free(NULL)", "ok", "ok", "");

    /* multiple allocs are independent */
    void *a = aligned_malloc(32, 16);
    void *b = aligned_malloc(32, 16);
    chk_align("two allocs: a aligned", "aligned_malloc(32,16) x2: a", a, 16);
    chk_align("two allocs: b aligned", "aligned_malloc(32,16) x2: b", b, 16);
    nlc_case("two allocs: different ptrs", a != b,
             "two independent allocs", "different ptrs",
             a != b ? "different ptrs" : "same ptr", "");
    aligned_free(a);
    aligned_free(b);

    nlc_end();
    return 0;
}

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}
static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_notnull(const char *nm, const char *in, void *got) {
    nlc_case(nm, got != NULL, in, "non-NULL", got ? "non-NULL" : "NULL", "");
}
static void chk_null(const char *nm, const char *in, void *got) {
    nlc_case(nm, got == NULL, in, "NULL", got ? "non-NULL" : "NULL", "");
}

int main(void) {
    nlc_begin();

    /* Must be aligned for pointer storage */
    static uint8_t mem[128] __attribute__((aligned(8)));
    pool_t pool;

    /* bad init: block_size < sizeof(void*) */
    chk_bool("init bad block_size", "pool_init block<ptr", pool_init(&pool, mem, 128, 1), false);

    /* good init: 4 blocks of 16 bytes */
    chk_bool("init ok", "pool_init 4x16", pool_init(&pool, mem, 64, 16), true);
    chk_sz("init free_count=4", "pool_free_count after init", pool_free_count(&pool), 4);

    /* alloc all */
    void *p[4];
    for (int i = 0; i < 4; i++) {
        p[i] = pool_alloc(&pool);
        chk_notnull("alloc non-null", "pool_alloc", p[i]);
    }
    chk_sz("after alloc all free=0", "pool_free_count=0", pool_free_count(&pool), 0);

    /* exhaustion */
    chk_null("exhausted NULL", "pool_alloc when empty", pool_alloc(&pool));

    /* free one and reuse */
    chk_bool("free ok", "pool_free p[0]", pool_free(&pool, p[0]), true);
    chk_sz("after free free=1", "pool_free_count=1", pool_free_count(&pool), 1);
    void *p2 = pool_alloc(&pool);
    chk_notnull("reuse after free", "pool_alloc after free", p2);
    chk_sz("after realloc free=0", "pool_free_count=0 after reuse", pool_free_count(&pool), 0);

    /* NULL free returns false */
    chk_bool("free NULL false", "pool_free(NULL)", pool_free(&pool, NULL), false);

    nlc_end();
    return 0;
}

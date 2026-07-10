#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "nlc.h"
#include "solution.c"

int main(void) {
    nlc_begin();

    size_t got = struct_size_without_sizeof();
    size_t exp = sizeof(struct test_struct);

    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);

    nlc_case("matches sizeof", got == exp,
             "struct_size_without_sizeof()", e, g, "");

    /* Sanity: padding must exist — naive field sum is smaller */
    size_t naive = sizeof(char) + sizeof(uint32_t) + sizeof(uint16_t)
                 + sizeof(double) + sizeof(char);
    nlc_case("includes padding", got > naive,
             "size > sum of fields (padding present)",
             "yes", got > naive ? "yes" : "no", "");

    nlc_end();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void check_pow(const char* name, uint32_t x, bool exp) {
    char input[32];
    snprintf(input, sizeof input, "is_power_of_two(%u)", x);
    nlc_capture_begin();
    bool got = is_power_of_two(x);
    char* out = nlc_capture_end();
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
    free(out);
}
static void check_round(const char* name, uint32_t x, uint32_t exp) {
    char input[32], g[16], e[16];
    snprintf(input, sizeof input, "round_up_pow2(%u)", x);
    nlc_capture_begin();
    uint32_t got = round_up_pow2(x);
    char* out = nlc_capture_end();
    snprintf(g, sizeof g, "%u", got);
    snprintf(e, sizeof e, "%u", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check_pow("pow 1", 1u, true);
    check_pow("pow 16", 16u, true);
    check_pow("pow 0", 0u, false);
    check_pow("pow 6", 6u, false);
    check_pow("pow 2^31", 0x80000000u, true);
    check_round("round 0", 0u, 1u);
    check_round("round 1", 1u, 1u);
    check_round("round 5", 5u, 8u);
    check_round("round 16", 16u, 16u);
    check_round("round 17", 17u, 32u);
    check_round("round 1000", 1000u, 1024u);
    nlc_end();
    return 0;
}

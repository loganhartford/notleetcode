#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_u(const char *nm, const char *in, unsigned got, unsigned exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%u", got);
    snprintf(e, sizeof e, "%u", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_u32(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* popcount */
    chk_u("popcount(0)",          "popcount32(0)",          popcount32(0),          0);
    chk_u("popcount(0xFFFFFFFF)", "popcount32(0xFFFFFFFF)", popcount32(0xFFFFFFFFu),32);
    chk_u("popcount(0x55555555)", "popcount32(0x55555555)", popcount32(0x55555555u),16);
    chk_u("popcount(0x00000007)", "popcount32(7)",          popcount32(7u),          3);
    chk_u("popcount(1)",          "popcount32(1)",          popcount32(1u),          1);

    /* reverse_bits */
    chk_u32("rev(0x80000000)", "reverse_bits32(0x80000000)", reverse_bits32(0x80000000u), 0x00000001u);
    chk_u32("rev(0x00000001)", "reverse_bits32(0x00000001)", reverse_bits32(0x00000001u), 0x80000000u);
    chk_u32("rev(0xFFFFFFFF)", "reverse_bits32(0xFFFFFFFF)", reverse_bits32(0xFFFFFFFFu), 0xFFFFFFFFu);
    chk_u32("rev(0x12345678)", "reverse_bits32(0x12345678)", reverse_bits32(0x12345678u), 0x1E6A2C48u);

    /* rotl32 */
    chk_u32("rotl(1,1)",          "rotl32(1,1)",          rotl32(1u, 1),          0x00000002u);
    chk_u32("rotl(0x80000000,1)", "rotl32(0x80000000,1)", rotl32(0x80000000u, 1), 0x00000001u);
    chk_u32("rotl(x,0)",          "rotl32(0xABCDEF01,0)", rotl32(0xABCDEF01u, 0), 0xABCDEF01u);
    chk_u32("rotl(x,32)",         "rotl32(0xABCDEF01,32)",rotl32(0xABCDEF01u,32), 0xABCDEF01u);

    /* rotr32 */
    chk_u32("rotr(1,1)",          "rotr32(1,1)",          rotr32(1u, 1),          0x80000000u);
    chk_u32("rotr(0x00000002,1)", "rotr32(2,1)",          rotr32(2u, 1),          0x00000001u);
    chk_u32("rotr(x,0)",          "rotr32(0xABCDEF01,0)", rotr32(0xABCDEF01u, 0), 0xABCDEF01u);

    nlc_end();
    return 0;
}

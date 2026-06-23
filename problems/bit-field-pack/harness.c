#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void case_u8(const char* name, const char* input, uint8_t got, uint8_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%02X", got);
    snprintf(e, sizeof e, "0x%02X", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

int main(void) {
    nlc_begin();
    case_u8("pack A,5", "pack_nibbles(0xA, 0x5)", pack_nibbles(0xA, 0x5), 0xA5);
    case_u8("pack 1,2", "pack_nibbles(0x1, 0x2)", pack_nibbles(0x1, 0x2), 0x12);
    case_u8("pack masks hi", "pack_nibbles(0xFF, 0x3)", pack_nibbles(0xFF, 0x3), 0xF3);
    case_u8("pack masks lo", "pack_nibbles(0x4, 0xFF)", pack_nibbles(0x4, 0xFF), 0x4F);
    case_u8("unpack hi", "unpack_hi(0xA5)", unpack_hi(0xA5), 0xA);
    case_u8("unpack lo", "unpack_lo(0xA5)", unpack_lo(0xA5), 0x5);
    case_u8("unpack hi 0x12", "unpack_hi(0x12)", unpack_hi(0x12), 0x1);
    case_u8("unpack lo 0x12", "unpack_lo(0x12)", unpack_lo(0x12), 0x2);
    case_u8("round trip", "unpack_hi(pack(0x7,0xC))", unpack_hi(pack_nibbles(0x7, 0xC)), 0x7);
    nlc_end();
    return 0;
}

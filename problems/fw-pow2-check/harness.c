#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    chk("0 is not pow2",        "is_power_of_two_u32(0)",          is_power_of_two_u32(0),          false);
    chk("1 is pow2",            "is_power_of_two_u32(1)",          is_power_of_two_u32(1),          true);
    chk("2 is pow2",            "is_power_of_two_u32(2)",          is_power_of_two_u32(2),          true);
    chk("3 is not pow2",        "is_power_of_two_u32(3)",          is_power_of_two_u32(3),          false);
    chk("4 is pow2",            "is_power_of_two_u32(4)",          is_power_of_two_u32(4),          true);
    chk("32 is pow2",           "is_power_of_two_u32(32)",         is_power_of_two_u32(32),         true);
    chk("0x80000000 is pow2",   "is_power_of_two_u32(0x80000000)", is_power_of_two_u32(0x80000000u),true);
    chk("0x80000001 not pow2",  "is_power_of_two_u32(0x80000001)", is_power_of_two_u32(0x80000001u),false);
    chk("6 is not pow2",        "is_power_of_two_u32(6)",          is_power_of_two_u32(6),          false);
    chk("1024 is pow2",         "is_power_of_two_u32(1024)",       is_power_of_two_u32(1024),       true);

    nlc_end();
    return 0;
}

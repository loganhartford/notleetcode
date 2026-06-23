#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void case_u32(const char* name, const char* input, uint32_t got, uint32_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%u", got);
    snprintf(e, sizeof e, "%u", exp);
    nlc_case(name, got == exp, input, e, g, "");
}
static void case_int(const char* name, const char* input, int got, int exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

int main(void) {
    nlc_begin();
    uint32_t r = 0;
    SET_BIT(r, 3);   case_u32("SET_BIT(r,3)", "r=0; SET_BIT(r,3)", r, 8u);
    SET_BIT(r, 0);   case_u32("SET_BIT(r,0)", "SET_BIT(r,0)", r, 9u);
    case_int("TEST_BIT(r,3)", "TEST_BIT(r,3)", (int)TEST_BIT(r, 3), 1);
    case_int("TEST_BIT(r,1)", "TEST_BIT(r,1)", (int)TEST_BIT(r, 1), 0);
    CLEAR_BIT(r, 3); case_u32("CLEAR_BIT(r,3)", "CLEAR_BIT(r,3)", r, 1u);
    TOGGLE_BIT(r, 5); case_u32("TOGGLE_BIT(r,5)", "TOGGLE_BIT(r,5)", r, 33u);
    TOGGLE_BIT(r, 5); case_u32("TOGGLE_BIT(r,5) again", "TOGGLE_BIT(r,5)", r, 1u);
    case_int("TEST_BIT(r,0)", "TEST_BIT(r,0)", (int)TEST_BIT(r, 0), 1);
    SET_BIT(r, 31);  case_u32("SET_BIT(r,31)", "SET_BIT(r,31)", r, 0x80000001u);
    nlc_end();
    return 0;
}

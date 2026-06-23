#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, const char* input, const uint8_t* data, size_t len, uint8_t exp) {
    nlc_capture_begin();
    uint8_t got = crc8(data, len);
    char* out = nlc_capture_end();
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%02X", got);
    snprintf(e, sizeof e, "0x%02X", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { uint8_t d[] = "123456789"; check("check value", "\"123456789\"", d, 9, 0xF4); }
    { check("empty", "\"\"", (const uint8_t*)"", 0, 0x00); }
    { uint8_t d[] = {0x00}; check("single zero", "{0x00}", d, 1, 0x00); }
    { uint8_t d[] = {0xFF}; check("single 0xFF", "{0xFF}", d, 1, 0xF3); }
    { uint8_t d[] = {0xDE, 0xAD, 0xBE, 0xEF}; check("deadbeef", "{DE AD BE EF}", d, 4, 0xCA); }
    nlc_end();
    return 0;
}

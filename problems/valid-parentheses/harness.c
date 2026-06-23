#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, char* s, const char* input, bool exp) {
    nlc_capture_begin();
    bool got = isValid(s);
    char* out = nlc_capture_end();
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("simple pair", "()", "s=\"()\"", true);
    check("all types", "()[]{}", "s=\"()[]{}\"", true);
    check("mismatch", "(]", "s=\"(]\"", false);
    check("wrong order", "([)]", "s=\"([)]\"", false);
    check("nested", "{[]}", "s=\"{[]}\"", true);
    check("unclosed", "(", "s=\"(\"", false);
    check("extra close", "){", "s=\"){\"", false);
    nlc_end();
    return 0;
}

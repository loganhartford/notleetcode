#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_str(const char *nm, const char *in, const char *got, const char *exp) {
    nlc_case(nm, strcmp(got, exp) == 0, in, exp, got, "");
}

int main(void) {
    nlc_begin();

    char *argv[8];
    char line[64];
    size_t n;

    /* empty → 0 */
    strcpy(line, "");
    n = parse_tokens(line, argv, 8);
    chk_sz("empty → 0", "parse_tokens(\"\")", n, 0);

    /* single token */
    strcpy(line, "cmd");
    n = parse_tokens(line, argv, 8);
    chk_sz("single token count", "parse_tokens(\"cmd\")", n, 1);
    chk_str("single token value", "argv[0]", argv[0], "cmd");

    /* three tokens */
    strcpy(line, "cmd arg1 arg2");
    n = parse_tokens(line, argv, 8);
    chk_sz("three tokens count", "parse_tokens 3 tokens", n, 3);
    chk_str("tok0", "argv[0]", argv[0], "cmd");
    chk_str("tok1", "argv[1]", argv[1], "arg1");
    chk_str("tok2", "argv[2]", argv[2], "arg2");

    /* multiple spaces */
    strcpy(line, "  cmd   arg  ");
    n = parse_tokens(line, argv, 8);
    chk_sz("multi-space count", "parse_tokens multi-space", n, 2);
    chk_str("multi-space tok0", "argv[0]", argv[0], "cmd");
    chk_str("multi-space tok1", "argv[1]", argv[1], "arg");

    /* tab separator */
    strcpy(line, "a\tb\tc");
    n = parse_tokens(line, argv, 8);
    chk_sz("tab sep count", "parse_tokens tab", n, 3);
    chk_str("tab tok0", "argv[0]", argv[0], "a");

    /* max_args limit */
    strcpy(line, "a b c d e");
    n = parse_tokens(line, argv, 2);
    chk_sz("max_args cap", "parse_tokens max_args=2", n, 2);
    chk_str("cap tok0", "argv[0]", argv[0], "a");
    chk_str("cap tok1", "argv[1]", argv[1], "b");

    nlc_end();
    return 0;
}

// Time: O(n) — single pass through the string
#include <stddef.h>

static int is_ws(char c) { return c == ' ' || c == '\t'; }

size_t parse_tokens(char *line, char **argv, size_t max_args) {
    size_t n = 0;
    char *p = line;
    while (*p && n < max_args) {
        /* skip whitespace */
        while (*p && is_ws(*p)) p++;
        if (!*p) break;
        /* start of token */
        argv[n++] = p;
        /* scan to end of token */
        while (*p && !is_ws(*p)) p++;
        /* null-terminate */
        if (*p) *p++ = '\0';
    }
    return n;
}

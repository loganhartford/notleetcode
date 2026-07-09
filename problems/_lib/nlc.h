/* Shared C test-harness helper, injected into the run sandbox as nlc.h.
 *
 * A problem's harness.c does:
 *     #include "nlc.h"
 *     #include "solution.c"
 *     int main(void){ nlc_begin(); ... nlc_case(...); ... nlc_end(); }
 *
 * The JSON result protocol is written to stderr so that printf/fprintf(stdout)
 * calls in the user's solution flow to stdout uninterrupted and appear as
 * debug output in the Results panel.
 */
#ifndef NLC_H
#define NLC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _nlc_first = 1;

static void nlc_begin(void) { fputs("__TESTS_BEGIN__\n{\"results\":[", stderr); }
static void nlc_end(void)   { fputs("]}\n__TESTS_END__\n", stderr); fflush(stderr); }

static void _nlc_json_str(const char *s) {
    fputc('"', stderr);
    if (s) {
        for (; *s; s++) {
            unsigned char c = (unsigned char)*s;
            if (c == '"' || c == '\\') { fputc('\\', stderr); fputc(c, stderr); }
            else if (c == '\n') fputs("\\n", stderr);
            else if (c == '\r') fputs("\\r", stderr);
            else if (c == '\t') fputs("\\t", stderr);
            else if (c < 0x20) fprintf(stderr, "\\u%04x", c);
            else fputc(c, stderr);
        }
    }
    fputc('"', stderr);
}

static void nlc_case(const char *name, int passed, const char *input,
                     const char *expected, const char *got, const char *out) {
    if (!_nlc_first) fputc(',', stderr);
    _nlc_first = 0;
    fputs("{\"name\":", stderr);     _nlc_json_str(name);
    fputs(",\"passed\":", stderr);   fputs(passed ? "true" : "false", stderr);
    fputs(",\"input\":", stderr);    _nlc_json_str(input);
    fputs(",\"expected\":", stderr); _nlc_json_str(expected);
    fputs(",\"got\":", stderr);      _nlc_json_str(got);
    if (out && out[0]) { fputs(",\"stdout\":", stderr); _nlc_json_str(out); }
    fputc('}', stderr);
}

/* Format an int array as "[1, 2, 3]" into buf (caller-provided). */
static const char *nlc_fmt_ints(char *buf, size_t cap, const int *a, int n) {
    size_t off = 0;
    off += (size_t)snprintf(buf + off, cap - off, "[");
    for (int i = 0; i < n && off < cap; i++)
        off += (size_t)snprintf(buf + off, cap - off, "%s%d", i ? ", " : "", a[i]);
    snprintf(buf + off, cap - off, "]");
    return buf;
}

#endif /* NLC_H */

/* Shared C test-harness helper, injected into the run sandbox as nlc.h.
 *
 * A problem's harness.c does:
 *     #include "nlc.h"
 *     #include "solution.c"
 *     int main(void){ nlc_begin(); ... nlc_case(...); ... nlc_end(); }
 *
 * It centralizes the __TESTS_BEGIN__/__TESTS_END__ JSON protocol, JSON string
 * escaping, per-test stdout capture (so each test's printf output stays with
 * that test), and small array-formatting helpers.
 *
 * (A hard crash in the code under test kills the process before nlc_end(), so
 * no result block is emitted and the server surfaces stderr instead.)
 */
#ifndef NLC_H
#define NLC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int _nlc_first = 1;
static int _nlc_saved_fd = -1;
static FILE *_nlc_tmp = NULL;

static void nlc_begin(void) { fputs("__TESTS_BEGIN__\n{\"results\":[", stdout); }
static void nlc_end(void) { fputs("]}\n__TESTS_END__\n", stdout); }

static void _nlc_json_str(const char *s) {
    putchar('"');
    if (s) {
        for (; *s; s++) {
            unsigned char c = (unsigned char)*s;
            if (c == '"' || c == '\\') { putchar('\\'); putchar(c); }
            else if (c == '\n') fputs("\\n", stdout);
            else if (c == '\r') fputs("\\r", stdout);
            else if (c == '\t') fputs("\\t", stdout);
            else if (c < 0x20) printf("\\u%04x", c);
            else putchar(c);
        }
    }
    putchar('"');
}

/* Capture stdout emitted by the code under test, between begin and end. */
static void nlc_capture_begin(void) {
    fflush(stdout);
    _nlc_saved_fd = dup(fileno(stdout));
    _nlc_tmp = tmpfile();
    if (_nlc_tmp) dup2(fileno(_nlc_tmp), fileno(stdout));
}

/* Restore real stdout, return malloc'd captured text (caller frees). */
static char *nlc_capture_end(void) {
    char *buf = NULL;
    long n = 0;
    fflush(stdout);
    if (_nlc_saved_fd >= 0) {
        dup2(_nlc_saved_fd, fileno(stdout));
        close(_nlc_saved_fd);
        _nlc_saved_fd = -1;
    }
    if (_nlc_tmp) {
        fflush(_nlc_tmp);
        fseek(_nlc_tmp, 0, SEEK_END);
        n = ftell(_nlc_tmp);
        fseek(_nlc_tmp, 0, SEEK_SET);
        buf = (char *)malloc(n + 1);
        if (buf) {
            size_t r = fread(buf, 1, (size_t)n, _nlc_tmp);
            buf[r] = '\0';
        }
        fclose(_nlc_tmp);
        _nlc_tmp = NULL;
    }
    if (!buf) { buf = (char *)malloc(1); if (buf) buf[0] = '\0'; }
    return buf;
}

static void nlc_case(const char *name, int passed, const char *input,
                     const char *expected, const char *got, const char *out) {
    if (!_nlc_first) putchar(',');
    _nlc_first = 0;
    fputs("{\"name\":", stdout); _nlc_json_str(name);
    fputs(",\"passed\":", stdout); fputs(passed ? "true" : "false", stdout);
    fputs(",\"input\":", stdout); _nlc_json_str(input);
    fputs(",\"expected\":", stdout); _nlc_json_str(expected);
    fputs(",\"got\":", stdout); _nlc_json_str(got);
    if (out && out[0]) { fputs(",\"stdout\":", stdout); _nlc_json_str(out); }
    putchar('}');
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

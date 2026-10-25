#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

/* Format exactly n bytes, including bytes after a terminator. Never read the
 * user's result as a C string: missing terminators are part of what we test. */
static void format_bytes(char *out, size_t size, const unsigned char *bytes, size_t n) {
    size_t used = 0;
    out[used++] = '"';
    for (size_t i = 0; i < n && used + 5 < size; ++i) {
        unsigned char c = bytes[i];
        if (c == 0) { out[used++] = '\\'; out[used++] = '0'; }
        else if (c == '"' || c == '\\') { out[used++] = '\\'; out[used++] = (char)c; }
        else if (c >= 32 && c <= 126) out[used++] = (char)c;
        else used += (size_t)snprintf(out + used, size - used, "\\x%02X", c);
    }
    out[used++] = '"';
    out[used] = 0;
}

static void check(const char *name, size_t cap, const char *src,
                  size_t source_bytes, size_t expected_length, const char *expected) {
    unsigned char actual[32];
    memset(actual, 0xA5, sizeof actual);
    for (size_t i = 0; i < cap; ++i) actual[i + 1] = 'X';
    size_t got = c_strlcpy((char *)actual + 1, src, cap);
    size_t checked = cap ? (expected_length < cap ? expected_length : cap - 1) + 1 : 0;
    int passed = got == expected_length && memcmp(actual + 1, expected, checked) == 0;
    char source[160], got_bytes[160], expected_bytes[160];
    format_bytes(source, sizeof source, (const unsigned char *)src, source_bytes);
    format_bytes(got_bytes, sizeof got_bytes, actual + 1, checked);
    format_bytes(expected_bytes, sizeof expected_bytes, (const unsigned char *)expected, checked);
    char input[384], output[512], exp[256];
    snprintf(input, sizeof input, "c_strlcpy(dst, src bytes=%s, dstsize=%zu), initial dst: %zu 'X' bytes", source, cap, cap);
    snprintf(output, sizeof output, "return=%zu, result bytes=%s", got, got_bytes);
    snprintf(exp, sizeof exp, "return=%zu, result bytes=%s", expected_length, expected_bytes);
    for (size_t i = 0; i < sizeof actual; ++i) {
        if ((i == 0 || i > cap) && actual[i] != 0xA5) {
            size_t used = strlen(output);
            snprintf(output + used, sizeof output - used,
                     "; out-of-bounds write: dst[%td]=0x%02X (expected 0xA5)",
                     (ptrdiff_t)i - 1, actual[i]);
            passed = 0;
            break;
        }
    }
    nlc_case(name, passed, input, exp, output, "");
}

int main(void) {
    nlc_begin();
    check("exact fit", 4, "cat", 4, 3, "cat");
    check("room left over", 8, "hi", 3, 2, "hi");
    check("truncate long source", 4, "hello", 6, 5, "hel");
    check("one byte", 1, "hello", 6, 5, "");
    check("zero capacity", 0, "hello", 6, 5, "");
    check("empty source", 4, "", 1, 0, "");
    check("empty into one byte", 1, "", 1, 0, "");
    check("empty with zero capacity", 0, "", 1, 0, "");
    check("stop at first NUL", 8, "a\0ignored", 10, 1, "a");
    check("one byte short of exact fit", 3, "cat", 4, 3, "ca");
    nlc_end();
    return 0;
}

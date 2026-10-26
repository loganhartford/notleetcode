#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

#define H_PRE 8
#define H_REGION 160
#define H_CANARY 0xA5
#define H_FILL 'X'

static unsigned char h_region[H_REGION];

static char *h_prepare(size_t cap) {
    memset(h_region, H_CANARY, sizeof h_region);
    memset(h_region + H_PRE, H_FILL, cap);
    return (char *)h_region + H_PRE;
}

/* Quote exactly n bytes, including any after a terminator. Never read the
 * user's buffer as a C string: a missing terminator is part of what we test. */
static void h_quote(char *out, size_t size, const unsigned char *bytes, size_t n) {
    size_t used = 0;
    out[used++] = '"';
    for (size_t i = 0; i < n && used + 6 < size; ++i) {
        unsigned char c = bytes[i];
        if (c == 0) { out[used++] = '\\'; out[used++] = '0'; }
        else if (c == '"' || c == '\\') { out[used++] = '\\'; out[used++] = (char)c; }
        else if (c >= 32 && c <= 126) out[used++] = (char)c;
        else used += (size_t)snprintf(out + used, size - used, "\\x%02X", c);
    }
    out[used++] = '"';
    out[used] = 0;
}

static void h_report(const char *name, const char *call, size_t cap, int null_buf,
                     const char *full, int exp_ret, int got_ret) {
    size_t full_len = strlen(full);
    size_t keep = cap ? (full_len < cap ? full_len : cap - 1) : 0;
    size_t window = cap ? keep + 1 : 0;
    unsigned char expected[H_REGION];
    memcpy(expected, full, keep);
    expected[keep] = 0;
    const unsigned char *got = h_region + H_PRE;

    int passed = got_ret == exp_ret && memcmp(got, expected, window) == 0;

    char input[384], exp[512], out[640], qexp[256], qgot[256], qfull[256];
    if (null_buf)
        snprintf(input, sizeof input, "%s", call);
    else if (cap)
        snprintf(input, sizeof input, "%s   [buf: %zu byte%s, prefilled with 'X']",
                 call, cap, cap == 1 ? "" : "s");
    else
        snprintf(input, sizeof input, "%s   [buf: 0 bytes]", call);

    h_quote(qexp, sizeof qexp, expected, window);
    h_quote(qgot, sizeof qgot, got, window);
    h_quote(qfull, sizeof qfull, (const unsigned char *)full, full_len);
    if (null_buf) {
        snprintf(exp, sizeof exp, "return=%d, nothing written", exp_ret);
        snprintf(out, sizeof out, "return=%d", got_ret);
    } else if (cap == 0) {
        snprintf(exp, sizeof exp, "return=%d, buf untouched", exp_ret);
        snprintf(out, sizeof out, "return=%d", got_ret);
    } else {
        snprintf(exp, sizeof exp, "return=%d, buf=%s", exp_ret, qexp);
        if (full_len >= cap) {
            size_t used = strlen(exp);
            snprintf(exp + used, sizeof exp - used, "   (untruncated: %s)", qfull);
        }
        snprintf(out, sizeof out, "return=%d, buf=%s", got_ret, qgot);
    }

    for (size_t i = 0; i < H_REGION; ++i) {
        if (i >= H_PRE && i < H_PRE + cap) continue;
        if (h_region[i] != H_CANARY) {
            size_t used = strlen(out);
            snprintf(out + used, sizeof out - used,
                     "; out-of-bounds write: buf[%td]=0x%02X (cap is %zu)",
                     (ptrdiff_t)i - H_PRE, h_region[i], cap);
            passed = 0;
            break;
        }
    }
    nlc_case(name, passed, input, exp, out, "");
}

/* Expected output and return value come from the C library's snprintf. */
#define H_CASE(name, cap, ...) do {                                        \
    char h_full[128];                                                      \
    int h_exp = snprintf(h_full, sizeof h_full, __VA_ARGS__);              \
    char *h_buf = h_prepare(cap);                                          \
    int h_got = mini_snprintf(h_buf, (cap), __VA_ARGS__);                  \
    h_report(name, "mini_snprintf(buf, " #cap ", " #__VA_ARGS__ ")",       \
             (cap), 0, h_full, h_exp, h_got);                              \
} while (0)

/* For arguments libc must not be handed (a NULL %s): fixed expectation. */
#define H_CASE_FIXED(name, cap, full, ...) do {                            \
    char *h_buf = h_prepare(cap);                                          \
    int h_got = mini_snprintf(h_buf, (cap), __VA_ARGS__);                  \
    h_report(name, "mini_snprintf(buf, " #cap ", " #__VA_ARGS__ ")",       \
             (cap), 0, full, (int)strlen(full), h_got);                    \
} while (0)

#define H_CASE_NULL_BUF(name, ...) do {                                    \
    char h_full[128];                                                      \
    int h_exp = snprintf(h_full, sizeof h_full, __VA_ARGS__);              \
    h_prepare(0);                                                          \
    int h_got = mini_snprintf(NULL, 0, __VA_ARGS__);                       \
    h_report(name, "mini_snprintf(NULL, 0, " #__VA_ARGS__ ")", 0, 1,       \
             h_full, h_exp, h_got);                                        \
} while (0)

int main(void) {
    nlc_begin();

    H_CASE("plain text", 32, "hello, world");
    H_CASE("%d positive", 32, "%d", 42);
    H_CASE("%d negative", 32, "%d", -42);
    H_CASE("%d zero", 32, "%d", 0);
    H_CASE("%d INT_MAX", 32, "%d", INT_MAX);
    H_CASE("%d INT_MIN", 32, "%d", INT_MIN);
    H_CASE("%u", 32, "%u", 3000000000u);
    H_CASE("%u zero", 32, "%u", 0u);
    H_CASE("%u UINT_MAX", 32, "%u", UINT_MAX);
    H_CASE("%x lowercase", 32, "%x", 0xdeadbeefu);
    H_CASE("%x zero", 32, "%x", 0u);
    H_CASE("%x UINT_MAX", 32, "%x", UINT_MAX);
    H_CASE("%c", 32, "[%c]", 'A');
    H_CASE("%s", 32, "name=%s", "sensor");
    H_CASE("%s empty string", 32, "<%s>", "");
    H_CASE_FIXED("%s NULL", 32, "<(null)>", "<%s>", (const char *)NULL);
    H_CASE("%%", 32, "100%%");
    H_CASE("%% next to a conversion", 32, "%d%%", 50);
    H_CASE("width pads with spaces", 32, "[%4d]", 42);
    H_CASE("width, negative, spaces", 32, "[%6d]", -42);
    H_CASE("negative exactly fills width", 32, "[%3d]", -42);
    H_CASE("0 flag pads with zeros", 32, "[%08x]", 0xc0ffeeu);
    H_CASE("0 flag, negative: sign first", 32, "%05d", -42);
    H_CASE("0 flag, zero value", 32, "%03d", 0);
    H_CASE("value wider than width", 32, "%2u", 12345u);
    H_CASE("clock format", 32, "%02u:%02u:%02u", 7u, 5u, 30u);
    H_CASE("several conversions", 64, "id=%u temp=%d raw=0x%04x tag=%c name=%s %d%%",
           7u, -15, 0x2au, 'k', "imu", 99);
    H_CASE("truncate mid conversion", 5, "value=%d", 12345);
    H_CASE("truncate inside padding", 4, "%8d", 7);
    H_CASE("truncate inside %s", 8, "name=%s", "sensor");
    H_CASE("exact fit", 12, "value=%d", 12345);
    H_CASE("one byte short of exact fit", 11, "value=%d", 12345);
    H_CASE("cap 1: only the NUL", 1, "abc%d", 5);
    H_CASE("cap 0: buf untouched", 0, "value=%d", 12345);
    H_CASE_NULL_BUF("cap 0 with NULL buf", "%s=%c", "mode", 'A');

    nlc_end();
    return 0;
}

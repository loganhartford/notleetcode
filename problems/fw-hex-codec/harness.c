#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

/* out points H_GUARD bytes into h_buf. Bytes out[0..out_cap) start as H_FILL;
 * every other byte of h_buf is a canary that must still read H_CANARY after
 * the call, which catches writes before out[0] or past out[out_cap - 1]. */
#define H_GUARD 16
#define H_MAX 1024
#define H_FILL 0xCC
#define H_CANARY 0xA5
#define H_LIT(s) s, sizeof(s) - 1

static uint8_t h_buf[H_GUARD + H_MAX + H_GUARD];
static char h_hex_lower[513];
static char h_hex_upper[513];
static uint8_t h_all_bytes[256];

static uint8_t *h_prepare(size_t cap) {
    memset(h_buf, H_CANARY, sizeof h_buf);
    memset(h_buf + H_GUARD, H_FILL, cap);
    return h_buf + H_GUARD;
}

/* Quote exactly n bytes, showing NUL and non-printables as escapes. Never
 * treat the user's buffer as a C string: a missing terminator is a bug we test. */
static void h_fmt_chars(char *dst, size_t cap, const uint8_t *s, size_t n) {
    size_t shown = n > 48 ? 40 : n;
    size_t used = 0;
    dst[used++] = '"';
    for (size_t i = 0; i < shown && used + 8 < cap; i++) {
        uint8_t c = s[i];
        if (c == 0) { dst[used++] = '\\'; dst[used++] = '0'; }
        else if (c == '"' || c == '\\') { dst[used++] = '\\'; dst[used++] = (char)c; }
        else if (c >= 32 && c <= 126) dst[used++] = (char)c;
        else used += (size_t)snprintf(dst + used, cap - used, "\\x%02X", c);
    }
    if (shown < n) snprintf(dst + used, cap - used, "...\" (%zu bytes)", n);
    else snprintf(dst + used, cap - used, "\"");
}

static void h_fmt_u8(char *dst, size_t cap, const uint8_t *b, size_t n) {
    size_t shown = n > 16 ? 12 : n;
    size_t used = (size_t)snprintf(dst, cap, "{");
    for (size_t i = 0; i < shown && used < cap; i++)
        used += (size_t)snprintf(dst + used, cap - used, "%s0x%02X", i ? ", " : "", b[i]);
    if (used >= cap) return;
    if (shown < n) snprintf(dst + used, cap - used, ", ...} (%zu bytes)", n);
    else snprintf(dst + used, cap - used, "}");
}

static void h_append(char *dst, size_t cap, const char *fmt, size_t index, unsigned a, unsigned b) {
    size_t used = strlen(dst);
    snprintf(dst + used, cap - used, fmt, index, a, b);
}

static int h_guards_ok(size_t cap, char *res, size_t res_cap) {
    for (size_t i = 0; i < sizeof h_buf; i++) {
        if (i >= H_GUARD && i < H_GUARD + cap) continue;
        if (h_buf[i] != H_CANARY) {
            size_t used = strlen(res);
            snprintf(res + used, res_cap - used,
                     "; out-of-bounds write: out[%td]=0x%02X (canary 0x%02X was overwritten)",
                     (ptrdiff_t)i - H_GUARD, h_buf[i], H_CANARY);
            return 0;
        }
    }
    return 1;
}

static void h_check_decode(const char *name, const char *hex, size_t hex_len, size_t cap,
                           int expected, const uint8_t *want) {
    uint8_t *out = h_prepare(cap);
    int got = hex_to_bytes(hex, out, cap);
    int passed = got == expected;
    char hex_s[256], want_s[256], got_s[256], input[384], exp[384], res[768];

    h_fmt_chars(hex_s, sizeof hex_s, (const uint8_t *)hex, hex_len);
    snprintf(input, sizeof input, "hex_to_bytes(hex=%s, out, out_cap=%zu)", hex_s, cap);

    if (expected >= 0) {
        h_fmt_u8(want_s, sizeof want_s, want, (size_t)expected);
        snprintf(exp, sizeof exp, "return=%d, out=%s", expected, want_s);
    } else {
        snprintf(exp, sizeof exp, "return=%d", expected);
    }

    if (got >= 0) {
        size_t shown = (size_t)got < cap ? (size_t)got : cap;
        h_fmt_u8(got_s, sizeof got_s, out, shown);
        snprintf(res, sizeof res, "return=%d, out=%s", got, got_s);
    } else {
        snprintf(res, sizeof res, "return=%d", got);
    }

    if (passed && expected > 0) {
        for (size_t i = 0; i < (size_t)expected; i++) {
            if (out[i] != want[i]) {
                h_append(res, sizeof res, "; first difference at out[%zu]: expected 0x%02X, got 0x%02X",
                         i, want[i], out[i]);
                passed = 0;
                break;
            }
        }
    }

    if (!h_guards_ok(cap, res, sizeof res)) passed = 0;
    nlc_case(name, passed, input, exp, res, "");
}

/* want == NULL means bytes_to_hex must return -1 and leave out untouched. */
static void h_check_encode(const char *name, const uint8_t *in, size_t n, size_t cap,
                           const char *want) {
    uint8_t *out = h_prepare(cap);
    int got = bytes_to_hex(in, n, (char *)out, cap);
    int expected = want ? (int)strlen(want) : -1;
    size_t region = want ? (size_t)expected + 1 : cap;
    uint8_t untouched[H_MAX];
    const uint8_t *want_bytes = (const uint8_t *)want;
    char in_s[256], want_s[256], got_s[256], input[384], exp[384], res[768];

    if (!want) {
        memset(untouched, H_FILL, cap);
        want_bytes = untouched;
    }

    h_fmt_u8(in_s, sizeof in_s, in, n);
    snprintf(input, sizeof input, "bytes_to_hex(in=%s, n=%zu, out, out_cap=%zu), out prefilled with 0x%02X",
             in_s, n, cap, H_FILL);

    h_fmt_chars(want_s, sizeof want_s, want_bytes, region);
    snprintf(exp, sizeof exp, "return=%d, out=%s", expected, want_s);
    h_fmt_chars(got_s, sizeof got_s, out, region);
    snprintf(res, sizeof res, "return=%d, out=%s", got, got_s);

    int passed = got == expected;
    for (size_t i = 0; i < region; i++) {
        if (out[i] != want_bytes[i]) {
            h_append(res, sizeof res, want
                         ? "; first difference at out[%zu]: expected 0x%02X, got 0x%02X"
                         : "; out[%zu] was written (0x%02X -> 0x%02X) but out_cap is too small",
                     i, want_bytes[i], out[i]);
            passed = 0;
            break;
        }
    }

    if (!h_guards_ok(cap, res, sizeof res)) passed = 0;
    nlc_case(name, passed, input, exp, res, "");
}

int main(void) {
    for (int i = 0; i < 256; i++) {
        h_all_bytes[i] = (uint8_t)i;
        h_hex_lower[2 * i] = "0123456789abcdef"[i >> 4];
        h_hex_lower[2 * i + 1] = "0123456789abcdef"[i & 0xF];
        h_hex_upper[2 * i] = "0123456789ABCDEF"[i >> 4];
        h_hex_upper[2 * i + 1] = "0123456789ABCDEF"[i & 0xF];
    }
    h_hex_lower[512] = '\0';
    h_hex_upper[512] = '\0';

    nlc_begin();

    { static const uint8_t w[] = {0xDE, 0xAD, 0xBE, 0xEF};
      h_check_decode("decode: mixed case", H_LIT("deadBEEF"), 4, 4, w); }
    { static const uint8_t w[] = {0x00, 0xFF, 0x7F, 0x80};
      h_check_decode("decode: boundary byte values", H_LIT("00ff7f80"), 4, 4, w); }
    { static const uint8_t w[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      h_check_decode("decode: exactly fills out_cap", H_LIT("0123456789abcdef"), 8, 8, w); }
    { static const uint8_t w[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      h_check_decode("decode: upper case, room to spare", H_LIT("0123456789ABCDEF"), 16, 8, w); }
    h_check_decode("decode: empty string", H_LIT(""), 4, 0, NULL);
    h_check_decode("decode: empty string, out_cap 0", H_LIT(""), 0, 0, NULL);
    { static const uint8_t w[] = {0xAB};
      h_check_decode("decode: stops at the terminator", H_LIT("ab\0cd"), 1, 1, w); }
    h_check_decode("decode: odd length", H_LIT("abc"), 4, -1, NULL);
    h_check_decode("decode: bad high nibble", H_LIT("g0"), 4, -1, NULL);
    h_check_decode("decode: bad low nibble", H_LIT("0g"), 4, -1, NULL);
    h_check_decode("decode: '/' (just below '0')", H_LIT("12/4"), 4, -1, NULL);
    h_check_decode("decode: ':' (just above '9')", H_LIT("123:"), 4, -1, NULL);
    h_check_decode("decode: '@' (just below 'A')", H_LIT("@123"), 4, -1, NULL);
    h_check_decode("decode: 'G' (just above 'F')", H_LIT("1G23"), 4, -1, NULL);
    h_check_decode("decode: '`' (just below 'a')", H_LIT("12`4"), 4, -1, NULL);
    h_check_decode("decode: 'g' (just above 'f')", H_LIT("123g"), 4, -1, NULL);
    h_check_decode("decode: 0x prefix rejected", H_LIT("0x1f"), 4, -1, NULL);
    h_check_decode("decode: one byte too many for out_cap", H_LIT("0123456789abcdef01"), 8, -1, NULL);
    h_check_decode("decode: one byte into out_cap 0", H_LIT("ff"), 0, -1, NULL);
    h_check_decode("decode: all 256 byte values, lower case", h_hex_lower, 512, 256, 256, h_all_bytes);
    h_check_decode("decode: all 256 byte values, upper case", h_hex_upper, 512, 256, 256, h_all_bytes);

    { static const uint8_t in[] = {0xDE, 0xAD, 0xBE, 0xEF};
      h_check_encode("encode: basic", in, 4, 16, "deadbeef"); }
    { static const uint8_t in[] = {0x00, 0xFF, 0x7F, 0x80};
      h_check_encode("encode: exact fit (out_cap = 2n + 1)", in, 4, 9, "00ff7f80"); }
    { static const uint8_t in[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      h_check_encode("encode: every digit", in, 8, 32, "0123456789abcdef"); }
    { static const uint8_t in[] = {0x00, 0xFF, 0x7F, 0x80};
      h_check_encode("encode: out_cap one too small", in, 4, 8, NULL); }
    { static const uint8_t in[] = {0x5A};
      h_check_encode("encode: n = 0", in, 0, 1, ""); }
    { static const uint8_t in[] = {0x5A};
      h_check_encode("encode: n = 0, out_cap 0", in, 0, 0, NULL); }
    h_check_encode("encode: all 256 byte values", h_all_bytes, 256, 513, h_hex_lower);

    nlc_end();
    return 0;
}

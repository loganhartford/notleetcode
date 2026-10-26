#include <stdint.h>
#include <stddef.h>

static int hex_digit_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int hex_to_bytes(const char *hex, uint8_t *out, size_t out_cap) {
    size_t len = 0;
    while (hex[len] != '\0') len++;
    if (len % 2 != 0 || len / 2 > out_cap) return -1;
    for (size_t i = 0; i < len / 2; i++) {
        int hi = hex_digit_value(hex[2 * i]);
        int lo = hex_digit_value(hex[2 * i + 1]);
        if (hi < 0 || lo < 0) return -1;
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    return (int)(len / 2);
}

int bytes_to_hex(const uint8_t *in, size_t n, char *out, size_t out_cap) {
    static const char digits[] = "0123456789abcdef";
    if (out_cap < 2 * n + 1) return -1;
    for (size_t i = 0; i < n; i++) {
        out[2 * i] = digits[in[i] >> 4];
        out[2 * i + 1] = digits[in[i] & 0x0F];
    }
    out[2 * n] = '\0';
    return (int)(2 * n);
}

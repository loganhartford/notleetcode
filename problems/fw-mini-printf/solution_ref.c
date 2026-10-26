#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

struct sink {
    char *buf;
    size_t cap;
    size_t len;
};

static void put(struct sink *s, char c) {
    if (s->len + 1 < s->cap) s->buf[s->len] = c;
    s->len++;
}

static void put_num(struct sink *s, unsigned int mag, unsigned int base,
                    int neg, int width, char pad) {
    char digits[10];
    int n = 0;
    do {
        digits[n++] = "0123456789abcdef"[mag % base];
        mag /= base;
    } while (mag);

    int fill = width - n - neg;
    if (pad == ' ') while (fill-- > 0) put(s, ' ');
    if (neg) put(s, '-');
    if (pad == '0') while (fill-- > 0) put(s, '0');
    while (n) put(s, digits[--n]);
}

int mini_snprintf(char *buf, size_t cap, const char *fmt, ...) {
    struct sink s = { buf, cap, 0 };
    va_list ap;
    va_start(ap, fmt);

    const char *p = fmt;
    while (*p) {
        char c = *p++;
        if (c != '%') {
            put(&s, c);
            continue;
        }

        char pad = ' ';
        int width = 0;
        if (*p == '0') {
            pad = '0';
            p++;
        }
        if (*p >= '1' && *p <= '9') width = *p++ - '0';
        if (*p == '\0') break;

        switch (*p++) {
        case 'd': {
            int v = va_arg(ap, int);
            unsigned int mag = v < 0 ? 0u - (unsigned int)v : (unsigned int)v;
            put_num(&s, mag, 10, v < 0, width, pad);
            break;
        }
        case 'u':
            put_num(&s, va_arg(ap, unsigned int), 10, 0, width, pad);
            break;
        case 'x':
            put_num(&s, va_arg(ap, unsigned int), 16, 0, width, pad);
            break;
        case 'c':
            put(&s, (char)va_arg(ap, int));
            break;
        case 's': {
            const char *str = va_arg(ap, const char *);
            if (!str) str = "(null)";
            while (*str) put(&s, *str++);
            break;
        }
        case '%':
            put(&s, '%');
            break;
        default:
            break;
        }
    }

    va_end(ap);
    if (cap > 0) buf[s.len < cap ? s.len : cap - 1] = '\0';
    return (int)s.len;
}

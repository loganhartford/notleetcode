#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "nlc.h"
#include "solution.c"

#define NLC_STR_(x) #x
#define NLC_XSTR(x) NLC_STR_(x)

#define TYPE_NAME(x) _Generic((x),                  \
    int: "int",                                     \
    unsigned int: "unsigned int",                   \
    long: "long",                                   \
    unsigned long: "unsigned long",                 \
    long long: "long long",                         \
    unsigned long long: "unsigned long long",       \
    double: "double",                               \
    default: "another type")

static void chk_long(const char *name, const char *input, long got, long exp) {
    char g[32], e[32];
    snprintf(g, sizeof g, "%ld", got);
    snprintf(e, sizeof e, "%ld", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

static void chk_regs(const char *name, const char *input, int extra_ok,
                     const char *extra_got, const char *extra_exp,
                     uint8_t hi, uint8_t lo, uint8_t ehi, uint8_t elo) {
    char g[96], e[96];
    snprintf(g, sizeof g, "hi=0x%02X lo=0x%02X%s", (unsigned)hi, (unsigned)lo, extra_got);
    snprintf(e, sizeof e, "hi=0x%02X lo=0x%02X%s", (unsigned)ehi, (unsigned)elo, extra_exp);
    nlc_case(name, extra_ok && hi == ehi && lo == elo, input, e, g, "");
}

static int min_div(int x, int y)        { return 10 / MIN(x, y); }
static int min_times2(int a, int b)     { return MIN(a, b) * 2; }
static int min_mask_left(unsigned x)    { return (int)MIN(x & 0xFu, 7u); }
static int min_mask_right(unsigned x)   { return (int)MIN(7u, x & 0xFu); }
static int min_nested(int a, int b, int c) { return MIN(MIN(a, b), c); }

static const uint16_t fifo_data[] = { 0xA1B2u, 0xC3D4u, 0xE5F6u };
static int fifo_reads;
uint16_t fifo_pop(void) {
    uint16_t v = fifo_data[fifo_reads % 3];
    fifo_reads++;
    return v;
}

static uint8_t reg_hi, reg_lo;
static char reg_log[16];
uint8_t *reg_access(char which) {
    size_t n = strlen(reg_log);
    if (n + 1 < sizeof reg_log) { reg_log[n] = which; reg_log[n + 1] = '\0'; }
    return which == 'H' ? &reg_hi : &reg_lo;
}

static void write_expr_arg(uint16_t base, uint16_t off) {
    uint8_t hi = 0, lo = 0;
    WRITE_REG16(hi, lo, base | off);
    chk_regs("val is an expression", "WRITE_REG16(hi, lo, 0x1200 | 0x0034)",
             1, "", "", hi, lo, 0x12, 0x34);
}

/* A macro that expands to one statement: it does not start with a bare block,
 * and it has no ';' outside brackets (the caller's ';' must end it). */
static int single_statement_shape(const char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0' || *s == '{') return 0;
    int depth = 0;
    for (; *s; s++) {
        char c = *s;
        if (c == '"' || c == '\'') {
            for (s++; *s && *s != c; s++)
                if (*s == '\\' && s[1]) s++;
            if (!*s) break;
        } else if (c == '(' || c == '{' || c == '[') {
            depth++;
        } else if (c == ')' || c == '}' || c == ']') {
            depth--;
        } else if (c == ';' && depth <= 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    nlc_begin();

    /* SECONDS_PER_YEAR */
    {
        unsigned long v = SECONDS_PER_YEAR;
        char g[32];
        snprintf(g, sizeof g, "%lu", v);
        nlc_case("SECONDS_PER_YEAR value", v == 31536000UL, "SECONDS_PER_YEAR",
                 "31536000", g, "");
    }
    {
        int ok = _Generic((SECONDS_PER_YEAR), unsigned long: 1, default: 0);
        nlc_case("SECONDS_PER_YEAR has type unsigned long", ok,
                 "_Generic(SECONDS_PER_YEAR, ...)", "unsigned long",
                 TYPE_NAME(SECONDS_PER_YEAR), "");
    }
    {
        unsigned long elapsed = 94608000UL;
        unsigned long years = 0;
        if (SECONDS_PER_YEAR != 0) years = elapsed / SECONDS_PER_YEAR;
        char g[32];
        snprintf(g, sizeof g, "%lu", years);
        nlc_case("used as a divisor", years == 3, "94608000UL / SECONDS_PER_YEAR",
                 "3", g, "");
    }

    /* MIN */
    chk_long("MIN(3, 7)", "MIN(3, 7)", MIN(3, 7), 3);
    chk_long("MIN(7, 3)", "MIN(7, 3)", MIN(7, 3), 3);
    chk_long("negative argument", "MIN(4, -9)", MIN(4, -9), -9);
    chk_long("nested", "MIN(MIN(5, 2), 3)", min_nested(5, 2, 3), 2);
    chk_long("result as a divisor", "x=4, y=2: 10 / MIN(x, y)", min_div(4, 2), 5);
    chk_long("result in a product", "a=3, b=8: MIN(a, b) * 2", min_times2(3, 8), 6);
    chk_long("bitwise expression as A", "x=0x13: MIN(x & 0xF, 7)", min_mask_left(0x13u), 3);
    chk_long("bitwise expression as B", "x=0x13: MIN(7, x & 0xF)", min_mask_right(0x13u), 3);

    /* WRITE_REG16 */
    {
        uint8_t hi = 0, lo = 0;
        WRITE_REG16(hi, lo, 0xBEEF);
        chk_regs("splits high and low byte", "WRITE_REG16(hi, lo, 0xBEEF)",
                 1, "", "", hi, lo, 0xBE, 0xEF);
    }
    write_expr_arg(0x1200u, 0x0034u);
    {
        uint8_t hi = 0x55, lo = 0x55;
        if (0)
            WRITE_REG16(hi, lo, 0x1234);
        chk_regs("body of if (0) does nothing", "hi = lo = 0x55; if (0) WRITE_REG16(hi, lo, 0x1234);",
                 1, "", "", hi, lo, 0x55, 0x55);
    }
    {
        uint8_t hi = 0, lo = 0;
        fifo_reads = 0;
        WRITE_REG16(hi, lo, fifo_pop());
        char eg[32], ee[32];
        snprintf(eg, sizeof eg, ", %d pop%s", fifo_reads, fifo_reads == 1 ? "" : "s");
        snprintf(ee, sizeof ee, ", 1 pop");
        chk_regs("val evaluated exactly once", "WRITE_REG16(hi, lo, fifo_pop())  /* pops 0xA1B2 */",
                 fifo_reads == 1, eg, ee, hi, lo, 0xA1, 0xB2);
    }
    {
        reg_hi = reg_lo = 0;
        reg_log[0] = '\0';
        WRITE_REG16(*reg_access('H'), *reg_access('L'), 0x1234);
        char eg[32];
        snprintf(eg, sizeof eg, ", order \"%s\"", reg_log);
        chk_regs("high byte written first, each register once",
                 "WRITE_REG16(*reg_access('H'), *reg_access('L'), 0x1234)",
                 strcmp(reg_log, "HL") == 0, eg, ", order \"HL\"",
                 reg_hi, reg_lo, 0x12, 0x34);
    }
    {
        const char *expansion = NLC_XSTR(WRITE_REG16(hi, lo, val));
        uint8_t hi = 0, lo = 0;
        WRITE_REG16(hi, lo, 0xCAFE);
        int works = hi == 0xCA && lo == 0xFE;
        nlc_case("one statement, safe before else", works && single_statement_shape(expansion),
                 "if (c) WRITE_REG16(hi, lo, val); else ...",
                 "one working statement: not a bare { } block, no ';' of its own", expansion, "");
    }

    nlc_end();
    return 0;
}

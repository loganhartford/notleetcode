#include <stdio.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

/* Exact references, computed independently of the solution. */
static double ref_volts(unsigned code) {
    if (code > 4095u) code = 4095u;
    return (double)code * 10.0 / 4095.0 - 5.0;
}
static int32_t ref_uv(unsigned code) {
    if (code > 4095u) code = 4095u;
    return (int32_t)(((int64_t)code * 10000000 + 4095 / 2) / 4095) - 5000000;
}

#define VOLT_TOL 1e-5
#define UV_TOL   2

static void chk_volts(const char *name, unsigned code) {
    double exp = ref_volts(code);
    double got = (double)adc_to_volts((uint16_t)code);
    double d = got - exp;
    if (d < 0) d = -d;

    char in[48], g[48], e[48];
    snprintf(in, sizeof in, "adc_to_volts(%u)", code);
    snprintf(g, sizeof g, "%.6f", got);
    snprintf(e, sizeof e, "%.6f", exp);
    nlc_case(name, d <= VOLT_TOL, in, e, g, "");
}

static void chk_uv(const char *name, unsigned code) {
    int32_t exp = ref_uv(code);
    int32_t got = adc_to_uv((uint16_t)code);
    long d = (long)got - (long)exp;
    if (d < 0) d = -d;

    char in[48], g[48], e[48];
    snprintf(in, sizeof in, "adc_to_uv(%u)", code);
    snprintf(g, sizeof g, "%d uV", got);
    snprintf(e, sizeof e, "%d uV (+/- %d)", exp, UV_TOL);
    nlc_case(name, d <= UV_TOL, in, e, g, "");
}

int main(void) {
    /* The candidate's own driver. Its output flows to stdout and is shown as
       debug output alongside the results. */
    demo();
    fflush(stdout);

    nlc_begin();

    /* --- endpoints: the two values the spec pins down exactly --- */
    chk_volts("float: code 0 -> -5 V", 0);
    chk_volts("float: code 4095 -> +5 V", 4095);

    /* --- midscale straddles zero by half an LSB --- */
    chk_volts("float: code 2047 just below 0 V", 2047);
    chk_volts("float: code 2048 just above 0 V", 2048);

    /* Sign must actually flip between the two center codes. A /4096 mapping or a
       wrong offset gets one of these backwards. */
    {
        float lo = adc_to_volts(2047);
        float hi = adc_to_volts(2048);
        char g[64];
        snprintf(g, sizeof g, "2047 -> %.6f, 2048 -> %.6f", (double)lo, (double)hi);
        nlc_case("float: zero crossing sits between 2047 and 2048",
                 lo < 0.0f && hi > 0.0f,
                 "adc_to_volts(2047), adc_to_volts(2048)",
                 "negative then positive", g, "");
    }

    /* --- quarter points --- */
    chk_volts("float: code 1024 -> -2.498 V", 1024);
    chk_volts("float: code 3072 -> +2.502 V", 3072);

    /* --- full sweep of every legal code --- */
    {
        unsigned bad = 0, first_bad = 0;
        double worst = 0.0;
        for (unsigned c = 0; c <= 4095u; c++) {
            double d = (double)adc_to_volts((uint16_t)c) - ref_volts(c);
            if (d < 0) d = -d;
            if (d > worst) worst = d;
            if (d > VOLT_TOL) { if (!bad) first_bad = c; bad++; }
        }
        char g[96], e[48];
        if (bad) snprintf(g, sizeof g, "%u/4096 codes off, first at code %u (max err %.2e)",
                          bad, first_bad, worst);
        else snprintf(g, sizeof g, "all 4096 codes within %.2e (max err %.2e)", VOLT_TOL, worst);
        snprintf(e, sizeof e, "all 4096 codes within %.2e", VOLT_TOL);
        nlc_case("float: sweep codes 0..4095", bad == 0,
                 "adc_to_volts(c) for every c in [0, 4095]", e, g, "");
    }

    /* --- clamping above full scale --- */
    chk_volts("float: code 4096 clamps to +5 V", 4096);
    chk_volts("float: code 65535 clamps to +5 V", 65535);

    /* --- fixed point: same battery --- */
    chk_uv("fixed: code 0 -> -5000000 uV", 0);
    chk_uv("fixed: code 4095 -> +5000000 uV", 4095);
    chk_uv("fixed: code 2047 -> -1221 uV", 2047);
    chk_uv("fixed: code 2048 -> +1221 uV", 2048);
    chk_uv("fixed: code 1024", 1024);

    /* Top of the range is where a 32-bit intermediate wraps: code * 10000000
       exceeds INT32_MAX from code 215 upward, so this catches the classic bug. */
    chk_uv("fixed: code 215 (int32 intermediate overflows here)", 215);
    chk_uv("fixed: code 3000 (deep in overflow territory)", 3000);
    chk_uv("fixed: code 4094 (one LSB below full scale)", 4094);

    {
        unsigned bad = 0, first_bad = 0;
        long worst = 0;
        for (unsigned c = 0; c <= 4095u; c++) {
            long d = (long)adc_to_uv((uint16_t)c) - (long)ref_uv(c);
            if (d < 0) d = -d;
            if (d > worst) worst = d;
            if (d > UV_TOL) { if (!bad) first_bad = c; bad++; }
        }
        char g[96], e[48];
        if (bad) snprintf(g, sizeof g, "%u/4096 codes off, first at code %u (max err %ld uV)",
                          bad, first_bad, worst);
        else snprintf(g, sizeof g, "all 4096 codes within %d uV (max err %ld uV)", UV_TOL, worst);
        snprintf(e, sizeof e, "all 4096 codes within %d uV", UV_TOL);
        nlc_case("fixed: sweep codes 0..4095", bad == 0,
                 "adc_to_uv(c) for every c in [0, 4095]", e, g, "");
    }

    /* Strictly increasing: one code up must be one LSB up, never flat or backwards.
       Catches truncation that collapses adjacent codes onto the same output. */
    {
        unsigned bad = 0, first_bad = 0;
        for (unsigned c = 0; c < 4095u; c++) {
            if (adc_to_uv((uint16_t)(c + 1)) <= adc_to_uv((uint16_t)c)) {
                if (!bad) first_bad = c;
                bad++;
            }
        }
        char g[80];
        if (bad) snprintf(g, sizeof g, "%u non-increasing steps, first at code %u", bad, first_bad);
        else snprintf(g, sizeof g, "strictly increasing across all 4095 steps");
        nlc_case("fixed: output is strictly monotonic in code", bad == 0,
                 "adc_to_uv(c+1) > adc_to_uv(c) for all c",
                 "strictly increasing", g, "");
    }

    chk_uv("fixed: code 4096 clamps to +5000000 uV", 4096);
    chk_uv("fixed: code 65535 clamps to +5000000 uV", 65535);

    /* The two implementations must describe the same line. */
    {
        unsigned bad = 0, first_bad = 0;
        double worst = 0.0;
        for (unsigned c = 0; c <= 4095u; c++) {
            double d = (double)adc_to_volts((uint16_t)c) - (double)adc_to_uv((uint16_t)c) / 1e6;
            if (d < 0) d = -d;
            if (d > worst) worst = d;
            if (d > 1e-5) { if (!bad) first_bad = c; bad++; }
        }
        char g[96];
        if (bad) snprintf(g, sizeof g, "%u codes disagree, first at code %u (max %.2e V)",
                          bad, first_bad, worst);
        else snprintf(g, sizeof g, "agree everywhere (max %.2e V)", worst);
        nlc_case("float and fixed agree across the full range", bad == 0,
                 "adc_to_volts(c) vs adc_to_uv(c) / 1e6",
                 "agree within 1.00e-05 V", g, "");
    }

    nlc_end();
    return 0;
}

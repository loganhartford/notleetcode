#include <stdio.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *name, const char *input, float got, float exp) {
    float d = got - exp;
    if (d < 0) d = -d;
    char g[40], e[40];
    snprintf(g, sizeof g, "%.4f", got);
    snprintf(e, sizeof e, "%.4f", exp);
    nlc_case(name, d < 1e-3f, input, e, g, "");
}

int main(void) {
    nlc_begin();

    /* First sample into a zeroed filter: sum=1, always divide by 10. */
    {
        avgBuffer_t f = {0};
        chk("first sample warms up", "newSample=1 (fresh)",
            movingAverage(&f, 1), 0.1f);
    }

    /* Empty/zero input stays zero. */
    {
        avgBuffer_t f = {0};
        chk("first sample 0 -> 0.0", "newSample=0 (fresh)",
            movingAverage(&f, 0), 0.0f);
    }

    /* The documented ramp 0..14, checked at key points. */
    {
        avgBuffer_t f = {0};
        float last = 0.0f;
        float expected[] = {0.0f, 0.1f, 0.3f, 0.6f, 1.0f, 1.5f, 2.1f,
                            2.8f, 3.6f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f};
        int ok = 1;
        for (int i = 0; i < 15; i++) {
            last = movingAverage(&f, (uint8_t)i);
            float d = last - expected[i];
            if (d < 0) d = -d;
            if (d >= 1e-3f) ok = 0;
        }
        char g[40], e[40];
        snprintf(g, sizeof g, "%.4f", last);
        snprintf(e, sizeof e, "%.4f", 9.5f);
        nlc_case("ramp 0..14 matches table", ok,
                 "feed 0,1,2,...,14", e, g, "");
    }

    /* Full window at the 10th sample: sum = 0+1+...+9 = 45, /10 = 4.5. */
    {
        avgBuffer_t f = {0};
        float last = 0.0f;
        for (int i = 0; i < 10; i++) last = movingAverage(&f, (uint8_t)i);
        chk("window fills: mean of 0..9", "feed 0..9", last, 4.5f);
    }

    /* Steady state: a constant input converges to that constant. */
    {
        avgBuffer_t f = {0};
        float last = 0.0f;
        for (int i = 0; i < 40; i++) last = movingAverage(&f, 200);
        chk("constant 200 -> 200.0", "40x newSample=200", last, 200.0f);
    }

    /* Saturated 8-bit samples: sum tops out at 10*255 = 2550. */
    {
        avgBuffer_t f = {0};
        float last = 0.0f;
        for (int i = 0; i < 10; i++) last = movingAverage(&f, 255);
        chk("saturation 255 -> 255.0", "10x newSample=255", last, 255.0f);
    }

    /* Independent filters do not share state. */
    {
        avgBuffer_t a = {0};
        avgBuffer_t b = {0};
        movingAverage(&a, 100);
        movingAverage(&a, 100);
        float rb = movingAverage(&b, 50);
        chk("separate structs isolated", "b sees only newSample=50", rb, 5.0f);
    }

    nlc_end();
    return 0;
}

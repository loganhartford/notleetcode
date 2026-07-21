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

    /* Documented worked example: one message, raw=82, angle=230.625. */
    {
        uint8_t b[] = {56, 15, 22, 17};
        chk("single message (doc example)",
            "{56,15,22,17}", dev_angleSensor_decode(b, 4), 230.625f);
    }

    /* The full 16-pulse buffer: three complete messages, last sync incomplete. */
    {
        uint8_t b[] = {27, 56, 22, 25, 18, 56, 23, 24, 21, 56, 22, 24, 13, 56, 22, 23};
        chk("16-pulse buffer, mean of 3",
            "{27,56,22,...,23}", dev_angleSensor_decode(b, 16), 284.0625f);
    }

    /* Max nibbles: DATA2=27->15, DATA3=27->15 -> raw=(15<<3)|(15>>1)=127. */
    {
        uint8_t b[] = {56, 12, 27, 27};
        chk("max raw = 127", "{56,12,27,27}", dev_angleSensor_decode(b, 4), 357.1875f);
    }

    /* Min nibbles: DATA2=12->0, DATA3=12->0 -> raw=0 -> angle 0. */
    {
        uint8_t b[] = {56, 12, 12, 12};
        chk("min raw = 0", "{56,12,12,12}", dev_angleSensor_decode(b, 4), 0.0f);
    }

    /* DATA3 low bit is dropped: 16->4=0b0100 and 17->5=0b0101 decode the same. */
    {
        uint8_t b1[] = {56, 12, 22, 16};   /* DATA3 nibble 0b0100 */
        uint8_t b2[] = {56, 12, 22, 17};   /* DATA3 nibble 0b0101 */
        float a1 = dev_angleSensor_decode(b1, 4);
        float a2 = dev_angleSensor_decode(b2, 4);
        chk("DATA3 lsb ignored (16 vs 17)", "compare {..,16} and {..,17}", a1, a2);
    }

    /* Two complete messages averaged: raw 127 and raw 0 -> (357.1875+0)/2. */
    {
        uint8_t b[] = {56, 12, 27, 27, 56, 12, 12, 12};
        chk("two messages averaged",
            "{56,12,27,27,56,12,12,12}", dev_angleSensor_decode(b, 8), 178.59375f);
    }

    /* Trailing partial message is ignored (second sync has no DATA3). */
    {
        uint8_t b[] = {56, 15, 22, 17, 56, 15, 22};
        chk("trailing partial ignored",
            "{56,15,22,17,56,15,22}", dev_angleSensor_decode(b, 7), 230.625f);
    }

    /* Leading noise before the sync is skipped. */
    {
        uint8_t b[] = {13, 20, 25, 56, 22, 17, 19};   /* sync at index 3 */
        /* DATA2=b[5]=17->5=0b0101, DATA3=b[6]=19->7=0b0111 -> raw=(5<<3)|(7>>1)=43 */
        chk("skips leading noise",
            "{13,20,25,56,22,17,19}", dev_angleSensor_decode(b, 7), 43.0f * 2.8125f);
    }

    /* No sync pulse at all -> 0.0f, no divide-by-zero. */
    {
        uint8_t b[] = {12, 13, 14, 15, 16, 17};
        chk("no sync -> 0", "{12,13,14,15,16,17}", dev_angleSensor_decode(b, 6), 0.0f);
    }

    /* Sync too close to the end -> incomplete -> 0.0f. */
    {
        uint8_t b[] = {20, 21, 56, 22};   /* sync at index 2, needs index 5 */
        chk("sync too late -> 0", "{20,21,56,22}", dev_angleSensor_decode(b, 4), 0.0f);
    }

    /* Empty buffer. */
    {
        chk("empty buffer -> 0", "{}", dev_angleSensor_decode(NULL, 0), 0.0f);
    }

    nlc_end();
    return 0;
}

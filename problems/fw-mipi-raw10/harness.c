#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

/* Reference encoder: pack 4 pixels into 5 RAW10 bytes */
static void pack_group(uint16_t p0, uint16_t p1, uint16_t p2, uint16_t p3,
                       uint8_t out[5])
{
    out[0] = (uint8_t)(p0 >> 2);
    out[1] = (uint8_t)(p1 >> 2);
    out[2] = (uint8_t)(p2 >> 2);
    out[3] = (uint8_t)(p3 >> 2);
    out[4] = (uint8_t)(((p3 & 3u) << 6) | ((p2 & 3u) << 4) |
                       ((p1 & 3u) << 2) |  (p0 & 3u));
}

static void pack_pixels(const uint16_t *px, int n, uint8_t *raw10) {
    for (int g = 0; g < n / 4; g++)
        pack_group(px[g*4], px[g*4+1], px[g*4+2], px[g*4+3], raw10 + g * 5);
}

static void run_case(const char *name, const uint16_t *expected, int n) {
    uint8_t  *raw10 = (uint8_t  *)malloc((n / 4) * 5);
    uint16_t *got   = (uint16_t *)malloc(n * sizeof(uint16_t));

    pack_pixels(expected, n, raw10);
    memset(got, 0xDE, n * sizeof(uint16_t));
    mipiRaw10ToPlain16(raw10, got, n);

    int ok = 1, first_bad = -1;
    for (int i = 0; i < n; i++) {
        if (got[i] != expected[i]) { ok = 0; first_bad = i; break; }
    }

    char exp_str[64], got_str[64];
    if (ok) {
        snprintf(exp_str, sizeof exp_str, "all %d pixels correct", n);
        snprintf(got_str, sizeof got_str, "all %d pixels correct", n);
    } else {
        snprintf(exp_str, sizeof exp_str, "pixel[%d]=%u", first_bad, expected[first_bad]);
        snprintf(got_str, sizeof got_str, "pixel[%d]=%u", first_bad, got[first_bad]);
    }

    nlc_case(name, ok, name, exp_str, got_str, "");
    free(raw10);
    free(got);
}

int main(void) {
    nlc_begin();

    /* TC-1: boundary values */
    { uint16_t px[] = {1023, 0, 341, 682};
      run_case("TC-1: boundary values (1023, 0, 341, 682)", px, 4); }

    /* TC-2: all zeros */
    { uint16_t px[] = {0,0,0,0, 0,0,0,0};
      run_case("TC-2: all zeros (8 pixels)", px, 8); }

    /* TC-3: all max */
    { uint16_t px[] = {1023,1023,1023,1023, 1023,1023,1023,1023};
      run_case("TC-3: all max 1023 (8 pixels)", px, 8); }

    /* TC-4: all 512 — MSBs only, no LSBs set */
    { uint16_t px[] = {512,512,512,512, 512,512,512,512};
      run_case("TC-4: all 512, no LSBs (8 pixels)", px, 8); }

    /* TC-5: LSB-only patterns */
    { uint16_t px[] = {1, 2, 3, 0};
      run_case("TC-5: LSB-only patterns (1, 2, 3, 0)", px, 4); }

    /* TC-6: alternating 0/1023 across 16 pixels */
    { uint16_t px[16];
      for (int i = 0; i < 16; i++) px[i] = (i % 2) ? 1023 : 0;
      run_case("TC-6: alternating 0/1023 (16 pixels)", px, 16); }

    /* TC-7: ramp 0..1023 (1024 pixels) */
    { uint16_t *px = (uint16_t *)malloc(1024 * sizeof(uint16_t));
      for (int i = 0; i < 1024; i++) px[i] = (uint16_t)i;
      run_case("TC-7: ramp 0-1023 (1024 pixels)", px, 1024);
      free(px); }

    /* TC-8: full 10-bit value sweep repeated (4096 pixels) */
    { uint16_t *px = (uint16_t *)malloc(4096 * sizeof(uint16_t));
      for (int i = 0; i < 4096; i++) px[i] = (uint16_t)(i % 1024);
      run_case("TC-8: full 10-bit sweep (4096 pixels)", px, 4096);
      free(px); }

    /* TC-9: mixed LSB nibble */
    { uint16_t px[] = {0x3FF, 0x3FE, 0x3FD, 0x3FC};
      run_case("TC-9: mixed LSB nibble (0x3FF, 0x3FE, 0x3FD, 0x3FC)", px, 4); }

    /* TC-10: pseudo-random large buffer (40000 pixels) */
    { uint16_t *px = (uint16_t *)malloc(40000 * sizeof(uint16_t));
      uint32_t lfsr = 0xACE1u;
      for (int i = 0; i < 40000; i++) {
          lfsr ^= lfsr >> 7;
          lfsr ^= lfsr << 9;
          lfsr ^= lfsr >> 13;
          px[i] = (uint16_t)(lfsr & 0x3FFu);
      }
      run_case("TC-10: pseudo-random large buffer (40000 pixels)", px, 40000);
      free(px); }

    nlc_end();
    return 0;
}

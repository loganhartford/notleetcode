#include <stdio.h>
#include <stdint.h>

#define ADC_MAX_CODE 4095u

float adc_to_volts(uint16_t code) {
    if (code > ADC_MAX_CODE) code = ADC_MAX_CODE;

    return (float)code * (10.0f / (float)ADC_MAX_CODE) - 5.0f;
}

int32_t adc_to_uv(uint16_t code) {
    if (code > ADC_MAX_CODE) code = ADC_MAX_CODE;

    /* 64-bit intermediate: code * 10000000 peaks at ~4.1e10, well past int32. */
    int64_t scaled = (int64_t)code * 10000000;

    /* Round to nearest instead of truncating; the numerator is never negative. */
    int32_t uv = (int32_t)((scaled + ADC_MAX_CODE / 2) / ADC_MAX_CODE);

    return uv - 5000000;
}

void demo(void) {
    uint16_t codes[] = {0, 1024, 2047, 2048, 3072, 4095};

    for (unsigned i = 0; i < sizeof codes / sizeof codes[0]; i++) {
        uint16_t c = codes[i];
        printf("code %4u -> %9.6f V   %8d uV\n",
               c, (double)adc_to_volts(c), adc_to_uv(c));
    }
}

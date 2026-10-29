#include <stdio.h>
#include <stdint.h>

#define ADC_MAX_CODE 4095u

float adc_to_volts(uint16_t code) {
    return 0.0f;
}

int32_t adc_to_uv(uint16_t code) {
    return 0;
}

/* Your own caller. Runs once before the tests; stdout lands in the Results panel.
   Put whatever codes you want to eyeball in here. */
void demo(void) {
    uint16_t codes[] = {0, 1024, 2047, 2048, 3072, 4095};

    for (unsigned i = 0; i < sizeof codes / sizeof codes[0]; i++) {
        uint16_t c = codes[i];
        printf("code %4u -> %9.6f V   %8d uV\n",
               c, (double)adc_to_volts(c), adc_to_uv(c));
    }
}

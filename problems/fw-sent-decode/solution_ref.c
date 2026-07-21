#include <stdio.h>
#include <stdint.h>

#define NUM_TICKS_IN_SYNC_PULSE 56U
#define DATA_PULSE_OFFSET       12U
#define FAST_CHANNEL_RAW_TO_ANGLE_CONVERSION_FACTOR 2.8125f

float dev_angleSensor_decode(const uint8_t *pulse_buffer, uint8_t length) {
    float sum = 0.0f;
    int   count = 0;

    for (int i = 0; i + 3 < (int)length; i++) {
        if (pulse_buffer[i] == NUM_TICKS_IN_SYNC_PULSE) {
            uint8_t d2 = pulse_buffer[i + 2] - DATA_PULSE_OFFSET;
            uint8_t d3 = pulse_buffer[i + 3] - DATA_PULSE_OFFSET;
            uint8_t raw = (uint8_t)((d2 << 3) | (d3 >> 1));   /* 7-bit value */
            sum += (float)raw * FAST_CHANNEL_RAW_TO_ANGLE_CONVERSION_FACTOR;
            count++;
            i += 3;                     /* skip this message's data pulses */
        }
    }

    return count > 0 ? sum / (float)count : 0.0f;
}

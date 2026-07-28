#include <stdio.h>
#include <stdint.h>

#define CAPACITY 10

typedef struct {
    uint8_t  buf[CAPACITY];   /* the last CAPACITY raw samples          */
    uint8_t  head;            /* next slot to overwrite (oldest sample) */
    uint32_t sum;             /* running sum of the buffer contents     */
} avgBuffer_t;

float movingAverage(avgBuffer_t *filter, uint8_t newSample) {
    return 0.0f;
}

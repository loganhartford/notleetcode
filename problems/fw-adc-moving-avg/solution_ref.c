#include <stdio.h>
#include <stdint.h>

#define CAPACITY 10

typedef struct {
    uint8_t  buf[CAPACITY];
    uint8_t  head;
    uint32_t sum;
} avgBuffer_t;

float movingAverage(avgBuffer_t *filter, uint8_t newSample) {
    filter->sum -= filter->buf[filter->head];
    filter->sum += newSample;

    filter->buf[filter->head] = newSample;
    filter->head = (filter->head + 1) % CAPACITY;

    return (float)filter->sum / (float)CAPACITY;
}

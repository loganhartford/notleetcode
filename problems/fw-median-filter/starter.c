#include <stdint.h>

#define MEDIAN_MAX 15

typedef struct {
    int16_t  ring[MEDIAN_MAX];
    uint8_t  n;
    uint8_t  head;
    uint8_t  count;
} median_t;

void median_init(median_t *m, uint8_t n) {
}

int16_t median_update(median_t *m, int16_t sample) {
    return 0;
}

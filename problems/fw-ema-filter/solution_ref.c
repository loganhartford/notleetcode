#include <stdint.h>

typedef struct {
    int32_t  state;
    uint8_t  k;
} ema_t;

void ema_init(ema_t *f, uint8_t k, int16_t initial) {
    f->k = k;
    f->state = (int32_t)initial * ((int32_t)1 << k);
}

int16_t ema_update(ema_t *f, int16_t x) {
    f->state += x - (f->state >> f->k);
    return (int16_t)(f->state >> f->k);
}

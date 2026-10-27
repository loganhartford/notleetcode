#include <stdint.h>

typedef struct {
    int32_t  state;
    uint8_t  k;
} ema_t;

void ema_init(ema_t *f, uint8_t k, int16_t initial) {
}

int16_t ema_update(ema_t *f, int16_t x) {
    return 0;
}

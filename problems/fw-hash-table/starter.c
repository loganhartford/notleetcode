#include <stdint.h>
#include <stdbool.h>

#define HT_CAP 64

typedef struct {
    uint32_t keys[HT_CAP];
    uint32_t vals[HT_CAP];
    uint8_t  state[HT_CAP];
    uint32_t count;
} ht_t;

void ht_init(ht_t *t) {
}

bool ht_put(ht_t *t, uint32_t key, uint32_t val) {
    return false;
}

bool ht_get(const ht_t *t, uint32_t key, uint32_t *val) {
    return false;
}

bool ht_del(ht_t *t, uint32_t key) {
    return false;
}

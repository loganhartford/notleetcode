#include <stdint.h>
#include <stdbool.h>

#define HT_CAP 64

typedef struct {
    uint32_t keys[HT_CAP];
    uint32_t vals[HT_CAP];
    uint8_t  state[HT_CAP];
    uint32_t count;
} ht_t;

enum { SLOT_EMPTY, SLOT_USED, SLOT_DELETED };

static uint32_t ht_hash(uint32_t key) {
    key ^= key >> 16;
    key *= 0x45d9f3bu;
    key ^= key >> 16;
    return key % HT_CAP;
}

static int ht_find(const ht_t *t, uint32_t key) {
    uint32_t i = ht_hash(key);
    for (int n = 0; n < HT_CAP; n++) {
        if (t->state[i] == SLOT_EMPTY) return -1;
        if (t->state[i] == SLOT_USED && t->keys[i] == key) return (int)i;
        i = (i + 1) % HT_CAP;
    }
    return -1;
}

void ht_init(ht_t *t) {
    for (int i = 0; i < HT_CAP; i++) t->state[i] = SLOT_EMPTY;
    t->count = 0;
}

bool ht_put(ht_t *t, uint32_t key, uint32_t val) {
    int found = ht_find(t, key);
    if (found >= 0) {
        t->vals[found] = val;
        return true;
    }
    if (t->count == HT_CAP) return false;
    uint32_t i = ht_hash(key);
    while (t->state[i] == SLOT_USED) i = (i + 1) % HT_CAP;
    t->keys[i] = key;
    t->vals[i] = val;
    t->state[i] = SLOT_USED;
    t->count++;
    return true;
}

bool ht_get(const ht_t *t, uint32_t key, uint32_t *val) {
    int i = ht_find(t, key);
    if (i < 0) return false;
    *val = t->vals[i];
    return true;
}

bool ht_del(ht_t *t, uint32_t key) {
    int i = ht_find(t, key);
    if (i < 0) return false;
    t->state[i] = SLOT_DELETED;
    t->count--;
    return true;
}

#include <stdint.h>
#include <stdbool.h>

#define BM_IDS 128

typedef struct {
    uint32_t bits[BM_IDS / 32];
} bitmap_t;

#define BM_WORDS (BM_IDS / 32)

static bool bm_in_use(const bitmap_t *b, int id) {
    return (b->bits[id / 32] >> (id % 32)) & 1u;
}

void bm_init(bitmap_t *b) {
    for (int w = 0; w < BM_WORDS; w++) b->bits[w] = 0;
}

int bm_alloc(bitmap_t *b) {
    for (int w = 0; w < BM_WORDS; w++) {
        uint32_t word = b->bits[w];
        if (word != UINT32_MAX) {
            b->bits[w] = word | (word + 1);
            return w * 32 + __builtin_ctz(~word);
        }
    }
    return -1;
}

int bm_alloc_run(bitmap_t *b, unsigned n) {
    if (n == 0 || n > BM_IDS) return -1;
    unsigned len = 0;
    for (int id = 0; id < BM_IDS; id++) {
        len = bm_in_use(b, id) ? 0 : len + 1;
        if (len == n) {
            int start = id + 1 - (int)n;
            for (int i = start; i <= id; i++) b->bits[i / 32] |= 1u << (i % 32);
            return start;
        }
    }
    return -1;
}

bool bm_free(bitmap_t *b, int id) {
    if (id < 0 || id >= BM_IDS || !bm_in_use(b, id)) return false;
    b->bits[id / 32] &= ~(1u << (id % 32));
    return true;
}

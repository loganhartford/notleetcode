#include <stdint.h>
#include <stdbool.h>

#define BM_IDS 128

typedef struct {
    uint32_t bits[BM_IDS / 32];
} bitmap_t;

void bm_init(bitmap_t *b) {
}

int bm_alloc(bitmap_t *b) {
    return -1;
}

int bm_alloc_run(bitmap_t *b, unsigned n) {
    return -1;
}

bool bm_free(bitmap_t *b, int id) {
    return false;
}

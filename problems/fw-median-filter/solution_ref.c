#include <stdint.h>

#define MEDIAN_MAX 15

typedef struct {
    int16_t  ring[MEDIAN_MAX];
    uint8_t  n;
    uint8_t  head;
    uint8_t  count;
} median_t;

static int16_t select_kth(int16_t *a, int len, int k) {
    int lo = 0;
    int hi = len - 1;
    while (lo < hi) {
        int16_t pivot = a[k];
        int i = lo;
        int j = hi;
        do {
            while (a[i] < pivot) i++;
            while (pivot < a[j]) j--;
            if (i <= j) {
                int16_t t = a[i];
                a[i] = a[j];
                a[j] = t;
                i++;
                j--;
            }
        } while (i <= j);
        if (j < k) lo = i;
        if (k < i) hi = j;
    }
    return a[k];
}

void median_init(median_t *m, uint8_t n) {
    m->n = n;
    m->head = 0;
    m->count = 0;
}

int16_t median_update(median_t *m, int16_t sample) {
    m->ring[m->head] = sample;
    m->head = (uint8_t)((m->head + 1) % m->n);
    if (m->count < m->n) m->count++;

    int16_t window[MEDIAN_MAX];
    for (int i = 0; i < m->count; i++) window[i] = m->ring[i];
    return select_kth(window, m->count, (m->count - 1) / 2);
}

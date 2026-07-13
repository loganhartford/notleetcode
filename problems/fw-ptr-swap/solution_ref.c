#include <stdint.h>
#include <stddef.h>

void swap_values(void *a, void *b, size_t size) {
    uint8_t *pa = (uint8_t *)a;
    uint8_t *pb = (uint8_t *)b;
    for (size_t i = 0; i < size; i++) {
        uint8_t tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

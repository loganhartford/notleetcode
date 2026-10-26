#include <stdint.h>
#include <stddef.h>

void swap_values(void *a, void *b, size_t size) {
    uint8_t *value1 = (uint8_t *)a;
    uint8_t *value2 = (uint8_t *)b;
    
    for (int i = 0; i < size; i++) {
        uint8_t temp = value1[i];
        value1[i] = value2[i];
        value2[i] = temp;
    }
    
    return;
}

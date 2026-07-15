#include <stdint.h>
#include <stddef.h>

void swap_values(void *a, void *b, size_t size) {
    
    uint8_t buf;
    for (int i = 0; i < size; i++) {
        buf = *((uint8_t *)a + i);
        *((uint8_t *)a + i) = *((uint8_t *)b + i);
        *((uint8_t *)b + i) = buf;
    }
    
    // Doesn't work because we don't know the type
    // *(int *)a = *(int *)a ^ *(int *)b;
    // *(int *)b = *(int *)a ^ *(int *)b;
    // *(int *)a = *(int *)a ^ *(int *)b;
}

#include <stdint.h>

void link_block_array(void *storage, size_t block_count, size_t block_size) {
    uint8_t * base = (uint8_t *)storage;
    for (int i = 0; i < block_count-1; i++) {
        uint8_t * current = base + i * block_size;
        uint8_t * next = current + block_size;
        *(void **)current = next;
    }
}

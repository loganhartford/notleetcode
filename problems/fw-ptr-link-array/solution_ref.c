#include <stdint.h>

void link_block_array(void *storage, size_t block_count, size_t block_size) {
    for (size_t i = 0; i < block_count; i++) {
        void *block = (uint8_t *)storage + i * block_size;
        void *next  = (i + 1 < block_count)
                      ? (uint8_t *)storage + (i + 1) * block_size
                      : NULL;
        *(void **)block = next;
    }
}

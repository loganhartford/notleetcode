#include <stdint.h>

void link_blocks(void *current, void *next) {
    *(void **)current = next;
}

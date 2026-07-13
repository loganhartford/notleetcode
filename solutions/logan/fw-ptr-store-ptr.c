#include <stdint.h>

void store_pointer(void *storage, void *value) {
    *(void **)storage = value;
}

#include <stdint.h>
#include <string.h>

void *load_pointer(const void *storage) {
    void *result;
    memcpy(&result, storage, sizeof(void *));
    return result;
}

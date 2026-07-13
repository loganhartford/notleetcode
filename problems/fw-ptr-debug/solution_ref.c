#include <stdint.h>
#include <string.h>

void ptr_write_int(void *p, int v) {
    *(int *)p = v;
}

int ptr_read_int(const void *p) {
    return *(const int *)p;
}

void ptr_store_ptr(void *storage, void *val) {
    *(void **)storage = val;
}

void *ptr_load_ptr(const void *storage) {
    void *result;
    memcpy(&result, storage, sizeof(void *));
    return result;
}

void *ptr_next_byte(void *p) {
    return (uint8_t *)p + 1;
}

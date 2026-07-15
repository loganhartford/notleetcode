#include <stdint.h>
#include <string.h>

void ptr_write_int(void *p, int v) {
    *(int *)p = v;
}

int ptr_read_int(const void *p) {
    return *(int *)p;
}

void ptr_store_ptr(void *storage, void *val) {
    *(void **)storage = val;
}

void *ptr_load_ptr(const void *storage) {
    return *(void **)storage;
}

void *ptr_next_byte(void *p) {
    return ((uint8_t *)p + 1);
}

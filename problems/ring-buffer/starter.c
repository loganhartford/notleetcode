#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t* buf;
    size_t   cap;
    size_t   head;
    size_t   count;
} ring_buffer_t;

void rb_init(ring_buffer_t* rb, uint8_t* storage, size_t capacity) {
}

bool rb_push(ring_buffer_t* rb, uint8_t byte) {
    return false;
}

bool rb_pop(ring_buffer_t* rb, uint8_t* out) {
    return false;
}

size_t rb_count(const ring_buffer_t* rb) {
    return 0;
}

bool rb_is_empty(const ring_buffer_t* rb) {
    return true;
}

bool rb_is_full(const ring_buffer_t* rb) {
    return false;
}

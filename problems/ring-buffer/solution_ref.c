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
    rb->buf = storage;
    rb->cap = capacity;
    rb->head = 0;
    rb->count = 0;
}

bool rb_push(ring_buffer_t* rb, uint8_t byte) {
    if (rb->count == rb->cap) return false;
    size_t tail = (rb->head + rb->count) % rb->cap;
    rb->buf[tail] = byte;
    rb->count++;
    return true;
}

bool rb_pop(ring_buffer_t* rb, uint8_t* out) {
    if (rb->count == 0) return false;
    *out = rb->buf[rb->head];
    rb->head = (rb->head + 1) % rb->cap;
    rb->count--;
    return true;
}

size_t rb_count(const ring_buffer_t* rb) {
    return rb->count;
}

bool rb_is_empty(const ring_buffer_t* rb) {
    return rb->count == 0;
}

bool rb_is_full(const ring_buffer_t* rb) {
    return rb->count == rb->cap;
}

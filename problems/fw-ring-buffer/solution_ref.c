// Time: O(1) per operation
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint8_t *buf;
    size_t capacity;
    size_t head;
    size_t tail;
    bool full;
} ringbuf_t;

void rb_init(ringbuf_t *rb, uint8_t *storage, size_t capacity) {
    rb->buf      = storage;
    rb->capacity = capacity;
    rb->head     = 0;
    rb->tail     = 0;
    rb->full     = false;
}

bool rb_push(ringbuf_t *rb, uint8_t byte) {
    if (rb->full) return false;
    rb->buf[rb->tail] = byte;
    rb->tail = (rb->tail + 1) % rb->capacity;
    if (rb->tail == rb->head) rb->full = true;
    return true;
}

bool rb_pop(ringbuf_t *rb, uint8_t *out) {
    if (!rb->full && rb->head == rb->tail) return false;
    *out = rb->buf[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->full = false;
    return true;
}

size_t rb_available(const ringbuf_t *rb) {
    if (rb->full) return rb->capacity;
    return (rb->tail - rb->head + rb->capacity) % rb->capacity;
}

size_t rb_free_space(const ringbuf_t *rb) {
    return rb->capacity - rb_available(rb);
}

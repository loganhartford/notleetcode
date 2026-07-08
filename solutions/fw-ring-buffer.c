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

/* Mistakes made the first time:
    - Didn't read the no-overwrite conditions
    - Didn't read the constraints

*/


void rb_init(ringbuf_t *rb, uint8_t *storage, size_t capacity) {
    rb->buf = storage;
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->full = false;
}

bool rb_push(ringbuf_t *rb, uint8_t byte) {
    if (rb->full) return false;
    
    rb->buf[rb->head] = byte;
    rb->head = (rb->head + 1) % rb->capacity;
    
    rb->full = (rb->head == rb->tail);
    
    return true;
}

bool rb_pop(ringbuf_t *rb, uint8_t *out) {
    bool empty = ((rb->tail == rb->head) && !rb->full);
    if (empty) return false;
    
    
    *out = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->full = false;
    
    return true;
}

size_t rb_available(const ringbuf_t *rb) {
    if (rb->full) {
        return 4;
    } else if (rb->tail == rb->head) {
        return 0;
    }
    
    if (rb->head > rb->tail){
        return rb->head - rb->tail;
    } else {
        return rb->capacity - rb->tail + rb->head;
    }
}

size_t rb_free_space(const ringbuf_t *rb) {
    return rb->capacity - rb_available(rb);
}



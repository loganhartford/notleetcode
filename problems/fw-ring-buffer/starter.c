#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void rb_init(ringbuf_t *rb, uint8_t *storage, size_t capacity) {
    // TODO
}

bool rb_push(ringbuf_t *rb, uint8_t byte) {
    // TODO
    return false;
}

bool rb_pop(ringbuf_t *rb, uint8_t *out) {
    // TODO
    return false;
}

size_t rb_available(const ringbuf_t *rb) {
    // TODO
    return 0;
}

size_t rb_free_space(const ringbuf_t *rb) {
    // TODO
    return 0;
}

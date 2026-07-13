#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct { uint16_t id; uint32_t arg; } event_t;
typedef struct {
    event_t *buf;
    size_t   capacity;
    size_t   head;
    size_t   count;
} event_queue_t;

void eq_init(event_queue_t *q, event_t *storage, size_t capacity) {
    q->buf = storage;
    q->capacity = capacity;
    q->head = 0;
    q->count = 0;
}

bool eq_push(event_queue_t *q, event_t e) {
    // TODO
    return false;
}

bool eq_pop(event_queue_t *q, event_t *out) {
    // TODO
    return false;
}

size_t eq_count(const event_queue_t *q) {
    // TODO
    return 0;
}

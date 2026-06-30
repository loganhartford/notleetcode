// Time: O(1) per operation
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void eq_init(event_queue_t *q, event_t *storage, size_t capacity) {
    q->buf      = storage;
    q->capacity = capacity;
    q->head     = 0;
    q->count    = 0;
}

bool eq_push(event_queue_t *q, event_t e) {
    if (q->count >= q->capacity) return false;
    size_t wi = (q->head + q->count) % q->capacity;
    q->buf[wi] = e;
    q->count++;
    return true;
}

bool eq_pop(event_queue_t *q, event_t *out) {
    if (q->count == 0) return false;
    *out = q->buf[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    return true;
}

size_t eq_count(const event_queue_t *q) {
    return q->count;
}

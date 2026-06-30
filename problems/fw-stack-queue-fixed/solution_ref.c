// Time: O(1) per operation
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool stack_push(fwstack_t *s, int32_t v) {
    if (s->count >= s->cap) return false;
    s->buf[s->count++] = v;
    return true;
}

bool stack_pop(fwstack_t *s, int32_t *out) {
    if (s->count == 0) return false;
    *out = s->buf[--s->count];
    return true;
}

bool q32_push(queue32_t *q, int32_t v) {
    if (q->count >= FW_QUEUE_CAP) return false;
    size_t wi = (q->head + q->count) % FW_QUEUE_CAP;
    q->buf[wi] = v;
    q->count++;
    return true;
}

bool q32_pop(queue32_t *q, int32_t *out) {
    if (q->count == 0) return false;
    *out = q->buf[q->head];
    q->head = (q->head + 1) % FW_QUEUE_CAP;
    q->count--;
    return true;
}

size_t q32_count(const queue32_t *q) {
    return q->count;
}

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct { int32_t *buf; size_t cap; size_t count; } fwstack_t;
#define FW_QUEUE_CAP 16
typedef struct { int32_t buf[FW_QUEUE_CAP]; size_t head; size_t count; } queue32_t;

bool stack_push(fwstack_t *s, int32_t v) {
    // TODO
    return false;
}

bool stack_pop(fwstack_t *s, int32_t *out) {
    // TODO
    return false;
}

bool q32_push(queue32_t *q, int32_t v) {
    // TODO
    return false;
}

bool q32_pop(queue32_t *q, int32_t *out) {
    // TODO
    return false;
}

size_t q32_count(const queue32_t *q) {
    // TODO
    return 0;
}

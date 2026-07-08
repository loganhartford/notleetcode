#include <stdint.h>
#include <stdbool.h>


#define TSQ_CAP 16
typedef struct {
    int32_t inbox[TSQ_CAP];
    int32_t outbox[TSQ_CAP];
    size_t  in_top;
    size_t  out_top;
} two_stack_queue_t;

bool tsq_push(two_stack_queue_t *q, int32_t v) {
    // TODO
    return false;
}

bool tsq_pop(two_stack_queue_t *q, int32_t *out) {
    // TODO
    return false;
}

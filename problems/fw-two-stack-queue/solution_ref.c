// Time: O(1) amortized — each element transferred at most once
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
    if (q->in_top >= TSQ_CAP) return false;
    q->inbox[q->in_top++] = v;
    return true;
}

bool tsq_pop(two_stack_queue_t *q, int32_t *out) {
    if (q->out_top == 0) {
        /* transfer inbox → outbox */
        while (q->in_top > 0) {
            if (q->out_top >= TSQ_CAP) return false; /* shouldn't happen with equal caps */
            q->outbox[q->out_top++] = q->inbox[--q->in_top];
        }
    }
    if (q->out_top == 0) return false;
    *out = q->outbox[--q->out_top];
    return true;
}

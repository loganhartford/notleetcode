#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void case_bool(const char* name, const char* input, bool got, bool exp, const char* out) {
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
}
static void case_int(const char* name, const char* input, int got, int exp, const char* out) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
}

int main(void) {
    /* Run the whole scenario under capture so user printf can't corrupt the
     * results stream; record each return value, then emit cases. */
    nlc_capture_begin();
    MyCircularQueue* q = myCircularQueueCreate(3);
    bool r1 = myCircularQueueEnQueue(q, 1);
    bool r2 = myCircularQueueEnQueue(q, 2);
    bool r3 = myCircularQueueEnQueue(q, 3);
    bool r4 = myCircularQueueEnQueue(q, 4);
    int rear1 = myCircularQueueRear(q);
    bool full = myCircularQueueIsFull(q);
    bool deq = myCircularQueueDeQueue(q);
    bool r5 = myCircularQueueEnQueue(q, 4);
    int rear2 = myCircularQueueRear(q);
    int front = myCircularQueueFront(q);
    bool empty = myCircularQueueIsEmpty(q);
    myCircularQueueFree(q);
    char* out = nlc_capture_end();

    nlc_begin();
    case_bool("enQueue(1)", "create(3); enQueue(1)", r1, true, out);
    case_bool("enQueue(2)", "enQueue(2)", r2, true, "");
    case_bool("enQueue(3)", "enQueue(3)", r3, true, "");
    case_bool("enQueue(4) when full", "enQueue(4)", r4, false, "");
    case_int("Rear()", "Rear()", rear1, 3, "");
    case_bool("isFull()", "isFull()", full, true, "");
    case_bool("deQueue()", "deQueue()", deq, true, "");
    case_bool("enQueue(4) after deQueue", "enQueue(4)", r5, true, "");
    case_int("Rear() after wrap", "Rear()", rear2, 4, "");
    case_int("Front()", "Front()", front, 2, "");
    case_bool("isEmpty()", "isEmpty()", empty, false, "");
    nlc_end();
    free(out);
    return 0;
}

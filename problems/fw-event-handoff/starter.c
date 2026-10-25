#include <stdint.h>

typedef unsigned irq_state_t;
irq_state_t irq_save(void);
void irq_restore(irq_state_t previous);

void post_events_from_isr(volatile uint32_t *pending, uint32_t events) {
}

uint32_t take_pending_events(volatile uint32_t *pending) {
    return 0;
}

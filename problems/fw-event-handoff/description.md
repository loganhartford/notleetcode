# Interrupt-Safe Pending Event Handoff

Optional custom drill. The API and simulator below were created for this app; this is not a reported interview question. The concurrency oral card is the core practice for this topic.

A single-core MCU has one non-nesting producer ISR that sets bits in a shared `volatile uint32_t` event word. The main loop periodically takes all pending events and clears the word. Repeated occurrences of the same event coalesce into one bit.

```c
void post_events_from_isr(volatile uint32_t *pending, uint32_t events);
uint32_t take_pending_events(volatile uint32_t *pending);
```

The platform supplies these hooks; do not implement them:

```c
typedef unsigned irq_state_t;
irq_state_t irq_save(void);
void irq_restore(irq_state_t previous);
```

`irq_save()` disables the producer interrupt and returns its previous mask state. `irq_restore(previous)` restores exactly that state. The hooks provide the compiler and hardware ordering required by this platform. Aligned 32-bit loads and stores are atomic on this target. The ISR cannot preempt while masked. No DMA, other core, or other writer touches the word.

- `post_events_from_isr` adds the given event bits without removing existing bits. It runs only in the producer ISR.
- `take_pending_events` returns a snapshot and clears exactly the events in that snapshot, without losing events arriving at the handoff boundary.
- The caller's interrupt mask must be restored even if interrupts were already disabled.
- Do not block, allocate, busy-wait, or invoke a mutex. Keep the critical section bounded.

### Example 1
```
pending = 0
post_events_from_isr(&pending, 0x01)
post_events_from_isr(&pending, 0x04)
take_pending_events(&pending) -> 0x05; pending becomes 0
take_pending_events(&pending) -> 0
```

### Example 2
```
pending = 0x01
An event 0x08 arrives immediately after interrupts are restored.
take_pending_events(&pending) -> 0x01
pending is now 0x08, to be collected on the next call.
```

### Constraints
- `pending` points to a valid, aligned event word.
- `volatile` alone does not make read-and-clear indivisible.
- This is a single-core interrupt model, not a general thread synchronization API.
- Python models the same protocol: `pending` is a one-element list; `take_pending_events(pending, irq)` receives an object with `save()` and `restore(previous)`. `post_events_from_isr(pending, events)` has no `irq` argument. This models ordering, not Python thread safety.

After coding, explain why `snapshot = *pending; *pending = 0;` can lose an interrupt, why a mutex can deadlock an ISR, and how you would change the design if every occurrence must be counted.

**Optimal complexity:** O(1) time and space per call.

### Concept reference

The [avr-libc atomic-block documentation](https://avrdudes.github.io/avr-libc/avr-libc-user-manual-1.8.1/group__util__atomic.html) demonstrates protecting an ISR-shared variable and restoring the prior interrupt state. This drill applies those concepts to a simulated event word.

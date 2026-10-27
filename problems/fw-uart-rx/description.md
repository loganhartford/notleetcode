# ISR-Safe UART RX Buffer

Implement a UART receive buffer shared between an interrupt service routine and the main loop on a single-core MCU. `uart_rx_isr_byte` runs in the ISR; `uart_rx_read` and `uart_rx_dropped` run in the main loop. The ISR can fire between any two instructions of `uart_rx_read`, so the buffer must stay consistent without a lock.

The `uart_rx_t` struct is provided:

```c
typedef struct {
    volatile uint8_t *buf;
    size_t            capacity;
    volatile size_t   head;     // total bytes ever written  (only the ISR writes this)
    volatile size_t   tail;     // total bytes ever read     (only the main loop writes this)
    volatile size_t   dropped;  // bytes dropped while full  (only the ISR writes this)
} uart_rx_t;
```

Implement:

```c
void   uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity);
void   uart_rx_isr_byte(uart_rx_t *u, uint8_t byte);
size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len);
size_t uart_rx_dropped(const uart_rx_t *u);
```

`head` and `tail` are free-running counters: the number of stored bytes is `head - tail`, and a byte lives at index `counter % capacity`. Every shared variable has exactly one writer, which is what makes this safe without disabling interrupts.

**Drop policy:** when the buffer is full, drop the newest incoming byte and increment `dropped`.

`uart_rx_read` copies up to `max_len` bytes into `out`, removes them from the buffer, and returns the number of bytes copied.

The harness calls the functions sequentially, so it cannot catch a race. Be ready to explain why a layout with a shared `count` (ISR does `count++`, main loop does `count--`) loses updates, and why a mutex cannot be used inside an ISR.

## Examples

**Example 1** — basic receive and read
```
uart_rx_init(&u, buf, 8);
uart_rx_isr_byte(&u, 'H');
uart_rx_isr_byte(&u, 'i');
uart_rx_read(&u, out, 8)  → 2, out="Hi"
```

**Example 2** — overflow and drop tracking
```
uart_rx_init(&u, buf, 3);
uart_rx_isr_byte(&u, 1); uart_rx_isr_byte(&u, 2); uart_rx_isr_byte(&u, 3);
uart_rx_isr_byte(&u, 4);  // dropped
uart_rx_dropped(&u)       → 1
uart_rx_read(&u, out, 3)  → 3, out={1,2,3}
```

**Example 3** — wraparound
```
uart_rx_init(&u, buf, 4);
// Push 4 bytes, read 2, push 2 more — exercises wrap
```

## Constraints

- Only the ISR writes `head` and `dropped`; only the main loop writes `tail`
- On overflow: drop the new byte, increment `dropped`
- `uart_rx_read` from an empty buffer returns 0

**Optimal complexity:** O(n) read, O(1) isr_byte

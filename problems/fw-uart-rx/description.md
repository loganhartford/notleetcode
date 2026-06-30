# ISR-Safe UART RX Buffer

Implement a UART receive buffer designed for use in interrupt-driven systems. The `uart_rx_isr_byte` function is called from an interrupt service routine (ISR), while `uart_rx_read` is called from the main loop.

The `uart_rx_t` struct is provided. Implement:

```c
void   uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity);
void   uart_rx_isr_byte(uart_rx_t *u, uint8_t byte);
size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len);
size_t uart_rx_dropped(const uart_rx_t *u);
```

**Drop policy:** when the buffer is full, drop the newest incoming byte and increment `u->dropped`.

`uart_rx_read` copies up to `max_len` bytes from the buffer into `out`, removes them from the buffer, and returns the number of bytes copied.

The struct layout uses `head` as the read pointer and `count` as the number of bytes stored. Write index = `(head + count) % capacity`.

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

- `head` is the index of the oldest byte; `count` is how many bytes are stored
- Write index: `(head + count) % capacity`
- On overflow: drop the new byte, increment `u->dropped`
- `uart_rx_read` from an empty buffer returns 0

**Optimal complexity:** O(n) read, O(1) isr_byte

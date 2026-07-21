# UART ISR: Stream Words to the Console

Bytes arrive one at a time from a UART peripheral. Each incoming byte fires an
interrupt (`uart_receive_isr`), where the received character is sitting in the global
`UART_RX_REGISTER`. Your job is to print the incoming text with **each word on its own
line**.

```
Input bytes:  H e r e   w e   a r e
Output:
              Here
              we
              are
```

A word boundary is a space character `' '` — print a newline in its place.

### The two halves

You implement three functions that share a ring buffer:

```c
void uart_init(void);          /* called once at startup: reset the buffer to empty */
void uart_receive_isr(void);   /* fires per byte: the char is in UART_RX_REGISTER    */
void main_loop(void);          /* runs from the super-loop: print what has arrived   */
```

- `uart_receive_isr` runs in interrupt context. It must be short: **enqueue the byte and
  return. Do not print from the ISR.**
- `main_loop` is the consumer. Each time it runs it should print out whatever characters
  have arrived so far (spaces rendered as newlines), then return.

### The rules

- **You may not print from the ISR** — only from `main_loop`.
- You have **no control over the interleaving** of `uart_receive_isr` and `main_loop`,
  except that the ISR fires **1 to 3 times** between consecutive `main_loop` runs.
- You do **not** have to wait for a whole word before printing. Print characters as they
  stream in. Words are at most `MAX_WORD_SIZE` (5) characters, but note your buffer does
  **not** need to hold an entire word — a correct streaming solution prints a word longer
  than the buffer just fine.
- The implementation must be able to run **forever**, never falling behind or dropping a
  byte under the 1-to-3 guarantee above.

### Why a ring buffer

The ISR (producer) and `main_loop` (consumer) touch the same buffer but run at different
times. With a single-producer / single-consumer ring buffer where the ISR only advances
`head` and `main_loop` only advances `tail`, the two never write the same variable, so no
locking is required. Size the buffer so that up to 3 pending bytes always fit.

### Example
```
uart_init();
feed "Here we are " one byte at a time,
running main_loop() somewhere between every 1 and 3 bytes.

Console:
Here
we
are
```

### Constraints
- Incoming bytes are printable ASCII plus the space separator.
- Between any two `main_loop` calls, `uart_receive_isr` fires 1–3 times.
- No byte may be lost, duplicated, or reordered.

**Optimal complexity:** O(1) work per byte in the ISR, O(bytes buffered) per `main_loop`.

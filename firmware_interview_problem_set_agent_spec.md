# Firmware Interview Coding Problem Set

Purpose: add a curated firmware-oriented C problem set to an existing LeetCode-style test harness.

Ordering principle: main list is sorted by approximate observed frequency and firmware-interview signal. This is not a generic DS&A list. Problems emphasize C fluency, memory layout, fixed-size buffers, bit manipulation, hardware-facing APIs, state machines, timers, and ISR/thread boundaries.

Target language: C11 unless the harness supports multiple languages. Prefer pure functions when possible. For hardware-style questions, model registers and peripherals using structs, arrays, or fake memory.

For each problem, generate:
- A concise problem statement.
- C function signature(s).
- Constraints.
- At least 8 tests if practical.
- Edge-case tests.
- Reference solution.
- Time/space complexity.
- Firmware notes: UB, alignment, endian, overflow, volatile/atomic/critical-section assumptions where relevant.

Avoid:
- Dynamic allocation unless the problem is explicitly about allocation.
- C++ containers.
- Generic DP/backtracking/tree/graph problems unless explicitly included below.
- Hidden platform assumptions such as `int` width. Use `stdint.h`.

---

# Warm Up

These are the five easiest problems in the set. They should be added first as low-friction C warmups.

## W01 — Set, Clear, Toggle, and Test a Bit

Priority: warmup  
Category: bit manipulation, registers  
Difficulty: easy

Implement helpers for manipulating a single bit in a `uint32_t`.

Suggested signatures:

```c
uint32_t bit_set(uint32_t x, unsigned n);
uint32_t bit_clear(uint32_t x, unsigned n);
uint32_t bit_toggle(uint32_t x, unsigned n);
bool bit_test(uint32_t x, unsigned n);
```

Requirements:
- Valid bit positions are `0..31`.
- The problem may either define out-of-range behavior or require returning the original value for `n >= 32`.
- Tests should include bit 0, bit 31, already-set bit, already-clear bit, and out-of-range if defined.

Firmware relevance:
- Basic register manipulation.
- Avoid signed shifts and `1 << 31` on signed `int`; use `UINT32_C(1)`.

---

## W02 — Check Power of Two

Priority: warmup  
Category: bit manipulation  
Difficulty: easy

Implement `is_power_of_two_u32`.

Suggested signature:

```c
bool is_power_of_two_u32(uint32_t x);
```

Requirements:
- Return false for zero.
- Use the `x && !(x & (x - 1))` property.
- Tests: `0`, `1`, `2`, `3`, `4`, `31`, `32`, `0x80000000`.

Firmware relevance:
- Common bit-manipulation screen.
- Useful for buffer sizes, alignment, and masks.

---

## W03 — Reverse a String In Place

Priority: warmup  
Category: pointers, strings  
Difficulty: easy

Reverse a null-terminated string in place.

Suggested signature:

```c
void reverse_cstr(char *s);
```

Requirements:
- Handle empty string and one-character string.
- Do not allocate.
- Tests: even length, odd length, empty, single char, punctuation.

Firmware relevance:
- Tests pointer indexing and in-place mutation.

---

## W04 — Byte Swap 16/32

Priority: warmup  
Category: endian, bit manipulation  
Difficulty: easy

Implement byte-order reversal for 16-bit and 32-bit values.

Suggested signatures:

```c
uint16_t bswap16_u(uint16_t x);
uint32_t bswap32_u(uint32_t x);
```

Requirements:
- Use shifts and masks.
- Tests: `0x0000`, `0xFFFF`, `0x1234`, `0xAABBCCDD`, `0x01020304`.

Firmware relevance:
- Endian conversion for protocols, flash formats, and wire formats.

---

## W05 — Implement `strlen`

Priority: warmup  
Category: pointers, libc primitives  
Difficulty: easy

Implement `strlen` for a null-terminated string.

Suggested signature:

```c
size_t c_strlen(const char *s);
```

Requirements:
- Input is guaranteed non-null unless the harness wants to define null behavior.
- Tests: empty, one char, normal string, embedded null.

Firmware relevance:
- Basic pointer walking.
- Good warmup for libc reimplementation questions.

---

# Main Ordered Problem List

## P01 — Register Bitfield Helpers

Priority: 1  
Category: bit manipulation, register access  
Difficulty: easy/medium

Implement generic helpers to read and write bitfields inside a `uint32_t` register value.

Suggested signatures:

```c
uint32_t reg_set_bits(uint32_t reg, uint32_t mask);
uint32_t reg_clear_bits(uint32_t reg, uint32_t mask);
uint32_t reg_write_field(uint32_t reg, uint32_t mask, unsigned shift, uint32_t value);
uint32_t reg_read_field(uint32_t reg, uint32_t mask, unsigned shift);
```

Requirements:
- `reg_write_field` clears the masked field, inserts `value << shift`, and preserves other bits.
- `value` should be masked so it cannot leak outside the field.
- Tests should cover fields at LSB, middle, MSB, zero-width mask if defined, and preserving unrelated bits.

Firmware relevance:
- Directly maps to MMIO register manipulation.
- Tests masking discipline and shift UB awareness.

---

## P02 — Fixed-Size Ring Buffer

Priority: 2  
Category: embedded data structures, buffers  
Difficulty: medium

Implement a fixed-size byte ring buffer.

Suggested API:

```c
typedef struct {
    uint8_t *buf;
    size_t capacity;
    size_t head;
    size_t tail;
    bool full;
} ringbuf_t;

void rb_init(ringbuf_t *rb, uint8_t *storage, size_t capacity);
bool rb_push(ringbuf_t *rb, uint8_t byte);
bool rb_pop(ringbuf_t *rb, uint8_t *out);
size_t rb_available(const ringbuf_t *rb);
size_t rb_free_space(const ringbuf_t *rb);
```

Requirements:
- No malloc.
- Define full/empty disambiguation. Either use a `full` flag or reserve one slot.
- Tests: empty pop, fill to capacity, overflow behavior, wraparound, interleaved push/pop, capacity 1 if supported.

Firmware relevance:
- Classic UART/SPI/logging buffer problem.
- Tests fixed memory, wraparound, and API clarity.

---

## P03 — `memcpy`, `memmove`, and `memset`

Priority: 3  
Category: pointers, libc primitives, memory  
Difficulty: medium

Implement basic C memory primitives.

Suggested signatures:

```c
void *c_memcpy(void *dst, const void *src, size_t n);
void *c_memmove(void *dst, const void *src, size_t n);
void *c_memset(void *dst, int value, size_t n);
```

Requirements:
- `memcpy`: behavior undefined for overlap, but tests should cover normal operation.
- `memmove`: must correctly handle overlapping ranges by choosing copy direction.
- `memset`: writes low 8 bits of `value`.
- Return `dst`.
- Tests: zero length, one byte, normal copy, overlapping forward, overlapping backward, same source/destination.

Firmware relevance:
- Common interview prompt.
- Tests pointer arithmetic, overlap reasoning, and byte semantics.

---

## P04 — C Bug Snippet Diagnosis Pack

Priority: 4  
Category: code reading, C semantics  
Difficulty: medium

Create a set of small code snippets where the candidate must identify and fix the bug.

Recommended subproblems:
1. Missing `volatile` on ISR-updated flag.
2. Incorrect toggle expression: `x ^ ~(1u << i)` instead of `x ^ (1u << i)`.
3. Operator precedence bug: `n & (n - 1) == 0`.
4. Buffer overflow via unbounded copy.
5. Signed shift or signed overflow UB.
6. `sizeof(ptr)` used instead of array length.
7. Returning pointer to stack memory.
8. Missing bounds check before MMIO-style array/register access.

Harness format options:
- Multiple-choice expected diagnosis string.
- `fix_bug_X()` functions with tests that fail before the fix.
- Static snippets in markdown with expected answer.

Firmware relevance:
- Very common embedded interview format.
- Tests C correctness under low-level constraints.

---

## P05 — Endian-Safe Serialization and Deserialization

Priority: 5  
Category: endian, wire format, structs  
Difficulty: medium

Serialize and deserialize a fixed wire-format message without raw-copying a C struct.

Example message:

```c
typedef struct {
    uint32_t device_id;
    int16_t temperature_q8_8;
    uint16_t millivolts;
    uint8_t flags;
} sensor_msg_t;
```

Suggested signatures:

```c
size_t sensor_msg_encode(uint8_t *out, size_t out_len, const sensor_msg_t *msg);
bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len);
```

Requirements:
- Define wire format as little-endian or big-endian explicitly.
- Do not use `memcpy` of the whole struct.
- Return encoded length or zero on failure.
- Decode should reject insufficient input length.
- Tests should verify exact byte output and round trip.
- Include negative signed fixed-point temperature test.

Firmware relevance:
- Tests endian, padding, alignment, fixed field widths.
- Directly representative of protocol/flash/NVM work.

---

## P06 — Packet Parser State Machine

Priority: 6  
Category: state machines, protocol parsing  
Difficulty: medium/hard

Implement a byte-stream packet parser.

Example frame:

```text
0xAA LEN PAYLOAD... CHECKSUM
```

Where:
- `0xAA` is sync.
- `LEN` is payload length, `0..MAX_PAYLOAD`.
- `CHECKSUM` is low 8 bits of the sum of `LEN` and all payload bytes.

Suggested API:

```c
typedef enum {
    PARSER_NONE = 0,
    PARSER_PACKET,
    PARSER_ERROR
} parser_result_t;

typedef struct {
    /* implementation-defined state */
} parser_t;

void parser_init(parser_t *p);
parser_result_t parser_feed(parser_t *p, uint8_t byte, uint8_t *payload_out, size_t *len_out);
```

Requirements:
- Feed one byte at a time.
- Recover after noise before sync.
- Reject oversized length.
- Reset after checksum failure.
- Tests: valid empty payload, valid multi-byte payload, noise before packet, bad checksum, truncated packet followed by new sync, oversized length.

Firmware relevance:
- Extremely representative of UART/BLE/SPI/protocol parser work.
- Tests state machines and recovery behavior.

---

## P07 — ISR-Safe UART RX Buffer Model

Priority: 7  
Category: ISR boundary, ring buffers, concurrency  
Difficulty: hard

Model UART receive handling where an ISR pushes bytes into a ring buffer and foreground code reads them.

Suggested API:

```c
typedef struct {
    /* ring buffer and synchronization fields */
} uart_rx_t;

void uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity);
void uart_rx_isr_byte(uart_rx_t *u, uint8_t byte);
size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len);
size_t uart_rx_dropped(const uart_rx_t *u);
```

Requirements:
- No malloc.
- Define overflow policy: drop newest or drop oldest. Prefer drop newest and count drops.
- Single producer: ISR. Single consumer: foreground.
- Tests should simulate ISR calls and foreground reads.
- Include wraparound and overflow tests.

Firmware notes:
- In the problem statement, ask candidate to explain assumptions:
  - single-core bare metal vs RTOS threads,
  - whether index load/store is naturally atomic,
  - where critical sections may be needed,
  - why `volatile` is not a general synchronization primitive.

Firmware relevance:
- More realistic than plain ring buffer.
- Tests interrupt/foreground ownership boundaries.

---

## P08 — Static Memory Pool Allocator

Priority: 8  
Category: memory management, embedded allocation  
Difficulty: hard

Implement a fixed-size block allocator over caller-provided storage.

Suggested API:

```c
typedef struct {
    /* free list metadata */
} pool_t;

bool pool_init(pool_t *pool, void *storage, size_t storage_size, size_t block_size);
void *pool_alloc(pool_t *pool);
bool pool_free(pool_t *pool, void *ptr);
size_t pool_free_count(const pool_t *pool);
```

Requirements:
- No calls to `malloc`.
- Blocks must be aligned to at least `alignof(max_align_t)` or a specified alignment.
- Reject invalid frees.
- Reject double-free if practical.
- Tests: allocate all blocks, exhaustion, free/reuse, invalid pointer, unaligned storage, block size smaller than pointer size.

Firmware relevance:
- Common embedded substitute for heap.
- Tests alignment, ownership, and free-list mechanics.

---

## P09 — Producer/Consumer Bounded Queue

Priority: 9  
Category: queues, concurrency model  
Difficulty: medium/hard

Implement a fixed-size queue for events shared between a producer and consumer.

Example event:

```c
typedef struct {
    uint16_t id;
    uint32_t arg;
} event_t;
```

Suggested API:

```c
typedef struct {
    /* fixed queue */
} event_queue_t;

void eq_init(event_queue_t *q, event_t *storage, size_t capacity);
bool eq_push(event_queue_t *q, event_t e);
bool eq_pop(event_queue_t *q, event_t *out);
size_t eq_count(const event_queue_t *q);
```

Requirements:
- No malloc.
- Tests: empty, full, wraparound, interleaved operations.
- Optional extension: add ISR-safe section or mock lock hooks.

Firmware relevance:
- Represents ISR-to-task event passing and RTOS queue concepts.
- Also covers array-backed queue mechanics.

---

## P10 — Wraparound-Safe Software Timer Scheduler

Priority: 10  
Category: timers, callbacks, unsigned arithmetic  
Difficulty: hard

Implement a small software timer system driven by a monotonic `uint32_t` millisecond tick.

Suggested API:

```c
typedef void (*timer_cb_t)(void *ctx);

typedef struct {
    /* timer table */
} timer_sched_t;

void ts_init(timer_sched_t *ts);
int ts_start(timer_sched_t *ts, uint32_t now, uint32_t delay_ms, bool periodic, timer_cb_t cb, void *ctx);
bool ts_cancel(timer_sched_t *ts, int timer_id);
void ts_poll(timer_sched_t *ts, uint32_t now);
```

Requirements:
- Fixed maximum number of timers.
- No malloc.
- Support one-shot and periodic timers.
- Must handle `uint32_t` tick wraparound correctly.
- Tests: single timer, multiple timers, cancel, periodic, simultaneous expiry, wraparound near `UINT32_MAX`.

Firmware relevance:
- Common embedded scheduling primitive.
- Tests unsigned wraparound reasoning and callback design.

---

## P11 — Popcount, Bit Reverse, and Rotate Suite

Priority: 11  
Category: bit manipulation  
Difficulty: medium

Create separate problems or one multi-function problem for common bit operations.

Suggested signatures:

```c
unsigned popcount32(uint32_t x);
uint32_t reverse_bits32(uint32_t x);
uint32_t rotl32(uint32_t x, unsigned n);
uint32_t rotr32(uint32_t x, unsigned n);
```

Requirements:
- `popcount32`: use either loop or Kernighan method.
- `reverse_bits32`: loop solution acceptable; optimized mask-swap optional.
- Rotate must handle `n == 0` and `n >= 32` without UB.
- Tests: zero, all ones, alternating bits, MSB/LSB, rotate by 0/1/31/32/33.

Firmware relevance:
- Very common low-level C interview material.
- Tests shift semantics and unsigned operations.

---

## P12 — Fake SPI Register Driver

Priority: 12  
Category: driver design, MMIO model  
Difficulty: hard

Given a fake SPI peripheral register block, implement init, transmit, receive, and status handling.

Example fake hardware:

```c
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t STATUS;
    volatile uint32_t TXD;
    volatile uint32_t RXD;
} spi_regs_t;
```

Define bits:
- `CTRL_ENABLE`
- `STATUS_TX_READY`
- `STATUS_RX_READY`
- `STATUS_ERROR`

Suggested API:

```c
typedef enum {
    SPI_OK = 0,
    SPI_TIMEOUT,
    SPI_ERROR
} spi_status_t;

void spi_init(spi_regs_t *regs, uint32_t ctrl_flags);
spi_status_t spi_transfer_byte(spi_regs_t *regs, uint8_t tx, uint8_t *rx, uint32_t timeout_iters);
```

Requirements:
- Poll ready bits with timeout.
- Return error if error bit set.
- Read/write only through register fields.
- Tests should use fake register state transitions.

Firmware relevance:
- Represents hardware-facing coding interview questions.
- Tests volatile MMIO semantics, timeout loops, and status handling.

---

## P13 — Aligned Allocation Over a Static Buffer

Priority: 13  
Category: memory, alignment  
Difficulty: hard

Implement a simple bump allocator that returns aligned addresses from a fixed buffer.

Suggested API:

```c
typedef struct {
    uint8_t *base;
    size_t size;
    size_t offset;
} bump_alloc_t;

void bump_init(bump_alloc_t *a, void *storage, size_t size);
void *bump_alloc_aligned(bump_alloc_t *a, size_t size, size_t alignment);
void bump_reset(bump_alloc_t *a);
```

Requirements:
- `alignment` must be a power of two.
- Return null if insufficient space.
- No free except reset.
- Tests: alignments 1/2/4/8/16/32, unaligned base address, exhaustion, zero-size if defined.

Firmware relevance:
- Aligned allocation appears in embedded/firmware interviews.
- Tests pointer/integer conversions and alignment math.

---

## P14 — `atoi` With Overflow Handling

Priority: 14  
Category: strings, parsing  
Difficulty: medium

Implement ASCII decimal string to `int32_t`.

Suggested signature:

```c
bool parse_i32(const char *s, int32_t *out);
```

Requirements:
- Handle optional leading `+` or `-`.
- Reject empty strings and non-digit characters.
- Detect overflow and underflow.
- Do not use libc conversion functions.
- Tests: `0`, positive, negative, `INT32_MAX`, `INT32_MIN`, overflow, underflow, invalid input.

Firmware relevance:
- Common command parser primitive.
- Tests parsing and overflow reasoning.

---

## P15 — Integer to ASCII

Priority: 15  
Category: strings, formatting  
Difficulty: medium

Implement signed integer to decimal ASCII.

Suggested signature:

```c
bool i32_to_dec(char *out, size_t out_len, int32_t value);
```

Requirements:
- Null-terminate output.
- Return false if buffer is too small.
- Correctly handle `INT32_MIN`.
- Tests: zero, positive, negative, max, min, too-small buffer.

Firmware relevance:
- Useful when `printf` is unavailable or too heavy.
- Tests buffer sizing and signed edge cases.

---

## P16 — CRC-8 or CRC-16

Priority: 16  
Category: checksums, protocols  
Difficulty: medium

Implement a tableless CRC.

Suggested signature for CRC-8:

```c
uint8_t crc8_ccitt(const uint8_t *data, size_t len);
```

Alternative CRC-16:

```c
uint16_t crc16_ccitt_false(const uint8_t *data, size_t len);
```

Requirements:
- Specify polynomial, init value, reflection behavior, and final xor.
- Include known check vector: ASCII `"123456789"`.
- Tests: empty buffer, one byte, multi-byte, known vector.

Firmware relevance:
- Common protocol and NVM integrity primitive.
- Tests bitwise loops and deterministic byte processing.

---

## P17 — GPIO Debouncer State Machine

Priority: 17  
Category: state machine, timers, GPIO  
Difficulty: medium

Implement a debouncer that consumes sampled GPIO levels and emits stable edge events.

Suggested API:

```c
typedef enum {
    DB_NONE = 0,
    DB_RISING,
    DB_FALLING
} db_event_t;

typedef struct {
    /* state */
} debounce_t;

void db_init(debounce_t *d, bool initial_level, uint32_t debounce_ms);
db_event_t db_update(debounce_t *d, bool raw_level, uint32_t now_ms);
bool db_level(const debounce_t *d);
```

Requirements:
- Input is sampled irregularly with timestamps.
- State changes only after raw input remains changed for `debounce_ms`.
- Handle timestamp wraparound if using `uint32_t`.
- Tests: clean edge, bouncing edge, no change, rapid bounce, wraparound.

Firmware relevance:
- Common embedded state-machine/timer pattern.
- Good preparation for hardware input handling.

---

## P18 — Command Line Parser

Priority: 18  
Category: strings, parsing, embedded shell  
Difficulty: medium

Parse a simple command line into tokens.

Suggested signature:

```c
size_t parse_tokens(char *line, char **argv, size_t max_args);
```

Requirements:
- Tokenize in place by replacing separators with `\0`.
- Whitespace separates tokens.
- Ignore leading/trailing/repeated spaces.
- Optional stretch: quoted strings.
- Tests: empty line, one arg, multiple spaces, max args truncation, tabs/newlines.

Firmware relevance:
- Common embedded CLI/shell primitive.
- Tests pointer mutation and buffer ownership.

---

## P19 — Fixed-Point Q-Format Multiply

Priority: 19  
Category: fixed-point math, overflow  
Difficulty: medium/hard

Implement multiplication for signed Q16.16 values.

Suggested signatures:

```c
typedef int32_t q16_16_t;

q16_16_t q16_mul(q16_16_t a, q16_16_t b);
q16_16_t q16_from_int(int32_t x);
int32_t q16_to_int_trunc(q16_16_t x);
```

Requirements:
- Use `int64_t` intermediate.
- Define rounding behavior: truncation or nearest.
- Tests: 1.0 * 1.0, 1.5 * 2.0, negative values, fractional values, near-overflow behavior.

Firmware relevance:
- Common in control, estimation, and sensor processing.
- Tests integer math and overflow.

---

## P20 — Piecewise Linear Interpolation Table

Priority: 20  
Category: arrays, fixed-point, calibration  
Difficulty: medium

Given sorted calibration points, interpolate `y` for input `x`.

Suggested types:

```c
typedef struct {
    int32_t x;
    int32_t y;
} point_t;

bool interp_i32(const point_t *pts, size_t n, int32_t x, int32_t *y_out);
```

Requirements:
- Points sorted by `x`.
- Reject duplicate `x` points or define behavior.
- Clamp or reject out-of-range input; choose explicitly.
- Use `int64_t` for intermediate slope math.
- Tests: exact point, between points, below range, above range, negative slope, large values.

Firmware relevance:
- Calibration tables and sensor conversion.
- Tesla-style interpolation category.

---

## P21 — Intrusive Singly Linked List

Priority: 21  
Category: embedded data structures  
Difficulty: medium

Implement an intrusive singly linked list using caller-owned nodes.

Suggested type:

```c
typedef struct node {
    struct node *next;
    int value;
} node_t;
```

Suggested functions:

```c
void list_push_front(node_t **head, node_t *node);
node_t *list_pop_front(node_t **head);
bool list_remove(node_t **head, node_t *node);
size_t list_count(const node_t *head);
```

Requirements:
- No allocation.
- Nodes are owned by caller.
- Tests: empty list, one node, remove head/middle/tail, remove absent node.

Firmware relevance:
- Intrusive lists are more embedded-real than malloc-backed lists.
- Tests pointer manipulation.

---

## P22 — Reverse a Singly Linked List

Priority: 22  
Category: linked lists, DS&A subset  
Difficulty: medium

Reverse a singly linked list in place.

Suggested signature:

```c
node_t *list_reverse(node_t *head);
```

Requirements:
- No allocation.
- Tests: empty, one node, two nodes, many nodes.

Firmware relevance:
- This is one of the few generic DS&A linked-list questions with real evidence in embedded interviews.

---

## P23 — Detect Cycle in Singly Linked List

Priority: 23  
Category: linked lists, DS&A subset  
Difficulty: medium

Use Floyd's tortoise/hare algorithm.

Suggested signature:

```c
bool list_has_cycle(const node_t *head);
```

Requirements:
- No modification to the list.
- Tests: empty, no cycle, cycle at head, cycle in middle, self-cycle.

Firmware relevance:
- Common C pointer interview question.
- Useful DS&A subset without heavy abstractions.

---

## P24 — Merge Two Sorted Linked Lists

Priority: 24  
Category: linked lists, DS&A subset  
Difficulty: medium

Merge two sorted singly linked lists by relinking existing nodes.

Suggested signature:

```c
node_t *list_merge_sorted(node_t *a, node_t *b);
```

Requirements:
- No allocation.
- Stable merge preferred.
- Tests: empty lists, one empty, interleaved, duplicates, negative values.

Firmware relevance:
- Evidence-backed DS&A subset.
- Tests pointer relinking.

---

## P25 — Fixed-Size Stack and Queue

Priority: 25  
Category: arrays, embedded data structures  
Difficulty: easy/medium

Implement array-backed stack and queue for `int32_t`.

Suggested APIs:

```c
typedef struct {
    int32_t *buf;
    size_t capacity;
    size_t count;
} stack_t;

bool stack_push(stack_t *s, int32_t v);
bool stack_pop(stack_t *s, int32_t *out);
```

Queue can reuse ring-buffer mechanics.

Requirements:
- No malloc.
- Return false on overflow/underflow.
- Tests: empty pop, full push, order preservation.

Firmware relevance:
- Basic fixed-memory container practice.
- Appears in embedded prep sources more often than complex DS&A.

---

## P26 — Array Reverse and Rotate

Priority: 26  
Category: arrays, two pointers  
Difficulty: easy/medium

Implement in-place reverse and left-rotate by `k`.

Suggested signatures:

```c
void reverse_i32(int32_t *a, size_t n);
void rotate_left_i32(int32_t *a, size_t n, size_t k);
```

Requirements:
- No allocation.
- `k` may be greater than `n`.
- Tests: empty, one element, even/odd, rotate by 0/1/n/n+1.

Firmware relevance:
- Lower-priority but useful pointer/indexing practice.

---

## P27 — Find Duplicate in Range 0..n-1

Priority: 27  
Category: arrays, constraints  
Difficulty: medium

Given an array of length `n` containing values in `0..n-1`, find any duplicate.

Suggested signature:

```c
bool find_duplicate_u32(uint32_t *a, size_t n, uint32_t *dup_out);
```

Requirements:
- Can modify input if the chosen algorithm uses index marking/swapping.
- Return false if no duplicate, unless problem guarantees duplicate.
- Tests: duplicate at beginning, end, multiple duplicates, no duplicate, invalid value if defined.

Firmware relevance:
- Evidence-backed array subset.
- Tests constraints and in-place techniques.

---

## P28 — Binary Search Calibration Table

Priority: 28  
Category: arrays, search, calibration  
Difficulty: easy/medium

Find the index of the greatest calibration point with `x <= target`.

Suggested signature:

```c
bool lower_bracket_index(const int32_t *xs, size_t n, int32_t target, size_t *idx_out);
```

Requirements:
- Sorted ascending input.
- Return false if target is below first point or if n is zero.
- Tests: exact match, between points, below range, above range, duplicates if defined.

Firmware relevance:
- More firmware-real than generic binary-search problem.
- Useful before interpolation.

---

## P29 — Bit Pattern Search at Arbitrary Bit Offset

Priority: 29  
Category: bit manipulation, buffers  
Difficulty: hard

Given a byte buffer, find every bit offset where a `k`-bit pattern occurs. Start with fixed 32-bit pattern if simpler.

Suggested signature:

```c
size_t find_bit_pattern32(const uint8_t *buf,
                          size_t bit_len,
                          uint32_t pattern,
                          unsigned pattern_bits,
                          size_t *out_offsets,
                          size_t max_offsets);
```

Requirements:
- Search at every bit offset, not just byte-aligned offsets.
- Define bit order within bytes: MSB-first or LSB-first.
- Tests: byte-aligned match, unaligned match, overlapping matches, no match, pattern at end boundary.

Firmware relevance:
- Reported as a real firmware interview problem.
- Tests bit indexing and careful bounds handling.

---

## P30 — Store and Read MMIO Word Safely

Priority: 30  
Category: MMIO, volatile, alignment  
Difficulty: medium

Model writing a 32-bit value to a memory-mapped register.

Suggested signature:

```c
bool mmio_write32(uintptr_t addr, uint32_t value);
bool mmio_read32(uintptr_t addr, uint32_t *out);
```

Requirements:
- In a real system this would dereference `volatile uint32_t *`.
- In the harness, avoid actual arbitrary address dereference. Use a fake memory region and validate that `addr` maps into it.
- Reject unaligned addresses.
- Tests: aligned valid write/read, unaligned address, out-of-range address.

Firmware relevance:
- Tests `volatile`, alignment, and pointer/integer address reasoning without unsafe harness behavior.

---

## P31 — Detect Stack Growth Direction

Priority: 31  
Category: C runtime, stack, addresses  
Difficulty: stretch

Implement a function that determines whether stack addresses grow upward or downward.

Suggested signature:

```c
typedef enum {
    STACK_UNKNOWN = 0,
    STACK_GROWS_UP,
    STACK_GROWS_DOWN
} stack_dir_t;

stack_dir_t detect_stack_direction(void);
```

Requirements:
- Use addresses of locals in nested calls.
- Harness may be platform-dependent. Mark as optional or explanation-based if portability is an issue.

Firmware relevance:
- Reported in embedded interviews.
- Tests mental model of stack and call frames.

---

## P32 — Two-Pointer Subarray Sum

Priority: 32  
Category: arrays, narrow DS&A subset  
Difficulty: medium

Given non-negative integers, find a contiguous subarray whose sum equals target.

Suggested signature:

```c
bool subarray_sum_nonneg(const uint32_t *a, size_t n, uint32_t target, size_t *start, size_t *end_exclusive);
```

Requirements:
- Non-negative input enables sliding-window solution.
- Tests: found at beginning, middle, end, not found, target zero, empty input.

Firmware relevance:
- Lower-priority. Include because two-pointer/sliding-window array problems show up in some embedded interview reports.
- Avoid presenting as core firmware.

---

## P33 — Longest Substring Without Repeating Characters

Priority: 33  
Category: strings, company-specific DS&A  
Difficulty: medium

Implement the standard sliding-window solution over ASCII bytes.

Suggested signature:

```c
size_t longest_unique_substr_ascii(const char *s);
```

Requirements:
- Input is null-terminated ASCII.
- Use fixed array of 256 last-seen indices, not a heap map.
- Tests: empty, all unique, all same, repeated patterns.

Firmware relevance:
- Low firmware signal. Include only for Apple/big-tech embedded prep.

---

## P34 — Queue From Two Stacks

Priority: 34  
Category: generic DS&A, company-specific  
Difficulty: medium

Implement FIFO queue using two fixed-size stacks.

Suggested API:

```c
typedef struct {
    /* two fixed arrays or two stack_t objects */
} two_stack_queue_t;

bool tsq_push(two_stack_queue_t *q, int32_t v);
bool tsq_pop(two_stack_queue_t *q, int32_t *out);
```

Requirements:
- No malloc.
- Return false on overflow/underflow.
- Tests: FIFO order, alternating push/pop, refill after drain.

Firmware relevance:
- Low firmware signal. Include only as generic interview insurance.

---

# Optional Problem Families for Later Expansion

These are useful if the harness needs more volume, but they are not as high priority as the main list.

## O01 — `strcmp`, `strcpy`, `strncpy`

Add carefully because `strncpy` semantics are weird:
- `strncpy` pads with zeros if source shorter than count.
- It does not guarantee null termination if source length >= count.

## O02 — Remove Duplicate Characters In Place

Useful pointer/string practice. Lower firmware specificity.

## O03 — Binary Tree Height / Traversal

Only add if targeting big tech embedded roles. Do not prioritize for firmware-first preparation.

## O04 — Simple Min/Max/Average Sensor Filter

Implement moving average or median-of-3 over fixed-size buffer. Firmware-real, but less directly evidenced as an interview coding prompt.

## O05 — Flash Circular Log

Append fixed-size records with sequence number and CRC into circular flash pages. Great firmware problem, but larger than typical interview coding prompt.

---

# Suggested Harness Tags

Use these tags to organize filtering:

- `bit-manipulation`
- `registers`
- `pointers`
- `libc`
- `memory`
- `alignment`
- `endianness`
- `serialization`
- `ring-buffer`
- `state-machine`
- `parser`
- `isr`
- `concurrency`
- `timer`
- `driver`
- `fixed-point`
- `crc`
- `linked-list`
- `arrays`
- `strings`
- `ds-subset`
- `big-tech-only`

---

# Suggested Priority Cut

If adding only 15 problems, add:

1. P01 Register Bitfield Helpers
2. P02 Fixed-Size Ring Buffer
3. P03 `memcpy`, `memmove`, `memset`
4. P04 C Bug Snippet Diagnosis Pack
5. P05 Endian-Safe Serialization and Deserialization
6. P06 Packet Parser State Machine
7. P07 ISR-Safe UART RX Buffer Model
8. P08 Static Memory Pool Allocator
9. P09 Producer/Consumer Bounded Queue
10. P10 Wraparound-Safe Software Timer Scheduler
11. P11 Popcount, Bit Reverse, and Rotate Suite
12. P12 Fake SPI Register Driver
13. P14 `atoi` With Overflow Handling
14. P16 CRC-8 or CRC-16
15. P17 GPIO Debouncer State Machine

If adding only 5 after warmups, add P01 through P05.

# Running Median Filter

A median filter outputs the median of the most recent samples instead of their mean, so a short glitch, such as a single corrupted ADC read, never reaches the output, while a real step in the signal still passes through after a short delay. Implement one over a window of the last `n` samples. All of its state lives in the `median_t` the caller passes in: no `malloc`, no static or global storage. The struct layout is fixed; how you use its fields is up to you.

```c
#define MEDIAN_MAX 15

typedef struct {
    int16_t  ring[MEDIAN_MAX];
    uint8_t  n;
    uint8_t  head;
    uint8_t  count;
} median_t;

void    median_init(median_t *m, uint8_t n);
int16_t median_update(median_t *m, int16_t sample);
```

- **`median_init(m, n)`** makes `m` an empty filter with window length `n`. `m` may contain garbage bytes, or be a filter that was already in use; no sample added before this call counts afterwards.
- **`median_update(m, sample)`** adds `sample` and returns the median of the most recent `min(k, n)` samples, where `k` is the number of samples added since `median_init`, this one included. The median is the middle value of those samples in sorted order. While the window is still filling, the count can be even: then return the lower of the two middle values.

Python twin: `median_init(m, n)` and `median_update(m, sample) -> int` take a `Median` object (defined in the starter) whose `ring` list has fixed length `MEDIAN_MAX`, with int fields `n`, `head` and `count`.

### Example 1
```
median_init(&m, 5);
samples:  10  12  11  500  13  12  11
returns:  10  10  11   11  12  12  12       the 500 never reaches the output
```

### Example 2
```
median_init(&m, 5);
samples:  40  20  30  10
returns:  40  20  30  20       after 2 and 4 samples: the lower of the two middle values
```

### Example 3
```
median_init(&m, 3);
samples:  5  7  900  6  8  4
returns:  5  5    7  7  8  6
```

### Constraints
- `n` is odd and `1 <= n <= MEDIAN_MAX` (15); the caller guarantees it
- `median_update` is only called on a filter that has been through `median_init`
- Samples are any `int16_t` (Python: any int in `[-32768, 32767]`), including negatives and repeated values
- No `malloc` and no static or global variables: two `median_t`s must be fully independent
- Python: no `sorted()`, `list.sort()`, `statistics` or `heapq`, and `ring` keeps length `MEDIAN_MAX`

**Optimal complexity:** O(n) average time per sample, O(n) state

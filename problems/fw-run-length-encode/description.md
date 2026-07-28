# Run-Length Encoding

Compress a string by replacing each **run** of identical characters with the character
followed by the run's length in decimal:

```
"aaabbbcccc"  ->  "a3b3c4"
```

Implement:

```c
char *run_length_encode(char *str);
```

Return a newly `malloc`'d, null-terminated string holding the encoding. The caller owns the
result and frees it.

### Rules

- Every run gets an explicit count, **including runs of length 1**: `"abc" -> "a1b1c1"`.
- Runs can be longer than 9, so counts may be multiple digits: twelve `a`s -> `"a12"`.
- The same character can appear in separate, non-adjacent runs: `"aabbaa" -> "a2b2a2"`.
- The empty string encodes to the empty string `""` (still a valid, freeable buffer).
- Size the output buffer correctly. The worst case is all-distinct characters, where each
  input char becomes two output chars, so `2 * strlen(str) + 1` bytes always suffices.

### Examples

```
run_length_encode("aaabbc")      -> "a3b2c1"
run_length_encode("aaabbbcccc")  -> "a3b3c4"
run_length_encode("a")           -> "a1"
run_length_encode("abcde")       -> "a1b1c1d1e1"
run_length_encode("")            -> ""
```

### Watch out

- The count is written as **ASCII decimal digits**, not a raw byte. Storing `count` directly
  into the output (e.g. `output[i] = counter;`) writes a control byte, not the character `'3'`
  — a classic bug in a first pass at this problem.
- Advance through the string exactly once. Off-by-one errors from post-incrementing the read
  pointer at the wrong point will drop or double-count the first character.

### Constraints

- Input is a null-terminated ASCII string; characters are any non-`'\0'` byte.
- No fixed 256-byte scratch buffer — size the allocation to the input.

**Optimal complexity:** O(n) time, O(n) output space, single pass.

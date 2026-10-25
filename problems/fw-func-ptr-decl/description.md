# Declarations and Function-Pointer Jump Table

### Part 1: declarations

Write each of the following as a `typedef` named `T_a` … `T_h`. A typedef uses exactly the declarator you would write for a variable, with the type name where the variable name goes: the variable `unsigned char *a;` becomes `typedef unsigned char *T_x;`.

- **(a)** an integer
- **(b)** a pointer to an integer
- **(c)** a pointer to a pointer to an integer
- **(d)** an array of 10 integers
- **(e)** an array of 10 pointers to integers
- **(f)** a pointer to an array of 10 integers
- **(g)** a pointer to a function that takes an integer argument and returns an integer
- **(h)** an array of 10 pointers to functions that take an integer argument and return an integer

### Part 2: jump table

A command handler receives an opcode and two operands and dispatches through a table of function pointers indexed by the opcode. Implement:

```c
int dispatch(unsigned op, int a, int b, int *out);
```

| `op` | result |
|------|--------|
| 0 | `a + b` |
| 1 | `a - b` |
| 2 | `a * b` |
| 3 | `a / b` (C division, truncating toward zero) |

On success, store the result in `*out` and return `0`. Return `-1` and leave `*out` unchanged if `op` is not in the table, if `out` is `NULL`, or if the operation is a division by zero.

### Example 1
```
Input:  dispatch(1, 7, 5, &out)
Output: returns 0, out = 2
```

### Example 2
```
Input:  dispatch(3, -17, 5, &out)
Output: returns 0, out = -3
```

### Example 3
```
Input:  out = 42;  dispatch(4, 1, 1, &out)
Output: returns -1, out = 42
```

### Example 4
```
Input:  out = 42;  dispatch(3, 5, 0, &out)
Output: returns -1, out = 42
```

### Constraints
- Dispatch through a `static const` array of function pointers indexed by `op`, not a `switch` or `if` chain.
- Check `op` against the table's length before indexing it.
- Operands never make add, subtract, multiply or divide overflow `int`.

**Optimal complexity:** O(1) per call: one bounds check and one indirect call.

Be ready to read each Part 1 declaration aloud, and to say why the table is `const` and why `op` is unsigned.

*Source: Nigel Jones, ["A 'C' Test: The 0x10 Best Questions for Would-be Embedded Programmers"](https://web.archive.org/web/2020/https://rmbconsulting.us/publications/a-c-test-the-0x10-best-questions-for-would-be-embedded-programmers/) (Q5) and ["Arrays of Pointers to Functions"](https://barrgroup.com/embedded-systems/how-to/c-function-pointers) (Embedded Systems Programming, May 1999).*

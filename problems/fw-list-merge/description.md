# Merge Two Sorted Linked Lists

Merge two sorted singly linked lists into a single sorted list. Relink the existing nodes in place — do not allocate any new nodes.

The `node_t` type is provided. Implement:

```c
node_t *list_merge_sorted(node_t *a, node_t *b);
```

Both input lists are sorted ascending by `.value`. The result must also be sorted ascending. Equal values maintain stable order (elements from `a` appear before elements from `b` with the same value).

## Examples

**Example 1** — interleaved values
```
a: 1 → 3 → 5 → NULL
b: 2 → 4 → 6 → NULL
result: 1 → 2 → 3 → 4 → 5 → 6 → NULL
```

**Example 2** — one empty
```
a: NULL,  b: 1 → 2 → 3
result: 1 → 2 → 3
```

**Example 3** — duplicates
```
a: 1 → 2 → NULL
b: 1 → 2 → NULL
result: 1 → 1 → 2 → 2 → NULL
```

## Constraints

- No memory allocation — relink existing nodes
- Both `a` and `b` may be `NULL` (empty list)
- Stable: when values are equal, the node from `a` comes first
- Result list must be properly null-terminated

**Optimal complexity:** O(m+n)

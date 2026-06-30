# Reverse a Singly Linked List

Reverse a singly linked list in place without allocating any new nodes. Return the new head (which was the old tail).

The `node_t` type is provided with `.next` and `.value` fields.

```c
node_t *list_reverse(node_t *head);
```

## Examples

**Example 1** — five-node list
```
1 → 2 → 3 → 4 → 5 → NULL
list_reverse(head) →
5 → 4 → 3 → 2 → 1 → NULL
```

**Example 2** — edge cases
```
list_reverse(NULL)     → NULL    (empty list)
list_reverse(&a)       → &a     (single node, unchanged)
```

**Example 3** — two nodes
```
1 → 2 → NULL
list_reverse →
2 → 1 → NULL
```

## Constraints

- In-place: relink the existing nodes, no allocation
- Return the new head (old tail)
- After reversal, the last node's `next` must be `NULL`

**Optimal complexity:** O(n) — single pass

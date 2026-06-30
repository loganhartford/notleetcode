# Detect Cycle in Linked List

Detect whether a singly linked list contains a cycle using Floyd's tortoise-and-hare algorithm.

The `node_t` type is provided. Implement:

```c
bool list_has_cycle(const node_t *head);
```

A cycle exists when some node's `next` pointer points back to an earlier node in the list, making traversal infinite.

Floyd's algorithm uses two pointers: a slow pointer that advances one step at a time and a fast pointer that advances two steps. If they ever meet, there is a cycle.

## Examples

**Example 1** — no cycle
```
1 → 2 → 3 → NULL
list_has_cycle → false
```

**Example 2** — cycle at head
```
1 → 2 → 3
↑       ↓
└───────┘
list_has_cycle → true
```

**Example 3** — cycle in middle
```
1 → 2 → 3 → 4
        ↑   ↓
        └───┘
list_has_cycle → true
```

## Constraints

- `NULL` → `false`
- Single node with `next == NULL` → `false`
- Single node with `next == itself` (self-loop) → `true`
- No extra memory allocation; use only the two pointers

**Optimal complexity:** O(n) time, O(1) space

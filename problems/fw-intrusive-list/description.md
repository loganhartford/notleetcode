# Intrusive Singly Linked List

Implement operations on an intrusive singly linked list. "Intrusive" means the `next` pointer is embedded directly in each list element (the `node_t` struct) — no separate allocation for list nodes.

The `node_t` type is provided. Implement:

```c
void    list_push_front(node_t **head, node_t *node);
node_t *list_pop_front(node_t **head);
bool    list_remove(node_t **head, node_t *node);
size_t  list_count(const node_t *head);
```

- **`list_push_front`** — insert `node` at the front of the list
- **`list_pop_front`** — remove and return the front node; return `NULL` if empty
- **`list_remove`** — find and unlink `node`; return `false` if not found
- **`list_count`** — traverse and count nodes

No allocation — all nodes are provided by the caller.

## Examples

**Example 1** — push front order
```
list_push_front(&head, &a);  // head → a
list_push_front(&head, &b);  // head → b → a
list_pop_front(&head) == &b  // LIFO from front
```

**Example 2** — remove
```
list_push_front(&head, &a);
list_push_front(&head, &b);
list_push_front(&head, &c);   // head → c → b → a
list_remove(&head, &b) → true  // head → c → a
list_remove(&head, &b) → false // already removed
```

**Example 3** — count
```
list_count(NULL) → 0
// after pushing 3 nodes:
list_count(head) → 3
```

## Constraints

- `list_pop_front` on an empty list (`*head == NULL`) returns `NULL`
- `list_remove` traverses to find the node; returns `false` if not in list
- `list_count` traverses the full list
- Do not allocate any memory

**Optimal complexity:** O(1) push/pop_front, O(n) remove/count

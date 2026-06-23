# Reorder List

You are given the head of a singly linked list `L0 → L1 → … → Ln-1 → Ln`.

Reorder it **in place** to: `L0 → Ln → L1 → Ln-1 → L2 → Ln-2 → …`

You may not modify the values in the nodes — only the links between them. Return nothing (the harness reads the list from the same head).

The node type is provided by the test harness:
```c
struct ListNode { int val; struct ListNode* next; };
```

### Example 1
```
Input:  head = [1, 2, 3, 4]
After:  [1, 4, 2, 3]
```

### Example 2
```
Input:  head = [1, 2, 3, 4, 5]
After:  [1, 5, 2, 4, 3]
```

### Constraints
- The list has `1` to `5 * 10^4` nodes.
- `1 <= Node.val <= 1000`
- Classic approach: find the middle, reverse the second half, then merge the two halves.

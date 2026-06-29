# Remove Nth Node From End of List

Given the `head` of a linked list, remove the `n`-th node from the **end** of the list and return its head.

The node type is provided by the test harness:
```c
struct ListNode { int val; struct ListNode* next; };
```

### Example 1
```
Input:  head = [1, 2, 3, 4, 5], n = 2
Output: [1, 2, 3, 5]
```

### Example 2
```
Input:  head = [1], n = 1
Output: []
```

### Example 3
```
Input:  head = [1, 2], n = 1
Output: [1]
```

### Constraints
- The list has `sz` nodes, `1 <= sz <= 30`.
- `1 <= n <= sz`
- Try to do it in **one pass** (two pointers, n apart).

**Optimal complexity:** O(n)  — two-pointer single pass

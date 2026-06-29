# Reverse Linked List

Given the `head` of a singly linked list, reverse the list and return the new head.

The node type is provided by the test harness:
```c
struct ListNode { int val; struct ListNode* next; };
```

### Example 1
```
Input:  head = [1, 2, 3, 4, 5]
Output: [5, 4, 3, 2, 1]
```

### Example 2
```
Input:  head = [1, 2]
Output: [2, 1]
```

### Example 3
```
Input:  head = []
Output: []
```

### Constraints
- The list has `0` to `5000` nodes.
- `-5000 <= Node.val <= 5000`
- Do it in place; aim for O(n) time, O(1) extra space.

**Optimal complexity:** O(n)  — single-pass in-place reversal

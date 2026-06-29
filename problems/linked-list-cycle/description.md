# Linked List Cycle

Given the `head` of a singly linked list, determine whether the list has a **cycle** in it. A cycle exists if some node can be reached again by continuously following `next` pointers.

Return `True` if there is a cycle, `False` otherwise.

The node type is provided by the test harness:
```c
struct ListNode { int val; struct ListNode* next; };
```

### Example 1
```
Input:  head = [3, 2, 0, -4], tail connects to index 1
Output: True
```

### Example 2
```
Input:  head = [1, 2], tail connects to index 0
Output: True
```

### Example 3
```
Input:  head = [1], no cycle
Output: False
```

### Constraints
- The list has `0` to `10^4` nodes.
- `-10^5 <= Node.val <= 10^5`
- Solve it with **O(1)** extra memory (Floyd's tortoise and hare).

**Optimal complexity:** O(n)  — Floyd's slow/fast pointer

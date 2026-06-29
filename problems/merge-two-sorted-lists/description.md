# Merge Two Sorted Lists

You are given the heads of two sorted linked lists `list1` and `list2`.

Merge the two lists into one **sorted** list. The list should be made by splicing together the nodes of the first two lists. Return the head of the merged list.

The node type is provided by the test harness:
```c
struct ListNode { int val; struct ListNode* next; };
```

### Example 1
```
Input:  list1 = [1, 2, 4], list2 = [1, 3, 4]
Output: [1, 1, 2, 3, 4, 4]
```

### Example 2
```
Input:  list1 = [], list2 = []
Output: []
```

### Example 3
```
Input:  list1 = [], list2 = [0]
Output: [0]
```

### Constraints
- Each list has `0` to `50` nodes.
- `-100 <= Node.val <= 100`
- Both lists are sorted in non-decreasing order.

**Optimal complexity:** O(m + n)  — single pass through both lists

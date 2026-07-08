# Recursive to Iterative

Implement two linked list operations **iteratively** (no recursion).

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v, ListNode* n = nullptr) : val(v), next(n) {}
};

// Return the length of the linked list (iteratively)
int listLength(ListNode* head);

// Return all values in reverse order (iteratively)
vector<int> reverseValues(ListNode* head);
```

These are the iterative equivalents of simple recursive traversals.

## Examples

```
List: 1 → 2 → 3 → 4 → 5

listLength(head)   → 5
reverseValues(head) → [5, 4, 3, 2, 1]
```

## Constraints

- `0 <= list length <= 10^4`
- `null` head is valid (length 0, empty reverse)

**Optimal complexity:** O(n) time, O(1) space for listLength; O(n) space for reverseValues.

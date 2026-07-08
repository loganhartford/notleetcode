# Linked Structure Cycle

Given a linked structure that may contain a cycle, return the number of **distinct nodes** reachable before any node is visited twice.

```cpp
struct Node {
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

int longestNonRepeatingChain(Node* head);
```

Traverse the list starting from `head`. Count each unique node the first time it is visited. Stop when a node is visited for the second time (or when you reach `nullptr`). Return the count of unique nodes seen.

## Examples

```
null → 0
1 → null → 1
1 → 2 → 3 → null → 3

Cycle:
1 → 2 → 3 → (back to 1) → stops when 1 is seen again → 3

Tail + Cycle:
1 → 2 → 3 → 4 → (back to 3) → stops when 3 is seen again → 4
```

## Constraints

- `0 <= number of nodes <= 10^4`
- Node values may repeat; use **pointer identity** to detect revisits

**Optimal complexity:** O(n) time — hash set of visited pointers.

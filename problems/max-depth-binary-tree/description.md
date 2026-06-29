# Maximum Depth of Binary Tree

Given the `root` of a binary tree, return its **maximum depth** — the number of
nodes along the longest path from the root down to the farthest leaf.

The node type is provided by the harness:
```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None): ...
```

Trees are given as **level-order** lists with `None` for missing children.

### Example 1
```
Input:  root = [3, 9, 20, None, None, 15, 7]
Output: 3
```

### Example 2
```
Input:  root = [1, None, 2]
Output: 2
```

### Example 3
```
Input:  root = []
Output: 0
```

### Constraints
- `0 <= number of nodes <= 10^4`
- `-100 <= Node.val <= 100`

**Optimal complexity:** O(n)  — visits every node once

# Invert Binary Tree

Given the `root` of a binary tree, **invert** it (mirror it left-to-right) and
return its root.

The node type is provided by the harness:
```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right
```

Trees are given/checked as **level-order** lists with `None` for missing children.

### Example 1
```
Input:  root = [4, 2, 7, 1, 3, 6, 9]
Output: [4, 7, 2, 9, 6, 3, 1]
```

### Example 2
```
Input:  root = [2, 1, 3]
Output: [2, 3, 1]
```

### Example 3
```
Input:  root = []
Output: []
```

### Constraints
- `0 <= number of nodes <= 100`
- `-100 <= Node.val <= 100`

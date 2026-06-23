# Binary Tree Level Order Traversal

Given the `root` of a binary tree, return the **level-order traversal** of its
nodes' values — i.e. values grouped by depth, left to right, top to bottom.

The node type is provided by the harness:
```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None): ...
```

Input trees are given as level-order lists with `None` for missing children.

### Example 1
```
Input:  root = [3, 9, 20, None, None, 15, 7]
Output: [[3], [9, 20], [15, 7]]
```

### Example 2
```
Input:  root = [1]
Output: [[1]]
```

### Example 3
```
Input:  root = []
Output: []
```

### Constraints
- `0 <= number of nodes <= 2000`
- `-1000 <= Node.val <= 1000`
- This is the BFS template — process the tree one level (queue snapshot) at a time.

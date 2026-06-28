# Validate Binary Search Tree

Given the `root` of a binary tree, determine whether it is a valid **binary search
tree (BST)**.

A valid BST requires:
- the left subtree of a node contains **only** nodes with keys **less than** the
  node's key,
- the right subtree contains **only** nodes with keys **greater than** the node's key,
- both subtrees are themselves valid BSTs.

The node type is provided by the harness:
```python
class TreeNode:
    def __init__(self, val=0, left=None, right=None): ...
```

Trees are given as level-order lists with `None` for missing children.

### Example 1
```
Input:  root = [2, 1, 3]
Output: True
```

### Example 2
```
Input:  root = [5, 1, 4, None, None, 3, 6]
Output: False      # 3 is in the right subtree of 5 but 3 < 5
```

### Constraints
- `1 <= number of nodes <= 10^4`
- The classic bug is comparing only against the parent; carry **min/max bounds**
  down the recursion instead.

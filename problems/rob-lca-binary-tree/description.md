# Lowest Common Ancestor of Binary Tree

Given a binary tree and two nodes `p` and `q`, find their **lowest common ancestor (LCA)** — the deepest node that has both `p` and `q` as descendants (a node can be a descendant of itself).

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q);
```

## Example

```
Tree:       3
           / \
          5   1
         / \ / \
        6  2 0  8
          / \
         7   4

LCA(5, 1) = 3
LCA(5, 4) = 5   (5 is an ancestor of 4)
```

## Constraints

- The tree has between 2 and 10^5 nodes
- All node values are unique
- Both `p` and `q` are guaranteed to exist in the tree
- A node can be its own LCA

**Optimal complexity:** O(n) — single DFS pass; return the node when p or q is found, propagate up.

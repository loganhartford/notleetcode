from collections import deque
from solution import invertTree, TreeNode
from nlc import run_cases


def build_tree(vals):
    if not vals or vals[0] is None:
        return None
    root = TreeNode(vals[0])
    q = deque([root])
    i = 1
    while q and i < len(vals):
        node = q.popleft()
        if i < len(vals):
            if vals[i] is not None:
                node.left = TreeNode(vals[i])
                q.append(node.left)
            i += 1
        if i < len(vals):
            if vals[i] is not None:
                node.right = TreeNode(vals[i])
                q.append(node.right)
            i += 1
    return root


def serialize(root):
    if not root:
        return []
    out = []
    q = deque([root])
    while q:
        node = q.popleft()
        if node:
            out.append(node.val)
            q.append(node.left)
            q.append(node.right)
        else:
            out.append(None)
    while out and out[-1] is None:
        out.pop()
    return out


def drive(vals):
    return serialize(invertTree(build_tree(vals)))


CASES = [
    {"name": "full tree", "args": [[4, 2, 7, 1, 3, 6, 9]], "expected": [4, 7, 2, 9, 6, 3, 1]},
    {"name": "small", "args": [[2, 1, 3]], "expected": [2, 3, 1]},
    {"name": "empty", "args": [[]], "expected": []},
    {"name": "single", "args": [[1]], "expected": [1]},
    {"name": "lopsided", "args": [[1, 2, None, 3]], "expected": [1, None, 2, None, 3]},
]

run_cases(drive, CASES, fmt_input=lambda c: "root={}".format(c["args"][0]))

from collections import deque
from solution import isValidBST, TreeNode
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


def drive(vals):
    return isValidBST(build_tree(vals))


CASES = [
    {"name": "valid small", "args": [[2, 1, 3]], "expected": True},
    {"name": "deep violation", "args": [[5, 1, 4, None, None, 3, 6]], "expected": False},
    {"name": "single", "args": [[1]], "expected": True},
    {"name": "equal not allowed", "args": [[2, 2, 2]], "expected": False},
    {"name": "valid larger", "args": [[8, 4, 12, 2, 6, 10, 14]], "expected": True},
    {"name": "right subtree sneaky", "args": [[10, 5, 15, None, None, 6, 20]], "expected": False},
]

run_cases(drive, CASES, fmt_input=lambda c: "root={}".format(c["args"][0]))

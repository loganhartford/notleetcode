from collections import deque
from solution import maxDepth, TreeNode
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
    return maxDepth(build_tree(vals))


CASES = [
    {"name": "balanced-ish", "args": [[3, 9, 20, None, None, 15, 7]], "expected": 3},
    {"name": "right chain", "args": [[1, None, 2]], "expected": 2},
    {"name": "empty", "args": [[]], "expected": 0},
    {"name": "single", "args": [[5]], "expected": 1},
    {"name": "left heavy", "args": [[1, 2, None, 3, None, 4]], "expected": 4},
]

run_cases(drive, CASES, fmt_input=lambda c: "root={}".format(c["args"][0]))

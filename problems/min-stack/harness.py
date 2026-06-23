from solution import MinStack
from nlc import run_cases


def drive(operations, arguments):
    """Replay a LeetCode-style operation sequence, collecting return values."""
    obj = None
    out = []
    for op, arg in zip(operations, arguments):
        if op == "MinStack":
            obj = MinStack()
            out.append(None)
        elif op == "push":
            obj.push(arg[0])
            out.append(None)
        elif op == "pop":
            obj.pop()
            out.append(None)
        elif op == "top":
            out.append(obj.top())
        elif op == "getMin":
            out.append(obj.getMin())
        else:
            raise ValueError("unknown op " + op)
    return out


CASES = [
    {"name": "example",
     "args": [["MinStack", "push", "push", "push", "getMin", "pop", "top", "getMin"],
              [[], [-2], [0], [-3], [], [], [], []]],
     "expected": [None, None, None, None, -3, None, 0, -2]},
    {"name": "duplicates as min",
     "args": [["MinStack", "push", "push", "push", "getMin", "pop", "getMin"],
              [[], [1], [1], [0], [], [], []]],
     "expected": [None, None, None, None, 0, None, 1]},
    {"name": "single",
     "args": [["MinStack", "push", "top", "getMin"], [[], [5], [], []]],
     "expected": [None, None, 5, 5]},
]

run_cases(
    drive, CASES,
    fmt_input=lambda c: "ops={}, args={}".format(c["args"][0], c["args"][1]),
)

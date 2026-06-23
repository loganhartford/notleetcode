from solution import MyQueue
from nlc import run_cases


def drive(operations, arguments):
    obj = None
    out = []
    for op, arg in zip(operations, arguments):
        if op == "MyQueue":
            obj = MyQueue()
            out.append(None)
        elif op == "push":
            obj.push(arg[0])
            out.append(None)
        elif op == "pop":
            out.append(obj.pop())
        elif op == "peek":
            out.append(obj.peek())
        elif op == "empty":
            out.append(obj.empty())
        else:
            raise ValueError("unknown op " + op)
    return out


CASES = [
    {"name": "example",
     "args": [["MyQueue", "push", "push", "peek", "pop", "empty"],
              [[], [1], [2], [], [], []]],
     "expected": [None, None, None, 1, 1, False]},
    {"name": "drain to empty",
     "args": [["MyQueue", "push", "pop", "empty"], [[], [3], [], []]],
     "expected": [None, None, 3, True]},
    {"name": "interleaved",
     "args": [["MyQueue", "push", "push", "pop", "push", "peek", "pop", "pop", "empty"],
              [[], [1], [2], [], [3], [], [], [], []]],
     "expected": [None, None, None, 1, None, 2, 2, 3, True]},
]

run_cases(
    drive, CASES,
    fmt_input=lambda c: "ops={}, args={}".format(c["args"][0], c["args"][1]),
)

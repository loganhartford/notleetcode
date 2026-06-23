from solution import canFinish
from nlc import run_cases

CASES = [
    {"name": "linear ok", "args": [2, [[1, 0]]], "expected": True},
    {"name": "cycle", "args": [2, [[1, 0], [0, 1]]], "expected": False},
    {"name": "no prereqs", "args": [3, []], "expected": True},
    {"name": "chain", "args": [4, [[1, 0], [2, 1], [3, 2]]], "expected": True},
    {"name": "3-cycle", "args": [3, [[0, 1], [1, 2], [2, 0]]], "expected": False},
    {"name": "diamond", "args": [4, [[1, 0], [2, 0], [3, 1], [3, 2]]], "expected": True},
]

run_cases(canFinish, CASES, fmt_input=lambda c: "numCourses={}, prerequisites={}".format(*c["args"]))

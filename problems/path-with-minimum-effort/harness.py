from solution import minimumEffortPath
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[[1, 2, 2], [3, 8, 2], [5, 3, 5]]], "expected": 2},
    {"name": "example 2", "args": [[[1, 2, 3], [3, 8, 4], [5, 3, 5]]], "expected": 1},
    {"name": "zig-zag flat path", "args": [[[1, 2, 1, 1, 1], [1, 2, 1, 2, 1], [1, 2, 1, 2, 1], [1, 2, 1, 2, 1], [1, 1, 1, 2, 1]]], "expected": 0},
    {"name": "single cell", "args": [[[5]]], "expected": 0},
    {"name": "single row", "args": [[[1, 10, 6, 7, 9, 10, 4, 9]]], "expected": 9},
]

run_cases(minimumEffortPath, CASES, fmt_input=lambda c: "heights (rows={})".format(len(c["args"][0])))

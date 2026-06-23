from solution import orangesRotting
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[[2, 1, 1], [1, 1, 0], [0, 1, 1]]], "expected": 4},
    {"name": "impossible", "args": [[[2, 1, 1], [0, 1, 1], [1, 0, 1]]], "expected": -1},
    {"name": "none fresh", "args": [[[0, 2]]], "expected": 0},
    {"name": "all empty", "args": [[[0, 0], [0, 0]]], "expected": 0},
    {"name": "single fresh blocked", "args": [[[1]]], "expected": -1},
    {"name": "row spread", "args": [[[2, 1, 1, 1, 1]]], "expected": 4},
]

run_cases(orangesRotting, CASES, fmt_input=lambda c: "grid={}".format(c["args"][0]))

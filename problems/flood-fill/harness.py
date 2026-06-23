from solution import floodFill
from nlc import run_cases

CASES = [
    {"name": "basic", "args": [[[1, 1, 1], [1, 1, 0], [1, 0, 1]], 1, 1, 2],
     "expected": [[2, 2, 2], [2, 2, 0], [2, 0, 1]]},
    {"name": "same color (no loop)", "args": [[[0, 0, 0], [0, 0, 0]], 0, 0, 0],
     "expected": [[0, 0, 0], [0, 0, 0]]},
    {"name": "corner", "args": [[[0, 0, 0], [0, 1, 1]], 1, 1, 5],
     "expected": [[0, 0, 0], [0, 5, 5]]},
    {"name": "whole grid", "args": [[[1, 1], [1, 1]], 0, 0, 9],
     "expected": [[9, 9], [9, 9]]},
]

run_cases(
    floodFill, CASES,
    compare=lambda got, c, a: got is not None and [list(r) for r in got] == c["expected"],
    fmt_input=lambda c: "image={}, sr={}, sc={}, color={}".format(*c["args"]),
)

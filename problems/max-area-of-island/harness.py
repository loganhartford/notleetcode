from solution import maxAreaOfIsland
from nlc import run_cases

CASES = [
    {"name": "big island", "args": [[
        [0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0],
        [0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0],
        [0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0]]], "expected": 6},
    {"name": "no island", "args": [[[0, 0, 0, 0, 0, 0, 0, 0]]], "expected": 0},
    {"name": "single cell", "args": [[[1]]], "expected": 1},
    {"name": "full grid", "args": [[[1, 1], [1, 1]]], "expected": 4},
    {"name": "two islands", "args": [[[1, 1, 0], [0, 0, 0], [0, 1, 1]]], "expected": 2},
]

run_cases(maxAreaOfIsland, CASES, fmt_input=lambda c: "grid (rows={})".format(len(c["args"][0])))

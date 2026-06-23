from solution import swimInWater
from nlc import run_cases

CASES = [
    {"name": "2x2", "args": [[[0, 2], [1, 3]]], "expected": 3},
    {"name": "5x5 spiral", "args": [[[0, 1, 2, 3, 4], [24, 23, 22, 21, 5], [12, 13, 14, 15, 16], [11, 17, 18, 19, 20], [10, 9, 8, 7, 6]]], "expected": 16},
    {"name": "1x1", "args": [[[0]]], "expected": 0},
    {"name": "3x3", "args": [[[0, 1, 2], [5, 6, 3], [8, 7, 4]]], "expected": 4},
]

run_cases(swimInWater, CASES, fmt_input=lambda c: "grid (n={})".format(len(c["args"][0])))

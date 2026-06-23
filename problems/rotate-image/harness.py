from solution import rotate
from nlc import run_cases

CASES = [
    {"name": "3x3", "args": [[[1, 2, 3], [4, 5, 6], [7, 8, 9]]],
     "expected": [[7, 4, 1], [8, 5, 2], [9, 6, 3]]},
    {"name": "4x4", "args": [[[5, 1, 9, 11], [2, 4, 8, 10], [13, 3, 6, 7], [15, 14, 12, 16]]],
     "expected": [[15, 13, 2, 5], [14, 3, 4, 1], [12, 6, 8, 9], [16, 7, 10, 11]]},
    {"name": "1x1", "args": [[[1]]], "expected": [[1]]},
    {"name": "2x2", "args": [[[1, 2], [3, 4]]], "expected": [[3, 1], [4, 2]]},
]

run_cases(
    rotate, CASES,
    compare=lambda got, c, a: got is None and a[0] == c["expected"],
    fmt_input=lambda c: "matrix={}".format(c["args"][0]),
)

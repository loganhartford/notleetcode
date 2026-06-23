from solution import spiralOrder
from nlc import run_cases

CASES = [
    {"name": "3x3", "args": [[[1, 2, 3], [4, 5, 6], [7, 8, 9]]],
     "expected": [1, 2, 3, 6, 9, 8, 7, 4, 5]},
    {"name": "3x4", "args": [[[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]]],
     "expected": [1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7]},
    {"name": "single row", "args": [[[1, 2, 3]]], "expected": [1, 2, 3]},
    {"name": "single col", "args": [[[1], [2], [3]]], "expected": [1, 2, 3]},
    {"name": "1x1", "args": [[[7]]], "expected": [7]},
]

run_cases(
    spiralOrder, CASES,
    compare=lambda got, c, a: got is not None and list(got) == c["expected"],
    fmt_input=lambda c: "matrix={}".format(c["args"][0]),
)

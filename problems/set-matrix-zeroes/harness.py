from solution import setZeroes
from nlc import run_cases

CASES = [
    {"name": "center zero", "args": [[[1, 1, 1], [1, 0, 1], [1, 1, 1]]],
     "expected": [[1, 0, 1], [0, 0, 0], [1, 0, 1]]},
    {"name": "edge zeros", "args": [[[0, 1, 2, 0], [3, 4, 5, 2], [1, 3, 1, 5]]],
     "expected": [[0, 0, 0, 0], [0, 4, 5, 0], [0, 3, 1, 0]]},
    {"name": "no zeros", "args": [[[1, 2], [3, 4]]], "expected": [[1, 2], [3, 4]]},
    {"name": "all zero row", "args": [[[1, 2, 3], [0, 0, 0]]], "expected": [[0, 0, 0], [0, 0, 0]]},
    {"name": "1x1 zero", "args": [[[0]]], "expected": [[0]]},
]

run_cases(
    setZeroes, CASES,
    compare=lambda got, c, a: got is None and a[0] == c["expected"],
    fmt_input=lambda c: "matrix={}".format(c["args"][0]),
)

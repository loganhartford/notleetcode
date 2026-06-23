from solution import findMin
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[3, 4, 5, 1, 2]], "expected": 1},
    {"name": "example 2", "args": [[4, 5, 6, 7, 0, 1, 2]], "expected": 0},
    {"name": "not rotated", "args": [[11, 13, 15, 17]], "expected": 11},
    {"name": "single", "args": [[1]], "expected": 1},
    {"name": "two rotated", "args": [[2, 1]], "expected": 1},
    {"name": "min at end", "args": [[3, 4, 5, 6, 1]], "expected": 1},
]

run_cases(findMin, CASES, fmt_input=lambda c: "nums={}".format(c["args"][0]))

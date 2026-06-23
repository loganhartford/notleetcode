from solution import productExceptSelf
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[1, 2, 3, 4]], "expected": [24, 12, 8, 6]},
    {"name": "with zero", "args": [[-1, 1, 0, -3, 3]], "expected": [0, 0, 9, 0, 0]},
    {"name": "two elements", "args": [[2, 3]], "expected": [3, 2]},
    {"name": "two zeros", "args": [[0, 0]], "expected": [0, 0]},
    {"name": "negatives", "args": [[-1, -2, -3]], "expected": [6, 3, 2]},
]

run_cases(
    productExceptSelf, CASES,
    compare=lambda got, c, a: got is not None and list(got) == c["expected"],
    fmt_input=lambda c: "nums={}".format(c["args"][0]),
)

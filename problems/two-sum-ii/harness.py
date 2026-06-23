from solution import twoSum
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[2, 7, 11, 15], 9], "expected": [1, 2]},
    {"name": "example 2", "args": [[2, 3, 4], 6], "expected": [1, 3]},
    {"name": "negatives", "args": [[-1, 0], -1], "expected": [1, 2]},
    {"name": "adjacent at end", "args": [[1, 2, 3, 4, 4, 9, 56, 90], 8], "expected": [4, 5]},
    {"name": "first and last", "args": [[1, 5, 9], 10], "expected": [1, 3]},
]

run_cases(
    twoSum, CASES,
    compare=lambda got, c, a: got is not None and list(got) == c["expected"],
    fmt_input=lambda c: "numbers={}, target={}".format(c["args"][0], c["args"][1]),
)

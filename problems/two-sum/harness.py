from solution import twoSum
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[2, 7, 11, 15], 9], "expected": [0, 1]},
    {"name": "example 2", "args": [[3, 2, 4], 6], "expected": [1, 2]},
    {"name": "duplicate values", "args": [[3, 3], 6], "expected": [0, 1]},
    {"name": "negatives", "args": [[-1, -2, -3, -4, -5], -8], "expected": [2, 4]},
    {"name": "spread out", "args": [[0, 4, 3, 0], 0], "expected": [0, 3]},
]

# Order of the two indices doesn't matter.
run_cases(
    twoSum, CASES,
    compare=lambda got, c, a: got is not None and sorted(got) == sorted(c["expected"]),
    fmt_input=lambda c: "nums={}, target={}".format(c["args"][0], c["args"][1]),
)

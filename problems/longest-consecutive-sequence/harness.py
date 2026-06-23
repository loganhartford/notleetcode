from solution import longestConsecutive
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[100, 4, 200, 1, 3, 2]], "expected": 4},
    {"name": "example 2", "args": [[0, 3, 7, 2, 5, 8, 4, 6, 0, 1]], "expected": 9},
    {"name": "empty", "args": [[]], "expected": 0},
    {"name": "single", "args": [[5]], "expected": 1},
    {"name": "with dups", "args": [[1, 2, 2, 3]], "expected": 3},
    {"name": "negatives", "args": [[-3, -2, -1, 0, 5]], "expected": 4},
]

run_cases(longestConsecutive, CASES, fmt_input=lambda c: "nums={}".format(c["args"][0]))

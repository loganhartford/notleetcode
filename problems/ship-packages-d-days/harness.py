from solution import shipWithinDays
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 5], "expected": 15},
    {"name": "example 2", "args": [[3, 2, 2, 4, 1, 4], 3], "expected": 6},
    {"name": "example 3", "args": [[1, 2, 3, 1, 1], 4], "expected": 3},
    {"name": "one day", "args": [[1, 2, 3, 4, 5], 1], "expected": 15},
    {"name": "each its own day", "args": [[5, 5, 5, 5], 4], "expected": 5},
]

run_cases(shipWithinDays, CASES, fmt_input=lambda c: "weights={}, days={}".format(*c["args"]))

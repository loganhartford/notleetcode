from solution import search
from nlc import run_cases

CASES = [
    {"name": "in right half", "args": [[4, 5, 6, 7, 0, 1, 2], 0], "expected": 4},
    {"name": "not present", "args": [[4, 5, 6, 7, 0, 1, 2], 3], "expected": -1},
    {"name": "single miss", "args": [[1], 0], "expected": -1},
    {"name": "in left half", "args": [[4, 5, 6, 7, 0, 1, 2], 5], "expected": 1},
    {"name": "not rotated", "args": [[1, 2, 3, 4, 5], 4], "expected": 3},
    {"name": "pivot value", "args": [[6, 7, 8, 1, 2, 3, 4, 5], 8], "expected": 2},
]

run_cases(search, CASES, fmt_input=lambda c: "nums={}, target={}".format(*c["args"]))

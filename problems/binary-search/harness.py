from solution import search
from nlc import run_cases

CASES = [
    {"name": "found middle", "args": [[-1, 0, 3, 5, 9, 12], 9], "expected": 4},
    {"name": "not found", "args": [[-1, 0, 3, 5, 9, 12], 2], "expected": -1},
    {"name": "first", "args": [[1, 2, 3, 4, 5], 1], "expected": 0},
    {"name": "last", "args": [[1, 2, 3, 4, 5], 5], "expected": 4},
    {"name": "single hit", "args": [[5], 5], "expected": 0},
    {"name": "single miss", "args": [[5], -5], "expected": -1},
]

run_cases(search, CASES, fmt_input=lambda c: "nums={}, target={}".format(*c["args"]))

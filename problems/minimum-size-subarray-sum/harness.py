from solution import minSubArrayLen
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [7, [2, 3, 1, 2, 4, 3]], "expected": 2},
    {"name": "single suffices", "args": [4, [1, 4, 4]], "expected": 1},
    {"name": "impossible", "args": [11, [1, 1, 1, 1, 1, 1, 1, 1]], "expected": 0},
    {"name": "whole array", "args": [15, [1, 2, 3, 4, 5]], "expected": 5},
    {"name": "exact single", "args": [5, [5]], "expected": 1},
    {"name": "front window", "args": [6, [10, 2, 3]], "expected": 1},
]

run_cases(minSubArrayLen, CASES, fmt_input=lambda c: "target={}, nums={}".format(*c["args"]))

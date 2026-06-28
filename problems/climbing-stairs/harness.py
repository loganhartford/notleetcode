from solution import climbStairs
from nlc import run_cases

CASES = [
    {"name": "n=2", "args": [2], "expected": 2},
    {"name": "n=3", "args": [3], "expected": 3},
    {"name": "n=5", "args": [5], "expected": 8},
    {"name": "n=1", "args": [1], "expected": 1},
    {"name": "n=10", "args": [10], "expected": 89},
    {"name": "n=45", "args": [45], "expected": 1836311903},
]

run_cases(climbStairs, CASES, fmt_input=lambda c: "n={}".format(c["args"][0]))

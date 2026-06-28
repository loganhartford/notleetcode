from solution import rob
from nlc import run_cases

CASES = [
    {"name": "small", "args": [[1, 2, 3, 1]], "expected": 4},
    {"name": "skip pattern", "args": [[2, 7, 9, 3, 1]], "expected": 12},
    {"name": "single", "args": [[5]], "expected": 5},
    {"name": "two houses", "args": [[2, 1]], "expected": 2},
    {"name": "all equal", "args": [[4, 4, 4, 4]], "expected": 8},
    {"name": "zeros", "args": [[0, 0, 0]], "expected": 0},
]

run_cases(rob, CASES, fmt_input=lambda c: "nums={}".format(c["args"][0]))

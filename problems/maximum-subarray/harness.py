from solution import maxSubArray
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[-2, 1, -3, 4, -1, 2, 1, -5, 4]], "expected": 6},
    {"name": "single", "args": [[1]], "expected": 1},
    {"name": "all positive", "args": [[5, 4, -1, 7, 8]], "expected": 23},
    {"name": "all negative", "args": [[-3, -1, -2]], "expected": -1},
    {"name": "single negative", "args": [[-5]], "expected": -5},
    {"name": "zeros and neg", "args": [[-2, 0, -1]], "expected": 0},
]

run_cases(maxSubArray, CASES, fmt_input=lambda c: "nums={}".format(c["args"][0]))

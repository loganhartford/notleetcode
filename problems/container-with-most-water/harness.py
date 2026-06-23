from solution import maxArea
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[1, 8, 6, 2, 5, 4, 8, 3, 7]], "expected": 49},
    {"name": "two lines", "args": [[1, 1]], "expected": 1},
    {"name": "increasing", "args": [[1, 2, 3, 4, 5]], "expected": 6},
    {"name": "peak middle", "args": [[2, 3, 10, 5, 7, 8, 9]], "expected": 36},
    {"name": "flat tall", "args": [[4, 3, 2, 1, 4]], "expected": 16},
]

run_cases(maxArea, CASES, fmt_input=lambda c: "height={}".format(c["args"][0]))

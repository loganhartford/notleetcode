from solution import maxProfit
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[7, 1, 5, 3, 6, 4]], "expected": 5},
    {"name": "no profit", "args": [[7, 6, 4, 3, 1]], "expected": 0},
    {"name": "single day", "args": [[5]], "expected": 0},
    {"name": "increasing", "args": [[1, 2, 3, 4, 5]], "expected": 4},
    {"name": "dip then rise", "args": [[3, 2, 6, 5, 0, 3]], "expected": 4},
    {"name": "two days", "args": [[2, 4]], "expected": 2},
]

run_cases(maxProfit, CASES, fmt_input=lambda c: "prices={}".format(c["args"][0]))

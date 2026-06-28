from solution import coinChange
from nlc import run_cases

CASES = [
    {"name": "11 from [1,2,5]", "args": [[1, 2, 5], 11], "expected": 3},
    {"name": "impossible", "args": [[2], 3], "expected": -1},
    {"name": "zero amount", "args": [[1], 0], "expected": 0},
    {"name": "exact single coin", "args": [[1, 2, 5], 5], "expected": 1},
    {"name": "greedy trap", "args": [[1, 3, 4], 6], "expected": 2},
    {"name": "larger", "args": [[186, 419, 83, 408], 6249], "expected": 20},
]

run_cases(coinChange, CASES, fmt_input=lambda c: "coins={}, amount={}".format(*c["args"]))

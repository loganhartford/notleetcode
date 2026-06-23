from solution import containsDuplicate
from nlc import run_cases

CASES = [
    {"name": "has duplicate", "args": [[1, 2, 3, 1]], "expected": True},
    {"name": "all distinct", "args": [[1, 2, 3, 4]], "expected": False},
    {"name": "many dups", "args": [[1, 1, 1, 3, 3, 4, 3, 2, 4, 2]], "expected": True},
    {"name": "single", "args": [[7]], "expected": False},
    {"name": "two same", "args": [[2, 2]], "expected": True},
    {"name": "negatives", "args": [[-1, -2, -3, -1]], "expected": True},
]

run_cases(containsDuplicate, CASES, fmt_input=lambda c: "nums={}".format(c["args"][0]))

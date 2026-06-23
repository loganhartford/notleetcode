from solution import isValid
from nlc import run_cases

CASES = [
    {"name": "simple pair", "args": ["()"], "expected": True},
    {"name": "all types", "args": ["()[]{}"], "expected": True},
    {"name": "mismatch", "args": ["(]"], "expected": False},
    {"name": "wrong order", "args": ["([)]"], "expected": False},
    {"name": "nested", "args": ["{[]}"], "expected": True},
    {"name": "unclosed", "args": ["("], "expected": False},
    {"name": "extra close", "args": ["){"], "expected": False},
]

run_cases(isValid, CASES, fmt_input=lambda c: "s={!r}".format(c["args"][0]))

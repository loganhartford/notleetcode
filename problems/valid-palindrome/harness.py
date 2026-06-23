from solution import isPalindrome
from nlc import run_cases

CASES = [
    {"name": "panama", "args": ["A man, a plan, a canal: Panama"], "expected": True},
    {"name": "race a car", "args": ["race a car"], "expected": False},
    {"name": "blank", "args": [" "], "expected": True},
    {"name": "alphanumeric", "args": ["0P"], "expected": False},
    {"name": "digits palindrome", "args": ["1a2b2a1"], "expected": True},
    {"name": "single char", "args": ["x"], "expected": True},
]

run_cases(isPalindrome, CASES, fmt_input=lambda c: "s={!r}".format(c["args"][0]))

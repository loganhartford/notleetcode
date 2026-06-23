from solution import lengthOfLongestSubstring
from nlc import run_cases

CASES = [
    {"name": "abcabcbb", "args": ["abcabcbb"], "expected": 3},
    {"name": "bbbbb", "args": ["bbbbb"], "expected": 1},
    {"name": "pwwkew", "args": ["pwwkew"], "expected": 3},
    {"name": "empty", "args": [""], "expected": 0},
    {"name": "spaces", "args": [" "], "expected": 1},
    {"name": "tmmzuxt", "args": ["tmmzuxt"], "expected": 5},
    {"name": "dvdf", "args": ["dvdf"], "expected": 3},
]

run_cases(lengthOfLongestSubstring, CASES, fmt_input=lambda c: "s={!r}".format(c["args"][0]))

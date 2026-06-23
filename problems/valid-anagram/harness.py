from solution import isAnagram
from nlc import run_cases

CASES = [
    {"name": "anagram", "args": ["anagram", "nagaram"], "expected": True},
    {"name": "not anagram", "args": ["rat", "car"], "expected": False},
    {"name": "different lengths", "args": ["a", "ab"], "expected": False},
    {"name": "same string", "args": ["abc", "abc"], "expected": True},
    {"name": "repeated letters", "args": ["aacc", "ccac"], "expected": False},
    {"name": "single char", "args": ["a", "a"], "expected": True},
]

run_cases(isAnagram, CASES, fmt_input=lambda c: "s={!r}, t={!r}".format(*c["args"]))

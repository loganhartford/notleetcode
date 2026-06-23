from solution import reverseString
from nlc import run_cases

CASES = [
    {"name": "hello", "args": [["h", "e", "l", "l", "o"]], "expected": ["o", "l", "l", "e", "h"]},
    {"name": "Hannah", "args": [["H", "a", "n", "n", "a", "h"]], "expected": ["h", "a", "n", "n", "a", "H"]},
    {"name": "single", "args": [["x"]], "expected": ["x"]},
    {"name": "two", "args": [["a", "b"]], "expected": ["b", "a"]},
]

# reverseString mutates its argument in place and must return None; check both.
run_cases(
    reverseString, CASES,
    compare=lambda got, c, a: got is None and a[0] == c["expected"],
    fmt_input=lambda c: repr(c["args"][0]),
    show=lambda v: repr(v),
)

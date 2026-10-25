from solution import c_strlcpy
from nlc import run_cases


def exercise(cap, src):
    dst = bytearray(b"X" * cap + b"\xA5" * (32 - cap))
    result = c_strlcpy(dst, src, cap)
    source_length = src.index(0)
    checked = min(source_length, cap - 1) + 1 if cap else 0
    violations = []
    if len(dst) != 32:
        violations.append(f"destination resized: {len(dst)} bytes (expected 32)")
    for i in range(cap, len(dst)):
        if dst[i] != 0xA5:
            violations.append(f"out-of-bounds write: dst[{i}]=0x{dst[i]:02X} (expected 0xA5)")
            break
    return result, bytes(dst[:checked]), violations


CASES = [
    {"name": "exact fit", "args": [4, b"cat\0"], "expected": (3, b"cat\0", [])},
    {"name": "room left over", "args": [8, b"hi\0"], "expected": (2, b"hi\0", [])},
    {"name": "truncate long source", "args": [4, b"hello\0"], "expected": (5, b"hel\0", [])},
    {"name": "one byte", "args": [1, b"hello\0"], "expected": (5, b"\0", [])},
    {"name": "zero capacity", "args": [0, b"hello\0"], "expected": (5, b"", [])},
    {"name": "empty source", "args": [4, b"\0"], "expected": (0, b"\0", [])},
    {"name": "empty into one byte", "args": [1, b"\0"], "expected": (0, b"\0", [])},
    {"name": "empty with zero capacity", "args": [0, b"\0"], "expected": (0, b"", [])},
    {"name": "stop at first NUL", "args": [8, b"a\0ignored\0"], "expected": (1, b"a\0", [])},
    {"name": "one byte short of exact fit", "args": [3, b"cat\0"], "expected": (3, b"ca\0", [])},
]


def format_input(case):
    cap, src = case["args"]
    return f"c_strlcpy(dst, src bytes={src!r}, dstsize={cap}), initial dst: {cap} 'X' bytes"


def format_result(result):
    returned, prefix, violations = result
    return f"return={returned}, result bytes={prefix!r}" + ("; " + "; ".join(violations) if violations else "")


run_cases(exercise, CASES, fmt_input=format_input, show=format_result)

Q15_MAX = 32767
Q15_MIN = -32768


def _sat(x: int) -> int:
    return max(Q15_MIN, min(Q15_MAX, x))


def q15_add_sat(a: int, b: int) -> int:
    return _sat(a + b)


def q15_sub_sat(a: int, b: int) -> int:
    return _sat(a - b)


def q15_mul(a: int, b: int) -> int:
    return _sat((a * b + (1 << 14)) >> 15)

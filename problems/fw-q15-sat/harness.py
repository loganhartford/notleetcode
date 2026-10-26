import math
from fractions import Fraction

from solution import q15_add_sat, q15_sub_sat, q15_mul
from nlc import run_cases

H_OPS = {"q15_add_sat": q15_add_sat, "q15_sub_sat": q15_sub_sat, "q15_mul": q15_mul}

H_EDGES = [
    -32768, -32767, -32766, -16385, -16384, -16383, -8192, -3, -2, -1,
    0, 1, 2, 3, 8192, 16383, 16384, 16385, 32766, 32767,
]
H_NRANDOM = 4000
H_NMIXED = 1000


def h_clamp(v):
    return max(-32768, min(32767, v))


# Slow reference, straight from the definition: exact rational math, then clamp.
H_EXPECT = {
    "q15_add_sat": lambda a, b: h_clamp(a + b),
    "q15_sub_sat": lambda a, b: h_clamp(a - b),
    "q15_mul": lambda a, b: h_clamp(math.floor(Fraction(a * b, 32768) + Fraction(1, 2))),
}


def h_build_pairs():
    state = 0x2545F491

    def rand_q15():
        nonlocal state
        state = (state ^ (state << 13)) & 0xFFFFFFFF
        state ^= state >> 17
        state = (state ^ (state << 5)) & 0xFFFFFFFF
        return (state & 0xFFFF) - 32768

    pairs = [(a, b) for a in H_EDGES for b in H_EDGES]
    for _ in range(H_NRANDOM):
        a = rand_q15()
        b = rand_q15()
        pairs.append((a, b))
    for k in range(H_NMIXED):
        pairs.append((rand_q15(), H_EDGES[k % len(H_EDGES)]))
        pairs.append((H_EDGES[k % len(H_EDGES)], rand_q15()))
    return pairs


H_PAIRS = h_build_pairs()
H_ALL_OK = "all {} correct".format(len(H_PAIRS))


def h_fmt(v):
    if type(v) is not int:
        return repr(v)
    return "{} (0x{:04X}, {:.6g})".format(v, v & 0xFFFF, v / 32768)


def h_sweep(op):
    fn, ref = H_OPS[op], H_EXPECT[op]
    wrong, first = 0, None
    for a, b in H_PAIRS:
        got = fn(a, b)
        exp = ref(a, b)
        if type(got) is not int or got != exp:
            if first is None:
                first = (a, b, exp, got)
            wrong += 1
    if not wrong:
        return H_ALL_OK
    a, b, exp, got = first
    return "{}({}, {}) returned {}, expected {}; {} of {} wrong".format(
        op, a, b, h_fmt(got), h_fmt(exp), wrong, len(H_PAIRS))


def h_run(op, a=None, b=None):
    if a is None:
        return h_sweep(op)
    return H_OPS[op](a, b)


def h_case(name, op, a, b, expected):
    return {"name": name, "args": [op, a, b], "expected": expected}


def h_sweep_case(op):
    return {"name": op + " sweep", "args": [op], "expected": H_ALL_OK}


CASES = [
    h_case("add 0.5 + 0.25",           "q15_add_sat",  16384,   8192,  24576),
    h_case("add positive overflow",    "q15_add_sat",  28672,   8192,  32767),
    h_case("add negative overflow",    "q15_add_sat", -28672,  -8192, -32768),
    h_case("add max + 1 LSB",          "q15_add_sat",  32767,      1,  32767),
    h_case("add min + (-1 LSB)",       "q15_add_sat", -32768,     -1, -32768),
    h_case("add min + min",            "q15_add_sat", -32768, -32768, -32768),
    h_case("add max + min",            "q15_add_sat",  32767, -32768,     -1),

    h_case("sub 0.25 - 0.75",          "q15_sub_sat",   8192,  24576, -16384),
    h_case("sub max - (-1 LSB)",       "q15_sub_sat",  32767,     -1,  32767),
    h_case("sub min - 1 LSB",          "q15_sub_sat", -32768,      1, -32768),
    h_case("sub 0 - (-1.0)",           "q15_sub_sat",      0, -32768,  32767),
    h_case("sub min - min",            "q15_sub_sat", -32768, -32768,      0),

    h_case("mul 0.5 * 0.5",            "q15_mul",  16384,  16384,   8192),
    h_case("mul -1.0 * 0.5",           "q15_mul", -32768,  16384, -16384),
    h_case("mul -1.0 * max",           "q15_mul", -32768,  32767, -32767),
    h_case("mul max * max",            "q15_mul",  32767,  32767,  32766),
    h_case("mul -1.0 * -1.0",          "q15_mul", -32768, -32768,  32767),
    h_case("mul -1.0 * -1 LSB",        "q15_mul", -32768,     -1,      1),
    h_case("mul positive tie",         "q15_mul",      1,  16384,      1),
    h_case("mul negative tie",         "q15_mul",     -1,  16384,      0),
    h_case("mul negative tie at -1.5", "q15_mul",     -3,  16384,     -1),
    h_case("mul 0.5 * max (tie)",      "q15_mul",  16384,  32767,  16384),
    h_case("mul -0.5 * max (tie)",     "q15_mul", -16384,  32767, -16383),
    h_case("mul rounds 1.75 up",       "q15_mul",      7,   8192,      2),
    h_case("mul rounds -1.75 down",    "q15_mul",     -7,   8192,     -2),

    h_sweep_case("q15_add_sat"),
    h_sweep_case("q15_sub_sat"),
    h_sweep_case("q15_mul"),
]


def h_input(case):
    op = case["args"][0]
    if len(case["args"]) == 1:
        return "{}(a, b) over {} pairs (every pair of {} edge values + {} pseudo-random)".format(
            op, len(H_PAIRS), len(H_EDGES), H_NRANDOM + 2 * H_NMIXED)
    _, a, b = case["args"]
    return "{}({}, {})  [a = {:.6g}, b = {:.6g}]".format(op, a, b, a / 32768, b / 32768)


def h_show(v):
    return v if isinstance(v, str) else h_fmt(v)


run_cases(
    h_run, CASES,
    compare=lambda got, c, args: type(got) is type(c["expected"]) and got == c["expected"],
    fmt_input=h_input,
    show=h_show,
)

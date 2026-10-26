from solution import can_get_signal, can_get_signed, can_set_signal
from nlc import run_cases

# Expected values come from the literals in CASES and, for the sweeps, from
# the slow bit-by-bit model below. Nothing here calls the reference solution.

PAIRS = [(start, length) for length in range(1, 33) for start in range(0, 65 - length)]


def h_bit(frame, k):
    return (frame[k // 8] >> (k % 8)) & 1


def h_get(frame, start, length):
    return sum(h_bit(frame, start + i) << i for i in range(length))


def h_get_signed(frame, start, length):
    value = h_get(frame, start, length)
    return value - (1 << length) if h_bit(frame, start + length - 1) else value


def h_set(frame, start, length, value):
    out = bytearray(frame)
    for i in range(length):
        k = start + i
        if (value >> i) & 1:
            out[k // 8] |= 1 << (k % 8)
        else:
            out[k // 8] &= ~(1 << (k % 8)) & 0xFF
    return bytes(out)


def h_next(state):
    state ^= (state << 13) & 0xFFFFFFFF
    state ^= state >> 17
    state ^= (state << 5) & 0xFFFFFFFF
    return state


def h_fill(state):
    out = bytearray(8)
    for i in range(8):
        state = h_next(state)
        out[i] = state >> 24
    return bytes(out), state


def fmt_frame(frame):
    return "{" + " ".join("%02X" % b for b in frame) + "}"


def fmt_hex(value):
    return "0x%X" % value if isinstance(value, int) else repr(value)


def fmt_dec(value):
    return str(value) if isinstance(value, int) else repr(value)


def run_set(before, start, length, value):
    frame = bytearray(before)
    can_set_signal(frame, start, length, value)
    text = "frame=" + fmt_frame(frame)
    if len(frame) != 8:
        text += "; frame resized to %d bytes (expected 8)" % len(frame)
    return text


def sweep_get(seed):
    frame, _ = h_fill(seed)
    for start, length in PAIRS:
        want = h_get(frame, start, length)
        got = can_get_signal(bytes(frame), start, length)
        if got != want:
            return "first failing pair: can_get_signal(frame=%s, start_bit=%d, length=%d): expected %s, got %s" % (
                fmt_frame(frame), start, length, fmt_hex(want), fmt_hex(got))
    return "all %d pairs match" % len(PAIRS)


def sweep_signed(seed):
    frame, _ = h_fill(seed)
    for start, length in PAIRS:
        want = h_get_signed(frame, start, length)
        got = can_get_signed(bytes(frame), start, length)
        if got != want:
            return "first failing pair: can_get_signed(frame=%s, start_bit=%d, length=%d): expected %s, got %s" % (
                fmt_frame(frame), start, length, fmt_dec(want), fmt_dec(got))
    return "all %d pairs match" % len(PAIRS)


def sweep_set(seed):
    state = seed
    for start, length in PAIRS:
        before, state = h_fill(state)
        state = h_next(state)
        value = state
        want = "frame=" + fmt_frame(h_set(before, start, length, value))
        got = run_set(before, start, length, value)
        if got != want:
            return ("first failing pair: can_set_signal(frame=%s, start_bit=%d, length=%d, value=0x%X): "
                    "expected %s, got %s" % (fmt_frame(before), start, length, value, want, got))
    return "all %d pairs match" % len(PAIRS)


def round_trip(seed):
    state = seed
    for start, length in PAIRS:
        before, state = h_fill(state)
        state = h_next(state)
        value = state
        want = value & ((1 << length) - 1)
        frame = bytearray(before)
        can_set_signal(frame, start, length, value)
        got = can_get_signal(bytes(frame), start, length)
        if got != want:
            return ("first failing pair: can_set_signal(frame=%s, start_bit=%d, length=%d, value=0x%X), "
                    "then can_get_signal(frame, %d, %d): expected %s, got %s" % (
                        fmt_frame(before), start, length, value, start, length, fmt_hex(want), fmt_hex(got)))
    return "all %d pairs match" % len(PAIRS)


def drive(kind, *args):
    if kind == "get":
        frame, start, length = args
        return fmt_hex(can_get_signal(bytes(frame), start, length))
    if kind == "signed":
        frame, start, length = args
        return fmt_dec(can_get_signed(bytes(frame), start, length))
    if kind == "set":
        return run_set(*args)
    if kind == "sweep_get":
        return sweep_get(*args)
    if kind == "sweep_signed":
        return sweep_signed(*args)
    if kind == "sweep_set":
        return sweep_set(*args)
    if kind == "round_trip":
        return round_trip(*args)
    raise ValueError(kind)


def get_case(name, frame, start, length, expected):
    return {
        "name": name,
        "args": ["get", frame, start, length],
        "expected": fmt_hex(expected),
        "input": "can_get_signal(frame=%s, start_bit=%d, length=%d)" % (fmt_frame(frame), start, length),
    }


def signed_case(name, frame, start, length, expected):
    return {
        "name": name,
        "args": ["signed", frame, start, length],
        "expected": fmt_dec(expected),
        "input": "can_get_signed(frame=%s, start_bit=%d, length=%d)" % (fmt_frame(frame), start, length),
    }


def set_case(name, frame, start, length, value, expected):
    return {
        "name": name,
        "args": ["set", frame, start, length, value],
        "expected": "frame=" + fmt_frame(expected),
        "input": "can_set_signal(frame=%s, start_bit=%d, length=%d, value=0x%X)" % (
            fmt_frame(frame), start, length, value),
    }


def sweep_case(name, kind, seed, what):
    return {
        "name": name,
        "args": [kind, seed],
        "expected": "all %d pairs match" % len(PAIRS),
        "input": "%s for all %d (start_bit, length) pairs" % (what, len(PAIRS)),
    }


SEQ = bytes([0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0])
REV = bytes([0x21, 0x43, 0x65, 0x87, 0xA9, 0xCB, 0xED, 0x0F])
AB = bytes([0xAB, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
SPAN = bytes([0xC5, 0x3A, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00])
TOP = bytes([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80])
ONES = bytes([0xFF] * 8)
ZERO = bytes(8)

CASES = [
    get_case("example 1: byte-aligned 8-bit signal", SEQ, 16, 8, 0x56),
    get_case("byte-aligned 16-bit signal, little-endian", SEQ, 8, 16, 0x5634),
    get_case("example 2: 12-bit signal at bit 4", AB, 4, 12, 0xCDA),
    get_case("signal spanning three bytes (start 6, len 13)", SPAN, 6, 13, 0x18EB),
    get_case("full 32-bit signal at bit 0", SEQ, 0, 32, 0x78563412),
    get_case("full 32-bit signal at bit 17 (spans five bytes)", REV, 17, 32, 0xE5D4C3B2),
    get_case("full 32-bit signal in the top half (bit 32)", SEQ, 32, 32, 0xF0DEBC9A),
    get_case("1-bit signal at bit 63", TOP, 63, 1, 1),

    signed_case("example 3: signed 0x800 in 12 bits is -2048",
                bytes([0x0F, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]), 4, 12, -2048),
    signed_case("signed 0x7FF in 12 bits is 2047",
                bytes([0xFF, 0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00]), 4, 12, 2047),
    signed_case("signed all-ones 5-bit signal is -1",
                bytes([0x00, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x00]), 30, 5, -1),
    signed_case("signed 32-bit signal at bit 13 is INT32_MIN",
                bytes([0xFF, 0x1F, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF]), 13, 32, -2147483648),

    set_case("example 4: set 12 bits at bit 4 in an all-0xFF frame", ONES, 4, 12, 0x123,
             bytes([0x3F, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])),
    set_case("set across three bytes, neighbours stay 1 (0xFF frame)", ONES, 6, 13, 0x1234,
             bytes([0x3F, 0x8D, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])),
    set_case("set across three bytes, neighbours stay 0 (zero frame)", ZERO, 6, 13, 0x1234,
             bytes([0x00, 0x8D, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00])),
    set_case("example 5: value wider than len is truncated", ZERO, 10, 4, 0xFFFFFFF5,
             bytes([0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])),
    set_case("set full 32-bit signal at bit 17 (0xFF frame)", ONES, 17, 32, 0x13579BDF,
             bytes([0xFF, 0xFF, 0xBF, 0x37, 0xAF, 0x26, 0xFE, 0xFF])),
    set_case("clear full 32-bit signal at bit 17", SEQ, 17, 32, 0,
             bytes([0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xDE, 0xF0])),
    set_case("clear 1-bit signal at bit 63", ONES, 63, 1, 0,
             bytes([0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F])),

    sweep_case("get: every start_bit/len pair", "sweep_get", 0x2545F491,
               "can_get_signal on a random frame"),
    sweep_case("get signed: every start_bit/len pair", "sweep_signed", 0x9E3779B9,
               "can_get_signed on a random frame"),
    sweep_case("set: every start_bit/len pair, random frame and value", "sweep_set", 0xC0FFEE11,
               "can_set_signal on a random frame with a random 32-bit value"),
    sweep_case("set then get round trip: every start_bit/len pair", "round_trip", 0x1234ABCD,
               "can_set_signal then can_get_signal on a random frame with a random 32-bit value"),
]

run_cases(drive, CASES, show=str)

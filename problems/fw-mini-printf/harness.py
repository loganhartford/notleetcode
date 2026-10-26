from solution import mini_snprintf
from nlc import run_cases

INT_MIN = -2**31
INT_MAX = 2**31 - 1
UINT_MAX = 2**32 - 1


def reference_output(fmt, args):
    """Untruncated output, from Python's own %-formatting.

    For this subset it matches C's snprintf once bytes are decoded and None is
    mapped to "(null)". %u/%x arguments are already unsigned, so Python's
    signed-hex behaviour never comes into play."""
    values = []
    for a in args:
        if a is None:
            values.append("(null)")
        elif isinstance(a, bytes):
            values.append(a.decode("latin-1"))
        else:
            values.append(a)
    return (fmt.decode("latin-1") % tuple(values)).encode("latin-1")


def window_size(full, cap):
    return min(len(full), cap - 1) + 1 if cap else 0


def exercise(cap, fmt, *args):
    buf = bytearray(b"X" * cap)
    returned = mini_snprintf(buf, fmt, *args)
    full = reference_output(fmt, args)
    violations = []
    if len(buf) != cap:
        violations.append(f"buf resized to {len(buf)} bytes (it must stay {cap})")
    return returned, bytes(buf[:window_size(full, cap)]), violations


def case(name, cap, fmt, *args, shown=None):
    full = reference_output(fmt, args)
    n = window_size(full, cap)
    expected_bytes = full[:n - 1] + b"\0" if cap else b""
    note = f"   (untruncated: {full!r})" if cap and len(full) >= cap else ""
    arg_text = shown if shown is not None else ", ".join(repr(a) for a in args)
    call = f"mini_snprintf(buf, {fmt!r}{', ' + arg_text if arg_text else ''})"
    size = f"bytearray of {cap} byte{'' if cap == 1 else 's'}, prefilled with 'X'" if cap else "empty bytearray"
    return {
        "name": name,
        "args": [cap, fmt, *args],
        "expected": (len(full), expected_bytes, [], note),
        "input": f"{call}   [buf: {size}]",
    }


CASES = [
    case("plain text", 32, b"hello, world"),
    case("%d positive", 32, b"%d", 42),
    case("%d negative", 32, b"%d", -42),
    case("%d zero", 32, b"%d", 0),
    case("%d INT_MAX", 32, b"%d", INT_MAX, shown="2**31 - 1"),
    case("%d INT_MIN", 32, b"%d", INT_MIN, shown="-2**31"),
    case("%u", 32, b"%u", 3000000000),
    case("%u zero", 32, b"%u", 0),
    case("%u UINT_MAX", 32, b"%u", UINT_MAX, shown="2**32 - 1"),
    case("%x lowercase", 32, b"%x", 0xDEADBEEF, shown="0xdeadbeef"),
    case("%x zero", 32, b"%x", 0),
    case("%x UINT_MAX", 32, b"%x", UINT_MAX, shown="2**32 - 1"),
    case("%c from bytes", 32, b"[%c]", b"A"),
    case("%c from int", 32, b"[%c]", 66),
    case("%s", 32, b"name=%s", b"sensor"),
    case("%s empty string", 32, b"<%s>", b""),
    case("%s None", 32, b"<%s>", None),
    case("%%", 32, b"100%%"),
    case("%% next to a conversion", 32, b"%d%%", 50),
    case("width pads with spaces", 32, b"[%4d]", 42),
    case("width, negative, spaces", 32, b"[%6d]", -42),
    case("negative exactly fills width", 32, b"[%3d]", -42),
    case("0 flag pads with zeros", 32, b"[%08x]", 0xC0FFEE, shown="0xc0ffee"),
    case("0 flag, negative: sign first", 32, b"%05d", -42),
    case("0 flag, zero value", 32, b"%03d", 0),
    case("value wider than width", 32, b"%2u", 12345),
    case("clock format", 32, b"%02u:%02u:%02u", 7, 5, 30),
    case("several conversions", 64, b"id=%u temp=%d raw=0x%04x tag=%c name=%s %d%%",
         7, -15, 0x2A, b"k", b"imu", 99, shown="7, -15, 0x2a, b'k', b'imu', 99"),
    case("truncate mid conversion", 5, b"value=%d", 12345),
    case("truncate inside padding", 4, b"%8d", 7),
    case("truncate inside %s", 8, b"name=%s", b"sensor"),
    case("exact fit", 12, b"value=%d", 12345),
    case("one byte short of exact fit", 11, b"value=%d", 12345),
    case("cap 1: only the NUL", 1, b"abc%d", 5),
    case("cap 0: buf untouched", 0, b"value=%d", 12345),
]


def format_result(result):
    returned, prefix, violations = result[:3]
    note = result[3] if len(result) > 3 else ""
    text = f"return={returned!r}, buf={prefix!r}{note}"
    return text + ("; " + "; ".join(violations) if violations else "")


run_cases(
    exercise,
    CASES,
    compare=lambda got, c, a: got == c["expected"][:3],
    show=format_result,
)

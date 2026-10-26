from solution import hex_to_bytes, bytes_to_hex
from nlc import run_cases

FILL = 0xCC
HEX_LOWER = "".join("0123456789abcdef"[i >> 4] + "0123456789abcdef"[i & 0xF] for i in range(256)).encode()
HEX_UPPER = HEX_LOWER.upper()
ALL_BYTES = bytes(range(256))


def fmt_chars(data):
    shown = data[:40] if len(data) > 48 else data
    parts = []
    for c in shown:
        if c == 0:
            parts.append("\\0")
        elif c in (0x22, 0x5C):
            parts.append("\\" + chr(c))
        elif 32 <= c <= 126:
            parts.append(chr(c))
        else:
            parts.append(f"\\x{c:02X}")
    text = '"' + "".join(parts)
    return text + (f'..." ({len(data)} bytes)' if len(shown) < len(data) else '"')


def fmt_u8(data):
    shown = data[:12] if len(data) > 16 else data
    text = "{" + ", ".join(f"0x{b:02X}" for b in shown)
    return text + (f", ...}} ({len(data)} bytes)" if len(shown) < len(data) else "}")


def first_difference(got, want, untouched=False):
    for i, (g, w) in enumerate(zip(got, want)):
        if g != w:
            if untouched:
                return [f"out[{i}] was written (0x{w:02X} -> 0x{g:02X}) but out_cap is too small"]
            return [f"first difference at out[{i}]: expected 0x{w:02X}, got 0x{g:02X}"]
    return []


def resized(out, cap):
    if len(out) != cap:
        return [f"out was resized to {len(out)} bytes (out_cap is {cap}); write in place"]
    return []


def exercise(op, payload, cap, want_ret, want_out):
    out = bytearray([FILL] * cap)
    if op == "decode":
        ret = hex_to_bytes(payload, out)
        notes = resized(out, cap)
        if isinstance(ret, int) and ret >= 0:
            region = bytes(out[:min(ret, cap)])
            if ret == want_ret and want_ret > 0:
                notes += first_difference(region, want_out)
        else:
            region = None
        return ret, ("u8", region), notes

    ret = bytes_to_hex(payload, out)
    notes = resized(out, cap)
    region = bytes(out[:len(want_out)])
    notes += first_difference(region, want_out, untouched=want_ret < 0)
    return ret, ("chars", region), notes


def decode(name, hexstr, cap, want=None):
    want_ret = -1 if want is None else len(want)
    return {
        "name": name,
        "args": ["decode", hexstr, cap, want_ret, want],
        "expected": (want_ret, ("u8", want), []),
        "input": f"hex_to_bytes(hexstr=b{fmt_chars(hexstr)}, out) with len(out)={cap}",
    }


def encode(name, data, cap, want=None):
    if want is None:
        want_ret, want_out = -1, bytes([FILL] * cap)
    else:
        want_ret, want_out = len(want), want + b"\0"
    return {
        "name": name,
        "args": ["encode", data, cap, want_ret, want_out],
        "expected": (want_ret, ("chars", want_out), []),
        "input": f"bytes_to_hex(data={fmt_u8(data)}, out) with len(out)={cap}, out prefilled with 0x{FILL:02X}",
    }


CASES = [
    decode("decode: mixed case", b"deadBEEF\0", 4, bytes([0xDE, 0xAD, 0xBE, 0xEF])),
    decode("decode: boundary byte values", b"00ff7f80\0", 4, bytes([0x00, 0xFF, 0x7F, 0x80])),
    decode("decode: exactly fills out_cap", b"0123456789abcdef\0", 8,
           bytes([0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF])),
    decode("decode: upper case, room to spare", b"0123456789ABCDEF\0", 16,
           bytes([0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF])),
    decode("decode: empty string", b"\0", 4, b""),
    decode("decode: empty string, out_cap 0", b"\0", 0, b""),
    decode("decode: stops at the terminator", b"ab\0cd\0", 1, bytes([0xAB])),
    decode("decode: odd length", b"abc\0", 4),
    decode("decode: bad high nibble", b"g0\0", 4),
    decode("decode: bad low nibble", b"0g\0", 4),
    decode("decode: '/' (just below '0')", b"12/4\0", 4),
    decode("decode: ':' (just above '9')", b"123:\0", 4),
    decode("decode: '@' (just below 'A')", b"@123\0", 4),
    decode("decode: 'G' (just above 'F')", b"1G23\0", 4),
    decode("decode: '`' (just below 'a')", b"12`4\0", 4),
    decode("decode: 'g' (just above 'f')", b"123g\0", 4),
    decode("decode: 0x prefix rejected", b"0x1f\0", 4),
    decode("decode: one byte too many for out_cap", b"0123456789abcdef01\0", 8),
    decode("decode: one byte into out_cap 0", b"ff\0", 0),
    decode("decode: all 256 byte values, lower case", HEX_LOWER + b"\0", 256, ALL_BYTES),
    decode("decode: all 256 byte values, upper case", HEX_UPPER + b"\0", 256, ALL_BYTES),
    encode("encode: basic", bytes([0xDE, 0xAD, 0xBE, 0xEF]), 16, b"deadbeef"),
    encode("encode: exact fit (out_cap = 2n + 1)", bytes([0x00, 0xFF, 0x7F, 0x80]), 9, b"00ff7f80"),
    encode("encode: every digit", bytes([0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF]), 32,
           b"0123456789abcdef"),
    encode("encode: out_cap one too small", bytes([0x00, 0xFF, 0x7F, 0x80]), 8),
    encode("encode: n = 0", b"", 1, b""),
    encode("encode: n = 0, out_cap 0", b"", 0),
    encode("encode: all 256 byte values", ALL_BYTES, 513, HEX_LOWER),
]


def show(result):
    ret, (kind, region), notes = result
    text = f"return={ret}"
    if region is not None:
        text += ", out=" + (fmt_u8(region) if kind == "u8" else fmt_chars(region))
    return text + "".join("; " + n for n in notes)


run_cases(exercise, CASES, show=show)

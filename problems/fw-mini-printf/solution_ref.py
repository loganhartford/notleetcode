DIGITS = b"0123456789abcdef"


def number(value: int, base: int, width: int, pad: int) -> bytearray:
    neg = value < 0
    mag = -value if neg else value
    digits = bytearray()
    while True:
        digits.append(DIGITS[mag % base])
        mag //= base
        if mag == 0:
            break
    digits.reverse()

    fill = max(width - len(digits) - neg, 0)
    out = bytearray()
    if pad == ord(" "):
        out += b" " * fill
    if neg:
        out.append(ord("-"))
    if pad == ord("0"):
        out += b"0" * fill
    return out + digits


def mini_snprintf(buf: bytearray, fmt: bytes, *args) -> int:
    out = bytearray()
    arg = iter(args)
    i = 0
    while i < len(fmt):
        c = fmt[i]
        i += 1
        if c != ord("%"):
            out.append(c)
            continue

        pad = ord(" ")
        width = 0
        if i < len(fmt) and fmt[i] == ord("0"):
            pad = ord("0")
            i += 1
        if i < len(fmt) and ord("1") <= fmt[i] <= ord("9"):
            width = fmt[i] - ord("0")
            i += 1
        if i == len(fmt):
            break

        conv = fmt[i]
        i += 1
        if conv == ord("d") or conv == ord("u"):
            out += number(next(arg), 10, width, pad)
        elif conv == ord("x"):
            out += number(next(arg), 16, width, pad)
        elif conv == ord("c"):
            ch = next(arg)
            out.append(ch[0] if isinstance(ch, bytes) else ch & 0xFF)
        elif conv == ord("s"):
            s = next(arg)
            out += b"(null)" if s is None else s
        elif conv == ord("%"):
            out.append(ord("%"))

    if len(buf) > 0:
        n = min(len(out), len(buf) - 1)
        buf[:n] = out[:n]
        buf[n] = 0
    return len(out)

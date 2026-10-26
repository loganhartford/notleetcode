def hex_digit_value(c: int) -> int:
    if ord("0") <= c <= ord("9"):
        return c - ord("0")
    if ord("a") <= c <= ord("f"):
        return c - ord("a") + 10
    if ord("A") <= c <= ord("F"):
        return c - ord("A") + 10
    return -1


def hex_to_bytes(hexstr: bytes, out: bytearray) -> int:
    length = 0
    while hexstr[length] != 0:
        length += 1
    if length % 2 != 0 or length // 2 > len(out):
        return -1
    for i in range(length // 2):
        hi = hex_digit_value(hexstr[2 * i])
        lo = hex_digit_value(hexstr[2 * i + 1])
        if hi < 0 or lo < 0:
            return -1
        out[i] = (hi << 4) | lo
    return length // 2


def bytes_to_hex(data: bytes, out: bytearray) -> int:
    digits = b"0123456789abcdef"
    n = len(data)
    if len(out) < 2 * n + 1:
        return -1
    for i in range(n):
        out[2 * i] = digits[data[i] >> 4]
        out[2 * i + 1] = digits[data[i] & 0x0F]
    out[2 * n] = 0
    return 2 * n

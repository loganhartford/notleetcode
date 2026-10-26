def can_get_signal(frame: bytes, start_bit: int, length: int) -> int:
    raw = int.from_bytes(frame, "little")
    return (raw >> start_bit) & ((1 << length) - 1)


def can_get_signed(frame: bytes, start_bit: int, length: int) -> int:
    value = can_get_signal(frame, start_bit, length)
    if value >> (length - 1):
        value -= 1 << length
    return value


def can_set_signal(frame: bytearray, start_bit: int, length: int, value: int) -> None:
    mask = ((1 << length) - 1) << start_bit
    raw = int.from_bytes(frame, "little")
    raw = (raw & ~mask) | ((value << start_bit) & mask)
    frame[:] = raw.to_bytes(8, "little")

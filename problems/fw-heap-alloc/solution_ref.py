import struct
from typing import Optional

HDR = 8
FREE = 1


class Heap:
    def __init__(self) -> None:
        self.arena = bytearray()


def _read(h: Heap, off: int):
    size, prev = struct.unpack_from("<II", h.arena, off)
    return size & ~FREE, bool(size & FREE), prev


def _write(h: Heap, off: int, size: int, free: bool, prev: int) -> None:
    struct.pack_into("<II", h.arena, off, size | (FREE if free else 0), prev)


def _set_prev(h: Heap, off: int, prev: int) -> None:
    struct.pack_into("<I", h.arena, off + 4, prev)


def heap_init(h: Heap, arena: bytearray) -> None:
    h.arena = arena
    _write(h, 0, len(arena), True, 0)


def heap_alloc(h: Heap, n: int) -> Optional[int]:
    total = len(h.arena)
    if n <= 0 or n > total:
        return None
    need = HDR + ((n + 7) & ~7)
    off = 0
    while off < total:
        size, free, prev = _read(h, off)
        if free and size >= need:
            if size - need >= HDR + 8:
                _write(h, off + need, size - need, True, need)
                if off + size < total:
                    _set_prev(h, off + size, size - need)
                size = need
            _write(h, off, size, False, prev)
            return off + HDR
        off += size
    return None


def heap_free(h: Heap, off: Optional[int]) -> None:
    if off is None:
        return
    total = len(h.arena)
    start = off - HDR
    size, _, prev = _read(h, start)
    end = start + size
    if end < total:
        next_size, next_free, _ = _read(h, end)
        if next_free:
            size += next_size
            end = start + size
    if prev:
        prev_size, prev_free, prev_prev = _read(h, start - prev)
        if prev_free:
            start -= prev
            size += prev_size
            prev = prev_prev
    _write(h, start, size, True, prev)
    if end < total:
        _set_prev(h, end, size)

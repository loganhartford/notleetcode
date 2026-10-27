from typing import Optional


class Heap:
    def __init__(self) -> None:
        self.arena = bytearray()


def heap_init(h: Heap, arena: bytearray) -> None:
    pass


def heap_alloc(h: Heap, n: int) -> Optional[int]:
    return None


def heap_free(h: Heap, off: Optional[int]) -> None:
    pass

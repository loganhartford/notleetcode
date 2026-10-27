BM_IDS = 128
BM_WORDS = BM_IDS // 32
FULL = 0xFFFFFFFF


class Bitmap:
    def __init__(self) -> None:
        self.bits = [0] * BM_WORDS


def _in_use(b: Bitmap, i: int) -> bool:
    return (b.bits[i // 32] >> (i % 32)) & 1 == 1


def bm_init(b: Bitmap) -> None:
    for w in range(BM_WORDS):
        b.bits[w] = 0


def bm_alloc(b: Bitmap) -> int:
    for w in range(BM_WORDS):
        word = b.bits[w]
        if word != FULL:
            low = ~word & (word + 1)
            b.bits[w] = word | low
            return w * 32 + low.bit_length() - 1
    return -1


def bm_alloc_run(b: Bitmap, n: int) -> int:
    if not 1 <= n <= BM_IDS:
        return -1
    length = 0
    for i in range(BM_IDS):
        length = 0 if _in_use(b, i) else length + 1
        if length == n:
            start = i + 1 - n
            for j in range(start, i + 1):
                b.bits[j // 32] |= 1 << (j % 32)
            return start
    return -1


def bm_free(b: Bitmap, id: int) -> bool:
    if not 0 <= id < BM_IDS or not _in_use(b, id):
        return False
    b.bits[id // 32] &= ~(1 << (id % 32))
    return True

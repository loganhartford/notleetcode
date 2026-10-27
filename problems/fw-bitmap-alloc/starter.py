BM_IDS = 128


class Bitmap:
    def __init__(self) -> None:
        self.bits = [0] * (BM_IDS // 32)


def bm_init(b: Bitmap) -> None:
    pass


def bm_alloc(b: Bitmap) -> int:
    return -1


def bm_alloc_run(b: Bitmap, n: int) -> int:
    return -1


def bm_free(b: Bitmap, id: int) -> bool:
    return False

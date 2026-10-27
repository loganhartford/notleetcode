class Ema:
    def __init__(self) -> None:
        self.state = 0
        self.k = 0


def ema_init(f: Ema, k: int, initial: int) -> None:
    f.k = k
    f.state = initial << k


def ema_update(f: Ema, x: int) -> int:
    f.state += x - (f.state >> f.k)
    return f.state >> f.k

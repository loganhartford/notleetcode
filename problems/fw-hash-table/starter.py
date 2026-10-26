from typing import Optional

HT_CAP = 64


class HashTable:
    def __init__(self) -> None:
        self.keys = [0] * HT_CAP
        self.vals = [0] * HT_CAP
        self.state = [0] * HT_CAP
        self.count = 0


def ht_init(t: HashTable) -> None:
    pass


def ht_put(t: HashTable, key: int, val: int) -> bool:
    return False


def ht_get(t: HashTable, key: int) -> Optional[int]:
    return None


def ht_del(t: HashTable, key: int) -> bool:
    return False

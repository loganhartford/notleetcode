from typing import Optional

HT_CAP = 64

SLOT_EMPTY, SLOT_USED, SLOT_DELETED = 0, 1, 2


class HashTable:
    def __init__(self) -> None:
        self.keys = [0] * HT_CAP
        self.vals = [0] * HT_CAP
        self.state = [0] * HT_CAP
        self.count = 0


def _hash(key: int) -> int:
    key ^= key >> 16
    key = (key * 0x45D9F3B) & 0xFFFFFFFF
    key ^= key >> 16
    return key % HT_CAP


def _find(t: HashTable, key: int) -> int:
    i = _hash(key)
    for _ in range(HT_CAP):
        if t.state[i] == SLOT_EMPTY:
            return -1
        if t.state[i] == SLOT_USED and t.keys[i] == key:
            return i
        i = (i + 1) % HT_CAP
    return -1


def ht_init(t: HashTable) -> None:
    for i in range(HT_CAP):
        t.state[i] = SLOT_EMPTY
    t.count = 0


def ht_put(t: HashTable, key: int, val: int) -> bool:
    found = _find(t, key)
    if found >= 0:
        t.vals[found] = val
        return True
    if t.count == HT_CAP:
        return False
    i = _hash(key)
    while t.state[i] == SLOT_USED:
        i = (i + 1) % HT_CAP
    t.keys[i] = key
    t.vals[i] = val
    t.state[i] = SLOT_USED
    t.count += 1
    return True


def ht_get(t: HashTable, key: int) -> Optional[int]:
    i = _find(t, key)
    return None if i < 0 else t.vals[i]


def ht_del(t: HashTable, key: int) -> bool:
    i = _find(t, key)
    if i < 0:
        return False
    t.state[i] = SLOT_DELETED
    t.count -= 1
    return True

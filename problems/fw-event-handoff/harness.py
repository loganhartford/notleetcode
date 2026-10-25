from solution import post_events_from_isr, take_pending_events
from nlc import run_cases


class InterruptModel:
    def __init__(self, pending, mask, early, late):
        self.pending, self.mask = pending, mask
        self.early, self.late = early, late
        self.saves = self.restores = 0

    def save(self):
        old = self.mask
        if not self.mask:
            self.pending[0] |= self.early
        self.early = 0
        self.mask = 1
        self.saves += 1
        return old

    def restore(self, previous):
        self.mask = previous
        self.restores += 1
        if not self.mask:
            self.pending[0] |= self.late
            self.late = 0


def exercise(start, mask, early, late):
    pending = [start]
    irq = InterruptModel(pending, mask, early, late)
    result = take_pending_events(pending, irq)
    return result, pending[0], irq.mask, irq.saves, irq.restores


CASES = []
for name, start, mask, early, late in [
    ("empty", 0, 0, 0, 0), ("basic", 5, 0, 0, 0),
    ("high bit", 0x80000000, 0, 0, 0), ("all bits", 0xFFFFFFFF, 0, 0, 0),
    ("before masking", 1, 0, 4, 0), ("after restore", 1, 0, 0, 8),
    ("same bit again", 1, 0, 0, 1), ("both boundaries", 1, 0, 2, 4),
    ("already masked", 5, 1, 0, 8), ("masked empty", 0, 1, 0, 0),
]:
    CASES.append({"name": name, "args": [start, mask, early, late],
                  "expected": (start | (0 if mask else early), 0 if mask else late, mask, 1, 1)})


def all_checks(mode, *args):
    if mode == "take":
        return exercise(*args)
    p = [0]
    for event in args:
        post_events_from_isr(p, event)
    return p[0]


for case in CASES:
    case["args"].insert(0, "take")
CASES.extend([
    {"name": "OR and coalesce", "args": ["post", 1, 4, 1], "expected": 5},
    {"name": "zero preserves", "args": ["post", 5, 0], "expected": 5},
])
def format_input(case):
    mode, *args = case["args"]
    if mode == "post":
        events = ", ".join(f"0x{x:X}" for x in args)
        return f"initial pending=0x00000000; post_events_from_isr events=[{events}]"
    start, mask, early, late = args
    state = "disabled" if mask else "enabled"
    return (f"take_pending_events: initial pending=0x{start:08X}, interrupts={state}, "
            f"event before mask=0x{early:08X}, event at restore=0x{late:08X}")


def format_result(result):
    if isinstance(result, tuple):
        snapshot, pending, mask, saves, restores = result
        return (f"snapshot=0x{snapshot:08X}, pending=0x{pending:08X}, "
                f"interrupt mask={mask}, save/restore calls={saves}/{restores}")
    return f"pending=0x{result:08X}"


run_cases(all_checks, CASES, fmt_input=format_input, show=format_result)

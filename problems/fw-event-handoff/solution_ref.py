def post_events_from_isr(pending: list, events: int) -> None:
    pending[0] |= events


def take_pending_events(pending: list, irq) -> int:
    previous = irq.save()
    snapshot = pending[0]
    pending[0] = 0
    irq.restore(previous)
    return snapshot

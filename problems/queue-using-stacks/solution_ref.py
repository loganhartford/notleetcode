# Time: O(1) amortized per operation  — lazy transfer between stacks
class MyQueue:
    def __init__(self):
        self.inbox = []   # newest pushes
        self.outbox = []  # reversed, ready to pop in FIFO order

    def push(self, x: int) -> None:
        self.inbox.append(x)

    def _shift(self) -> None:
        if not self.outbox:
            while self.inbox:
                self.outbox.append(self.inbox.pop())

    def pop(self) -> int:
        self._shift()
        return self.outbox.pop()

    def peek(self) -> int:
        self._shift()
        return self.outbox[-1]

    def empty(self) -> bool:
        return not self.inbox and not self.outbox

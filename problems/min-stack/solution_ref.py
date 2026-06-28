# Time: O(1) per operation  — parallel min stack
class MinStack:
    def __init__(self):
        self.stack = []
        self.mins = []  # running minimum, parallel to stack

    def push(self, val: int) -> None:
        self.stack.append(val)
        m = val if not self.mins else min(val, self.mins[-1])
        self.mins.append(m)

    def pop(self) -> None:
        self.stack.pop()
        self.mins.pop()

    def top(self) -> int:
        return self.stack[-1]

    def getMin(self) -> int:
        return self.mins[-1]

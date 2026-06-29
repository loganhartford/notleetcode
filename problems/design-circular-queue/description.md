# Design Circular Queue

Design your implementation of a **circular queue** (ring buffer) of fixed capacity. The circular queue reuses the space freed by dequeues — when the buffer is full and you dequeue, the freed slot can be used again.

Implement these functions (this is the LeetCode C interface):

```c
MyCircularQueue* myCircularQueueCreate(int k);            // capacity k
bool myCircularQueueEnQueue(MyCircularQueue* obj, int value); // false if full
bool myCircularQueueDeQueue(MyCircularQueue* obj);            // false if empty
int  myCircularQueueFront(MyCircularQueue* obj);             // -1 if empty
int  myCircularQueueRear(MyCircularQueue* obj);              // -1 if empty
bool myCircularQueueIsEmpty(MyCircularQueue* obj);
bool myCircularQueueIsFull(MyCircularQueue* obj);
void myCircularQueueFree(MyCircularQueue* obj);
```

You define the `MyCircularQueue` struct yourself (a fixed array plus head index
and count is the classic approach).

### Example
```
MyCircularQueue(3)
enQueue(1)  -> true
enQueue(2)  -> true
enQueue(3)  -> true
enQueue(4)  -> false   (full)
Rear()      -> 3
isFull()    -> true
deQueue()   -> true
enQueue(4)  -> true
Rear()      -> 4
```

### Constraints
- `1 <= k <= 1000`
- `0 <= value <= 1000`
- At most `3000` calls total.

**Optimal complexity:** O(1) per operation

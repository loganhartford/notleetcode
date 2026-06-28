def climbStairs(n: int) -> int:
    a, b = 1, 1  # ways(0)=1, ways(1)=1
    for _ in range(n):
        a, b = b, a + b
    return a

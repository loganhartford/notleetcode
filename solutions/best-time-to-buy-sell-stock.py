from typing import List


def maxProfit(prices: List[int]) -> int:
    buyIndex = 0
    for i, price in enumerate(prices):
        

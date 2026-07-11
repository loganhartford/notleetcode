from typing import List


# def maxProfit(prices: List[int]) -> int:
#     # Brute force method
#     profit = 0
#     for i in range(len(prices)-1):
#         for j in range(i+1, len(prices)):
#             newProfit = prices[j] - prices[i]
#             if newProfit > profit:
#                 profit = newProfit
#             print(prices[j], prices[i], profit, newProfit)
#     return profit
# O(n^2)

def maxProfit(prices: List[int]) -> int:
    # Store the best purchase price you've seen so far
    # Compute the best profit from that
    # Since the purchase price is in the past, an
    best = 0
    buy = prices[0]
    for price in prices[1:]:
        buy = min(buy, price)
        best = max(price - buy, best)
    
    return best
// Time: O(n)
int maxProfit(int* prices, int pricesSize) {
    int lo = prices[0], best = 0;
    for (int i = 1; i < pricesSize; i++) {
        if (prices[i] < lo) lo = prices[i];
        else if (prices[i] - lo > best) best = prices[i] - lo;
    }
    return best;
}

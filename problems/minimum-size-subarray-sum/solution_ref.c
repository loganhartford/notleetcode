// Time: O(n)  — sliding window
int minSubArrayLen(int target, int* nums, int numsSize) {
    int left = 0, total = 0, best = numsSize + 1;
    for (int r = 0; r < numsSize; r++) {
        total += nums[r];
        while (total >= target) {
            int len = r - left + 1;
            if (len < best) best = len;
            total -= nums[left++];
        }
    }
    return best == numsSize + 1 ? 0 : best;
}

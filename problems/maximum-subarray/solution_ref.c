int maxSubArray(int* nums, int numsSize) {
    int best = nums[0], cur = nums[0];
    for (int i = 1; i < numsSize; i++) {
        int withPrev = cur + nums[i];
        cur = nums[i] > withPrev ? nums[i] : withPrev;
        if (cur > best) best = cur;
    }
    return best;
}

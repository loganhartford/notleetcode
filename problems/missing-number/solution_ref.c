// Time: O(n)  — XOR cancellation
int missingNumber(int* nums, int numsSize) {
    int x = numsSize;  // start with n (the index that has no array slot)
    for (int i = 0; i < numsSize; i++) {
        x ^= i ^ nums[i];
    }
    return x;
}

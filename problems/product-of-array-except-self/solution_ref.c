#include <stdlib.h>

int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    int* res = malloc(numsSize * sizeof(int));
    int prefix = 1;
    for (int i = 0; i < numsSize; i++) {
        res[i] = prefix;
        prefix *= nums[i];
    }
    int suffix = 1;
    for (int i = numsSize - 1; i >= 0; i--) {
        res[i] *= suffix;
        suffix *= nums[i];
    }
    *returnSize = numsSize;
    return res;
}

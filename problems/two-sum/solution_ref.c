// Time: O(n²)  — brute force; a hash map gives O(n)
#include <stdlib.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    for (int i = 0; i < numsSize; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                int* r = malloc(2 * sizeof(int));
                r[0] = i;
                r[1] = j;
                *returnSize = 2;
                return r;
            }
        }
    }
    *returnSize = 0;
    return NULL;
}

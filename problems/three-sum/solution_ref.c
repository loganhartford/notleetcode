// Time: O(n²)  — sort then two-pointer scan for each pivot
#include <stdlib.h>

static int _ts_cmp(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    qsort(nums, numsSize, sizeof(int), _ts_cmp);
    int cap = 8, cnt = 0;
    int** res = malloc(cap * sizeof(int*));

    for (int i = 0; i < numsSize - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        if (nums[i] > 0) break;
        int lo = i + 1, hi = numsSize - 1;
        while (lo < hi) {
            int s = nums[i] + nums[lo] + nums[hi];
            if (s < 0) {
                lo++;
            } else if (s > 0) {
                hi--;
            } else {
                if (cnt == cap) { cap *= 2; res = realloc(res, cap * sizeof(int*)); }
                int* t = malloc(3 * sizeof(int));
                t[0] = nums[i];
                t[1] = nums[lo];
                t[2] = nums[hi];
                res[cnt++] = t;
                lo++;
                hi--;
                while (lo < hi && nums[lo] == nums[lo - 1]) lo++;
                while (lo < hi && nums[hi] == nums[hi + 1]) hi--;
            }
        }
    }

    *returnSize = cnt;
    *returnColumnSizes = malloc((cnt > 0 ? cnt : 1) * sizeof(int));
    for (int k = 0; k < cnt; k++) (*returnColumnSizes)[k] = 3;
    return res;
}

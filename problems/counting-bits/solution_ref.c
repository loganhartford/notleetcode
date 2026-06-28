// Time: O(n)  — each ans[i] derived from ans[i>>1] in O(1)
#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    int* ans = malloc((n + 1) * sizeof(int));
    ans[0] = 0;
    for (int i = 1; i <= n; i++) {
        ans[i] = ans[i >> 1] + (i & 1);
    }
    *returnSize = n + 1;
    return ans;
}

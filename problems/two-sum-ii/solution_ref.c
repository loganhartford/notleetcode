#include <stdlib.h>

int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int i = 0, j = numbersSize - 1;
    while (i < j) {
        int s = numbers[i] + numbers[j];
        if (s == target) {
            int* r = malloc(2 * sizeof(int));
            r[0] = i + 1;
            r[1] = j + 1;
            *returnSize = 2;
            return r;
        } else if (s < target) {
            i++;
        } else {
            j--;
        }
    }
    *returnSize = 0;
    return NULL;
}

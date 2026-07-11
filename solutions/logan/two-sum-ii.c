#include <stdlib.h>

/* Return a malloc'd array of the two 1-indexed positions (index1 < index2),
 * and set *returnSize = 2. The caller frees the returned array. */
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    int i = 0;
    int j = numbersSize - 1;
    
    while (i < j) {
        int sum = numbers[i] + numbers[j];
        if (sum == target) {
            *returnSize = 2;
            int *indexes = malloc(sizeof(int)*(*returnSize));
            indexes[0] = i+1;
            indexes[1] = j+1;
            return indexes;
        }
        
        if (sum < target) {
            i++;
        } else {
            j--;
        }
    }
}

// Time: O(n)  — two-pointer single pass
int maxArea(int* height, int heightSize) {
    int i = 0, j = heightSize - 1, best = 0;
    while (i < j) {
        int h = height[i] < height[j] ? height[i] : height[j];
        int area = h * (j - i);
        if (area > best) best = area;
        if (height[i] < height[j]) i++;
        else j--;
    }
    return best;
}

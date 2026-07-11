int maxArea(int* height, int heightSize) {
    int i = 0;
    int j = heightSize -1;
    int bestArea = 0;
    while (i < j) {
        int h = 0;
        int w = 0;
        int *inc = NULL;
        int dir = 0;
        if (height[i] < height[j]) {
            h = height[i];
            inc = &i;
            dir = 1;
        } else {
            h = height[j];
            inc = &j;
            dir = -1;
        }
        
        w = j - i;
        if ((w*h) > bestArea) {
            bestArea = w*h;
        }
        
        *inc = *inc + dir;
        
    }
    
    return bestArea;
}

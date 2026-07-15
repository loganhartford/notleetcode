#include <stdint.h>

int hamming_distance(int a, int b) {
    int diff = a ^ b;
    uint8_t count = 0;
    while(diff) {
        diff &= diff - 1; // Clears lowest bit
        count++;
    }
    return count;
}

/* 
    x     = 0110 1000
    x - 1 = 0110 0111
    x & x - 1 = 0110 000
*/
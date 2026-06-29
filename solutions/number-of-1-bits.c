#include <stdint.h>

int hammingWeight(uint32_t n) {
    
    // Slightly better in best case
    int ones = 0;
    while (n) {
        ones++;
        // 11 = 1011
        // 10 = 1010
        // 1011 & 1010 = 1010
        // 128 = 1000 0000
        // 127 = 0111 1111
        // 128 & 127 = 0
        n &= n - 1; // clears lowest bit set each iteration
    }
    
    // # O(n)
    /*
    int ones = 0;
    for (int i = 0; i < 32; i++){
        if ((n >> i) & 1) {
            ones++;
        }
    }
    return ones;
    */
}

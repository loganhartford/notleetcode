#include <stdint.h>

int hamming_distance(int a, int b) {
    unsigned int diff = (unsigned int)a ^ (unsigned int)b;
    int count = 0;
    while (diff) {
        diff &= diff - 1;
        count++;
    }
    return count;
}

#include <stdint.h>
#include <stddef.h>

#define PATTERN 0xFE6B2840u

int findPattern(const uint32_t numBytes, const uint8_t data[]) {
    if (data == NULL || numBytes < 4)
        return -2;

    uint32_t window = 0;
    uint32_t bitsRead = 0;

    for (uint32_t i = 0; i < numBytes; i++) {
        for (int b = 7; b >= 0; b--) {
            window = (window << 1) | ((data[i] >> b) & 1u);
            bitsRead++;
            if (bitsRead >= 32 && window == PATTERN)
                return (int)(bitsRead - 32);
        }
    }
    return -1;
}

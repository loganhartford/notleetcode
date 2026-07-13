#include <stdint.h>

void write_int(void *ptr, int value) {
    *(int *)ptr = value;
}

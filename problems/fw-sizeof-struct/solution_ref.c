#include <stddef.h>
#include <stdint.h>

struct test_struct {
    char     tag;
    uint32_t id;
    uint16_t flags;
    double   value;
    char     tail;
};

size_t struct_size_without_sizeof(void) {
    struct test_struct arr[2];
    return (size_t)((char *)&arr[1] - (char *)&arr[0]);
}

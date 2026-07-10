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
    /* TODO */
    return 0;
}

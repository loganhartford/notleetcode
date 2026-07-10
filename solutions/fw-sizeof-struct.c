#include <stddef.h>
#include <stdint.h>

struct test_struct {
    char     tag;            // 1
    uint32_t id;             // 4       
    uint16_t flags;          // 2
    double   value;          // 8
    char     tail;           // 32
};

size_t struct_size_without_sizeof(void) {
    struct test_struct t;
    
    return ((size_t)(((char *)((&t) + 1)) - ((char *)(&t))));
}

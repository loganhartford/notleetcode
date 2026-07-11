#include <stddef.h>
#include <stdint.h>

struct test_struct {
    char     tag;            // 1
    uint32_t id;             // 4       
    uint16_t flags;          // 2
    double   value;          // 8
    char     tail;           // 1
};

size_t struct_size_without_sizeof(void) {
    return ((size_t)(((char *)(((struct test_struct *)0) + 1)) - ((char *)((struct test_struct *)0))));
}


/*
 * Follow-up question: why does this return 32 and not the intuitive 16?
 *
 * Short answer: alignment and padding.
 *
 * The compiler inserts padding so each member starts at an address compatible
 * with that member's alignment requirement.
 *
 * Typical layout on a 64-bit ABI:
 *
 *   offset 0:   tag          char      1 byte
 *   offset 1:   padding      3 bytes
 *   offset 4:   id           uint32_t  4 bytes
 *   offset 8:   flags        uint16_t  2 bytes
 *   offset 10:  padding      6 bytes
 *   offset 16:  value        double    8 bytes
 *   offset 24:  tail         char      1 byte
 *   offset 25:  padding      7 bytes
 *
 * Total: 32 bytes.
 *
 * Typical alignment requirements:
 *
 *   char      -> 1-byte aligned
 *   uint16_t  -> 2-byte aligned
 *   uint32_t  -> 4-byte aligned
 *   double    -> 8-byte aligned
 *
 * Because the struct contains a double, the struct itself usually has 8-byte
 * alignment. The trailing padding makes the struct size a multiple of 8, so
 * arrays of this struct keep every element's double member properly aligned.
 */

#include <stddef.h>

typedef enum {
    STACK_UNKNOWN  = 0,
    STACK_GROWS_UP,
    STACK_GROWS_DOWN
} stack_dir_t;

stack_dir_t detect_stack_direction(void) {
    /* hint: take the address of a local here, call a helper, compare */
    return STACK_UNKNOWN;
}

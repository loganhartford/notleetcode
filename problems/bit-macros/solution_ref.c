#include <stdint.h>

#define SET_BIT(reg, n)     ((reg) |= (1u << (n)))
#define CLEAR_BIT(reg, n)   ((reg) &= ~(1u << (n)))
#define TOGGLE_BIT(reg, n)  ((reg) ^= (1u << (n)))
#define TEST_BIT(reg, n)    (((reg) >> (n)) & 1u)

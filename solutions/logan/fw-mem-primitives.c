#include <stddef.h>
#include <stdint.h>

void *c_memcpy(void *dst, const void *src, size_t n) {
    /* My solution
    for (int i = 0; i < n; i++) {
        *(uint8_t *)dst = *(uint8_t *)src;
        dst = dst + sizeof(uint8_t);
        src = src + sizeof(uint8_t);
    }
    return dst;
    */
    
    /* example solution */
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (n--) *d++ = *s++;
    return dst;
    
    /* Things learned
    - const here applies to the uint8, not to the pointer,
      uint8_t * const s would make the pointer constant, but
      the value mutable.
    - while (n--) is slick
    - *d++ = *s++ assigns the current *s to *d, then increments d and s,
      because post-increment yields the old pointer value (used for the dereference) 
      while deferring the actual increment as a side effect. Precedence only matters 
      for grouping: ++ binds tighter than *, so it's *(d++), meaning the ++ applies to 
      the pointer, not the dereferenced value.
    */
}

/* REDO THIS ONE */
void *c_memmove(void *dst, const void *src, size_t n) { 
    uint8_t * d = (uint8_t *)dst;
    const uint8_t * s = (uint8_t *)src;
    
    if (d < s) {
        while (n--) *d++ = *s++;
    }
    else if (d > s) {
        d += n; s += n;
        while (n--) *--d = *--s;
        
        /* equivalent
        d += n-1; s += n-1;
        while (n--) *d-- = *s--;
        */
    }
    
    /* Things learned
    - *--d = *--s; decrements before the assigment, which we want here
      because + n moves the pointer one to far so we decrement first.
    */

    return dst;
}

void *c_memset(void *dst, int value, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    uint8_t lower = (value & 0xFF);
    while (n--) *d++ = lower;
    return dst;
}

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void *aligned_malloc(size_t size, size_t align) {
    // Return NULL if align is 0
    if (align == 0) return NULL;
    
    // Allocate, the size of the pointer to the start of memory
    // plus the size of the data plus the size of the align.
    void * raw = malloc(sizeof(void *) + size + align);
    if (!raw) return NULL;
    
    // Find the first aligned address that leave room for the pointer
    // Make room for the pointer
    uintptr_t base = (uintptr_t)raw + sizeof(void *);
    
    // Side note uintptr_t is an unsigned integer large enough to hold a pointer
    // Use uintptr_t when you want to do operations on an address.
    // C does not define arithmetic on void *.
    
    /*
     * Align is a power of two, so we know it will be of the form 0x00100000.
     * Any address that is not aligned to this, will have non-zero lower bits.
     * Like 0x00010010. If we take align - 1, we create a mask of the lower bits: 0b00011111.
     * Then we ! the bits 0b11100000. Then we add the lower of the alignment bits to  the base
     * to gaurentee we've crossed and alignment boundary the min times, and then we set the lower bits to 0
     * to find the lowest aligned address
     * Example:
     * align = 8 = 0b00001000 - 1 = 0b00000111 ~= 0b11111000 = mask
     * base = 3  = 0b00000011 + align - 1 = 0b00001010 & mask = 0b00001000 -> 8
     */

    uintptr_t aligned =  (base + align - 1) & (uintptr_t)(~(align -1));
    
    // Store the orignal malloc address right before teh aligned address
    ((void **)aligned)[-1] = raw;
    return (void *)aligned;
}

void aligned_free(void *ptr) {
    if (!ptr) return;
    free(((void **)ptr)[-1]);
    return;
}

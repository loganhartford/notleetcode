#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *base;
    size_t   size;
} heap_t;

typedef struct {
    uint32_t size;
    uint32_t prev;
} hdr_t;

#define HDR  sizeof(hdr_t)
#define FREE 1u

static hdr_t *at(heap_t *h, size_t off) { return (hdr_t *)(h->base + off); }
static size_t blk_size(const hdr_t *b) { return b->size & ~FREE; }
static int is_free(const hdr_t *b) { return (b->size & FREE) != 0; }

void heap_init(heap_t *h, void *arena, size_t size) {
    h->base = arena;
    h->size = size;
    hdr_t *b = at(h, 0);
    b->size = (uint32_t)size | FREE;
    b->prev = 0;
}

void *heap_alloc(heap_t *h, size_t n) {
    if (n == 0 || n > h->size) return NULL;
    size_t need = HDR + ((n + 7) & ~(size_t)7);
    for (size_t off = 0; off < h->size; off += blk_size(at(h, off))) {
        hdr_t *b = at(h, off);
        size_t sz = blk_size(b);
        if (!is_free(b) || sz < need) continue;
        if (sz - need >= HDR + 8) {
            hdr_t *rest = at(h, off + need);
            rest->size = (uint32_t)(sz - need) | FREE;
            rest->prev = (uint32_t)need;
            if (off + sz < h->size) at(h, off + sz)->prev = (uint32_t)(sz - need);
            sz = need;
        }
        b->size = (uint32_t)sz;
        return (uint8_t *)b + HDR;
    }
    return NULL;
}

void heap_free(heap_t *h, void *p) {
    if (!p) return;
    size_t off = (size_t)((uint8_t *)p - h->base) - HDR;
    hdr_t *b = at(h, off);
    size_t sz = blk_size(b);
    size_t end = off + sz;
    if (end < h->size && is_free(at(h, end))) {
        sz += blk_size(at(h, end));
        end = off + sz;
    }
    if (b->prev && is_free(at(h, off - b->prev))) {
        off -= b->prev;
        sz += blk_size(at(h, off));
        b = at(h, off);
    }
    b->size = (uint32_t)sz | FREE;
    if (end < h->size) at(h, end)->prev = (uint32_t)sz;
}

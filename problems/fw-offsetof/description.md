# Implement offsetof and container_of

`offsetof(type, member)` from `<stddef.h>` gives the byte offset of a member from the start of its struct. The Linux kernel builds `container_of` on top of it: given a pointer to a member embedded in a struct, it returns a pointer to the enclosing struct. Intrusive linked lists depend on this, because the list node lives inside each object and the code walking the list has to get from the node back to the object.

Implement both as macros in your solution, without using the standard `offsetof`:

```c
#define MY_OFFSETOF(type, member)           /* size_t: byte offset of member within type */
#define MY_CONTAINER_OF(ptr, type, member)  /* type *: the struct whose member ptr points to */
```

Be ready to explain why the offsets in Example 1 are not the running sum of the member sizes, and why the same struct can have different offsets on another compiler or target.

### Example 1
```c
struct sample {
    char     tag;
    uint32_t id;
    uint16_t flags;
    double   value;
};

MY_OFFSETOF(struct sample, tag)    -> 0
MY_OFFSETOF(struct sample, id)     -> 4
MY_OFFSETOF(struct sample, flags)  -> 8
MY_OFFSETOF(struct sample, value)  -> 16   (x86-64 / AArch64)
```

### Example 2
```c
struct node { struct node *next; };

struct task {
    uint8_t     priority;
    uint32_t    id;
    struct node link;
};

struct task t;
struct node *n = &t.link;

MY_CONTAINER_OF(n, struct task, link)  -> &t
```

### Constraints
- `MY_OFFSETOF` evaluates to a `size_t` equal to `offsetof(type, member)` for every member.
- `MY_CONTAINER_OF` evaluates to a `type *` and works for any member, not only the first.
- `ptr` may be any expression that yields a pointer to the member.
- Do not use `offsetof` or `__builtin_offsetof`, directly or through another macro.

*Source: reported in a [Juniper Networks interview](https://www.glassdoor.co.uk/Interview/implement-offsetof-in-C-QTN_272318.htm) (Glassdoor); see Nigel Jones, ["Learn a new trick with the offsetof() macro"](https://www.embedded.com/?p=4024941) (Embedded.com) and the Linux kernel's [`container_of`](https://github.com/torvalds/linux/blob/master/include/linux/container_of.h).*

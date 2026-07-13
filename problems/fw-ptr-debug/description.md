# Debug Pointer Expressions

Given:

```c
void *p = some_address;
void *other_address = ...;
```

Classify each expression and, where invalid, state why:

| Expression | Valid? | Why |
|-----------|--------|-----|
| `*p = 10;` | ❌ | Cannot dereference `void *` — compiler does not know the type or size |
| `*(int *)p = 10;` | ✅ | Cast tells compiler "treat as int*", then deref-assign |
| `*(void **)p = other_address;` | ✅ | Cast to `void **`, then write a pointer through it |
| `(void **)p = other_address;` | ❌ | Assigning to a cast expression — casts are not lvalues |
| `p + 1;` | ❌ | Pointer arithmetic on `void *` is undefined in C (no element size) |
| `(uint8_t *)p + 1;` | ✅ | Cast to byte pointer first, then advance by one byte |

---

Now implement the five functions below to demonstrate each valid pattern:

```c
/* Cast and write an int through a void* */
void  ptr_write_int(void *p, int v);

/* Cast and read an int through a const void* */
int   ptr_read_int(const void *p);

/* Store a void* value into a void* storage location */
void  ptr_store_ptr(void *storage, void *val);

/* Load a void* value from a storage location */
void *ptr_load_ptr(const void *storage);

/* Return a pointer one byte past p */
void *ptr_next_byte(void *p);
```

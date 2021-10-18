# Standard Test Host Library

[Main Imdex](../README.md)

## mem.s
Provides functions for memory allocation, copying, filling etc.

### mem_init
```
    ; void mem_init()

    bsr mem_init
```
Initialises the host memory subsystem. Should be called from _main_ before any other memory functions.

- Since v1.0.0
___

### mem_done
```
    ; void mem_done()

    bsr mem_done
```
Finalises the host memory subsytem. Should be called from _exit_ after all other memory functions.

- Since v1.0.0
___
### mem_alloc
```
    ; r8|a0:void* address, r0|d0:uint64 error mem_alloc(r0|d0:uint64 size)

    move.q  size, d0
    bsr     mem_alloc
    biz.q   a0, .no_memory
```
Attempts to allocate _size_ bytes of memory as indicated by the value in r0|d0.

- On success, the address of the newly allocated block is returned in r8|a0 and #ERR_NONE is returned in r0|d0
- If the requested size is zero, zero is returned in r8|a0 and #ERR_MEM is returned in r0|d0
- If the requested size could not be allocated, zero is returned in r8|a0 and #ERR_NO_MEM returned in r0|d0.
- Since v1.0.0
___
### mem_free
```
    ; mem_free(r8|a0:void* address)

    move.q  address, a0
    bsr     mem_free
```
Attempts to free the memory referenced by the address in r8|a0.

- Attempting to free address zero is safe.
- There is no protection against double freeing a non-zero address.
- The value in r8|a0 is explicitly set to zero after this call.
- Since v1.0.0
___
### mem_copy
```
    ; r0|d0:uint64 result mem_copy(r8|a0:void* from, r9|a1:void* to, r0|d0:uint64 size)

    move.q  size, d0
    move.q  from, a0
    move.q  to, a1
    bsr     mem_copy
```
Attempts to copy the block of memory of length _size_ bytes held in r0|d0 and pointed to by the address in r8|a0 to the region beginning at the address in r9|a1.

- On success, #ERR_NONE is returned in r0|d0.
- If either _from_ or _to_ are zero, #ERR_NULL_PTR is returned in r0|d0.
- If _size_ is zero, #ERR_BAD_SIZE is returned.
- Copy is equivalent to a byte wise copy. No alignment checks are required or applied.
- Overlapping regions are not supported.
- Since v1.0.0

___
### mem_fill_byte
```
    ; r0|d0:uint64 result mem_fill_byte(r8|a0:void* address, r0|d0:uint8 value, r1|d1:uint64 count)

    move.b  value, d0
    move.q  count, d1
    move.q  address, a0
    bsr     mem_fill_byte
```
Attempts to fill the memory block of _count_ bytes in length, held in r1|d1 and pointed to by the address in r8|a0 with the byte value stored in r0|d0.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- On success, #ERR_NONE is returned in r0|d0.
- If the input address is zero, #ERR_NULL_PTR is returned in r0|d0.
- If the count value is zero, #ERR_BAD_SIZE is returned in r0|d0.
- Since v1.0.0
___
### mem_fill_word
```
    ; r0|d0:uint64 result mem_fill_word(r8|a0:void* address, r0|d0:uint16 value, r1|d1:uint64 count)

    move.w  value, d0
    move.q  count, d1
    move.q  address, a0
    bsr     mem_fill_word
```
Attempts to fill the memory block of up to _count_ words in length, held in r1|d1 and pointed to by the address in r8|a0 with the word value stored in r0|d0.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- On success, #ERR_NONE is returned in r0|d0.
- If the input address is zero, #ERR_NULL_PTR is returned in r0|d0.
- If the count value is zero, #ERR_BAD_SIZE is returned in r0|d0.
- The input address is rounded to the nearest 16-bit aligned address. For addresses not on the correct boundary, this results in one less than the requested count being filled.
- Since v1.0.0
___
### mem_fill_long
```
    ; r0|d0:uint64 result mem_fill_long(r8|a0:void* address, r0|d0:uint32 value, r1|d1:uint64 count)

    move.l  value, d0
    move.q  count, d1
    move.q  address, a0
    bsr     mem_fill_long
```
Attempts to fill the memory block of up to _count_ longs in length, held in r1|d1 and pointed to by the address in r8|a0 with the long value stored in r0|d0.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- On success, #ERR_NONE is returned in r0|d0.
- If the input address is zero, #ERR_NULL_PTR is returned in r0|d0.
- If the count value is zero, #ERR_BAD_SIZE is returned in r0|d0.
- The input address is rounded to the nearest 32-bit aligned address. For addresses not on the correct boundary, this results in one less than the requested count being filled.
- Since v1.0.0
___
### mem_fill_quad
```
    ; r0|d0:uint64 result mem_fill_quad(r8|a0:void* address, r0|d0:uint64 value, r1|d1:uint64 count)

    move.q  value, d0
    move.q  count, d1
    move.q  address, a0
    bsr     mem_fill_quad
```
Attempts to fill the memory block of up to _count_ quads in length, held in r1|d1 and pointed to by the address in r8|a0 with the quad value stored in r0|d0.

- The full 64-bit width of r0|d0 is used.
- On success, #ERR_NONE is returned in r0|d0.
- If the input address is zero, #ERR_NULL_PTR is returned in r0|d0.
- If the count value is zero, #ERR_BAD_SIZE is returned in r0|d0.
- The input address is rounded to the nearest 64-bit aligned address. For addresses not on the correct boundary, this results in one less than the requested count being filled.
- Since v1.0.0

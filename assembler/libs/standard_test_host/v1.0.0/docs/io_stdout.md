[Standard Test Host Library](../README.md) > [IO Functions](/io.md)

# Terminal output

Basic terminal output functions.

### io_print_string
r0|d0:uint64 result io_print_string(r8|a0:char const\* string)
```asm
    lea message, a0
    hcf io_print_string
```
Writes the string pointed to by r8|a0 to standard output. No newline is added.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 is zero, #ERR_NULL_PTR is returned un r0|d0.
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_byte
r0|d0:uint64 result io_print_byte(r0|d0:uint8 value, r8|a0:char const\* format)
```asm
    move.b  value, d0
    hcf     io_print_byte
```
Writes the byte integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for bytes is used instead.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_word
r0|d0:uint64 result io_print_word(r0|d0:uint16 word, r8|a0:char const\* format)
```asm
    move.w  value, d0
    hcf     io_print_word
```
Writes the word integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for words is used instead.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_long
r0|d0:uint64 result io_print_long(r0|d0:uint32 long, r8|a0:char const\* format)
```asm
    move.l  value, d0
    hcf     io_print_long
```
Writes the long integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for longs is used instead.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_quad
r0|d0:uint64 result io_print_quad(r0|d0:uint64 quad, r8|a0:char const\* format)
```asm
    move.q  value, d0
    hcf     io_print_quad
```
Writes the quad integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for quads is used instead.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_single
r0|d0:uint64 result io_print_single(fp0:float32 single, r8|a0:char const\* format)
```asm
    fmove.s value, fp0
    hcf     io_print_single
```
Writes the single precision float in fp0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for singles is used instead.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_double
r0|d0:uint64 result io_print_double(fp0:float64 double, r8|a0:char const\* format)
```asm
    fmove.d value, fp0
    hcf     io_print_double
```
Writes the double precision float in fp0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for doubles is used instead.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

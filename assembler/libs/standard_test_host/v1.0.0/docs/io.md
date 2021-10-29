
# Standard Test Host Library

[Main Index](../README.md)

## io.s
Provides functions for basic stream IO.

### io_init
```asm
    ; void io_init()

    bsr io_init
```
Initialises the host stream IO subsystem. Should be called from _main_ before any other stream IO functions.

- Since v1.0.0
___

### io_done
```asm
    ; void io_done()

    bsr io_done
```
Finalises the host stream IO subsytem. Should be called from _exit_ after all other stream IO functions.

- Since v1.0.0
___

### io_print_string
```asm
    ; r0|d0:uint64 result io_print_string(r8|a0:char const* string)

    lea message, a0
    bsr io_print_string
```
Writes the string pointed to by r8|a0 to standard output. No newline is added.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 is zero, #ERR_NULL_PTR is returned un r0|d0.
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_byte
```asm
    ; r0|d0:uint64 result io_print_byte(r0|d0:uint8 value, r8|a0:char const* format)

    move.b  value, d0
    bsr     io_print_byte
```
Writes the byte integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for bytes is used instead.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_word
```asm
    ; r0|d0:uint64 result io_print_word(r0|d0:uint16 word, r8|a0:char const* format)

    move.w  value, d0
    bsr     io_print_word
```
Writes the word integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for words is used instead.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_long
```asm
    ; r0|d0:uint64 result io_print_long(r0|d0:uint32 long, r8|a0:char const* format)

    move.l  value, d0
    bsr     io_print_long
```
Writes the long integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for longs is used instead.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_quad
```asm
    ; r0|d0:uint64 result io_print_quad(r0|d0:uint64 quad, r8|a0:char const* format)

    move.q  value, d0
    bsr     io_print_quad
```
Writes the quad integer in r0|d0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for quads is used instead.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_single
```asm
    ; r0|d0:uint64 result io_print_single(fp0:float32 single, r8|a0:char const* format)

    fmove.s value, fp0
    bsr     io_print_single
```
Writes the single precision float in fp0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for singles is used instead.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_double
```asm
    ; r0|d0:uint64 result io_print_double(fp0:float64 double, r8|a0:char const* format)

    fmove.d value, fp0
    bsr     io_print_double
```
Writes the double precision float in fp0 to standard output, applying the supplied format pointer in r8|a0. If the supplied format is null, the current global formatting template for doubles is used instead.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_set_fmt_byte
```asm
    ; void io_set_fmt_byte(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_byte
```
Sets the default output formatting template for byte integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_byte or io_file_print_byte.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_word
```asm
    ; void io_set_fmt_word(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_word
```
Sets the default output formatting template for word integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_word or io_file_print_word.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_long
```asm
    ; void io_set_fmt_long(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_long
```
Sets the default output formatting template for long integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_long or io_file_print_long.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_quad
```asm
    ; void io_set_fmt_quad(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_quad
```
Sets the default output formatting template for quad integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_quad or io_file_print_quad.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_single
```asm
    ; void io_set_fmt_single(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_single
```
Sets the default output formatting template for single precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_single or io_file_print_single.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_double
```asm
    ; void io_set_fmt_double(r8|a0:char const* format)

    lea format, a0
    bsr io_set_fmt_double
```
Sets the default output formatting template for double precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_double or io_file_print_double.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_file_open
```asm
    ; r8|a0:FILE* stream, r0|d0:uint64 error io_file_open(r8|a0:char const* name, r0|d0:uint8 mode)

    bsr io_file_open
```
Description.

- Since v1.0.0
___
### io_file_seek
```asm
    ; io_file_seek()

    bsr io_file_seek
```
Description.

- Since v1.0.0
___
### io_file_tell
```asm
    ; io_file_tell()

    bsr io_file_tell
```
Description.

- Since v1.0.0
___
### io_file_read
```asm
    ; io_file_read()

    bsr io_file_read
```
Description.

- Since v1.0.0
___
### io_file_write
```asm
    ; io_file_write()

    bsr io_file_write
```
Description.

- Since v1.0.0
___
### io_file_close
```asm
    ; io_file_close()

    bsr io_file_close
```
Description.

- Since v1.0.0
___
### io_file_print_string
```asm
    ; r0|d0:uint64 result io_file_print_string(r8|a0:FILE* stream, r9|a1:char const* string)

    move.q  stream, a0
    lea     message, a1
    bsr     io_file_print_string
```
Writes the string pointed to by r9|a1 to the open stream pointed to by r8|a0. No newline is added.

- When successful, #ERR_NONE is returned in r0|d0.
- If either r8|a0 or r9|a1 are zero, #ERR_NULL_PTR is returned un r0|d0.
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_byte
```asm
    ; r0|d0:uint64 result io_file_print_byte(r8|a0:FILE* stream, r0|d0:uint8 value, r9|a1:char const* format)

    move.q  stream, a0
    move.b  value, d0
    bsr     io_file_print_byte
```
Writes the byte integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for bytes.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_word
```asm
    ; r0|d0:uint64 result io_file_print_word(r8|a0:FILE* stream, r0|d0:uint16 value, r9|a1:char const* format)

    move.q  stream, a0
    move.w  value, d0
    bsr     io_file_print_word
```
Writes the word integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for words.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_long
```asm
    ; r0|d0:uint64 result io_file_print_long(r8|a0:FILE* stream, r0|d0:uint32 value, r9|a1:char const* format)

    move.q  stream, a0
    move.l  value, d0
    bsr     io_file_print_long
```
Writes the long integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for longs.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_quad
```asm
    ; r0|d0:uint64 result io_file_print_quad(r8|a0:FILE* stream, r0|d0:uint64 value, r9|a1:char const* format)

    move.q  stream, a0
    move.q  value, d0
    bsr     io_file_print_quad
```
Writes the quad integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for longs.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_single
```asm
    ; r0|d0:uint64 result io_file_print_single(r8|a0:FILE* stream, fp0:float32 value, r9|a1:char const* format)

    move.q  stream, a0
    fmove.s value, fp0
    bsr     io_file_print_single
```
Writes the single precision floating point value in fp0 to the stream output pointed to by r8|a0, applying the global formatting template for singles.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_double
```asm
    ; r0|d0:uint64 result io_file_print_double(r8|a0:FILE* stream, fp0:float64 value, r9|a1:char const* format)

    move.q  stream, a0
    fmove.d value, fp0
    bsr     io_file_print_double
```
Writes the double precision floating point value in fp0 to the stream output pointed to by r8|a0, applying the global formatting template for doubles.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___


# Standard Test Host Library

[Main Imdex](../README.md)

## io.s
Provides functions for basic stream IO.

### io_init
```
    ; void io_init()

    bsr io_init
```
Initialises the host stream IO subsystem. Should be called from _main_ before any other stream IO functions.

- Since v1.0.0
___

### io_done
```
    ; void io_done()

    bsr io_done
```
Finalises the host stream IO subsytem. Should be called from _exit_ after all other stream IO functions.

- Since v1.0.0
___

### io_print_string
```
    ; r0|d0:uint64 result io_print_string(r8|a0:const char* string)

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
```
    ; r0|d0:uint64 result io_print_byte(r0|d0:uint8 value)

    move.b  value, d0
    bsr     io_print_byte
```
Writes the byte integer in r0|d0 to standard output, applying the global formatting template for bytes.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_word
```
    ; r0|d0:uint64 result io_print_word(r0|d0:uint16 word)

    move.w  value, d0
    bsr     io_print_word
```
Writes the word integer in r0|d0 to standard output, applying the global formatting template for words.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_long
```
    ; r0|d0:uint64 result io_print_long(r0|d0:uint32 long)

    move.l  value, d0
    bsr     io_print_long
```
Writes the long integer in r0|d0 to standard output, applying the global formatting template for longs.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_quad
```
    ; r0|d0:uint64 result io_print_quad(r0|d0:uint64 quad)

    move.q  value, d0
    bsr     io_print_quad
```
Writes the quad integer in r0|d0 to standard output, applying the global formatting template for quads.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_single
```
    ; r0|d0:uint64 result io_print_single(fp0:float32 single)

    fmove.s value, fp0
    bsr     io_print_single
```
Writes the single precision float in fp0 to standard output, applying the global formatting template for singles.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_print_double
```
    ; r0|d0:uint64 result io_print_double(fp0:float64 double)

    fmove.d value, fp0
    bsr     io_print_double
```
Writes the double precision float in fp0 to standard output, applying the global formatting template for doubles.

- When successful, #ERR_NONE is returned in r0|d0.
- If any issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_set_fmt_byte
```
    ; void io_set_fmt_byte(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_byte
```
Sets the default output formatting template for byte integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_byte or io_file_print_byte.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_word
```
    ; void io_set_fmt_word(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_word
```
Sets the default output formatting template for word integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_word or io_file_print_word.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_long
```
    ; void io_set_fmt_long(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_long
```
Sets the default output formatting template for long integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_long or io_file_print_long.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_quad
```
    ; void io_set_fmt_quad(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_quad
```
Sets the default output formatting template for quad integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_quad or io_file_print_quad.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_single
```
    ; void io_set_fmt_single(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_single
```
Sets the default output formatting template for single precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_single or io_file_print_single.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_double
```
    ; void io_set_fmt_double(r8|a0:const char* format)

    lea format, a0
    bsr io_set_fmt_double
```
Sets the default output formatting template for double precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_double or io_file_print_double.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_file_open
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_seek
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_tell
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_read
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_write
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_close
```
    ; io_file_xxx

    bsr io_file_xxx
```
Description.

- Since v1.0.0
___
### io_file_print_string
```
    ; r0|d0:uint64 result io_file_print_string(r8|a0:FILE* stream, r9|a1:const char* string)

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
```
    ; r0|d0:uint64 result io_file_print_byte(r8|a0:FILE* stream, r0|d0:uint8 value)

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
```
    ; r0|d0:uint64 result io_file_print_word(r8|a0:FILE* stream, r0|d0:uint16 value)

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
```
    ; r0|d0:uint64 result io_file_print_long(r8|a0:FILE* stream, r0|d0:uint32 value)

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
```
    ; r0|d0:uint64 result io_file_print_quad(r8|a0:FILE* stream, r0|d0:uint64 value)

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
```
    ; r0|d0:uint64 result io_file_print_single(r8|a0:FILE* stream, fp0:float32 value)

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
```
    ; r0|d0:uint64 result io_file_print_double(r8|a0:FILE* stream, fp0:float64 value)

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

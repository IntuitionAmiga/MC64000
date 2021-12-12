[Standard Test Host Library](../README.md) > [IO Functions](./io.md)

# File Input / Output

Library of routines for opening, closing, reading and writing file streams.

### io_file_open
r8|a0:FILE\* stream, r0|d0:uint64 error io_file_open(r8|a0:char const\* name, r0|d0:uint8 mode)
```asm
    hcf io_file_open
```
Description.

- Since v1.0.0
___
### io_file_seek
io_file_seek()

```asm
    hcf io_file_seek
```
Description.

- Since v1.0.0
___
### io_file_tell
io_file_tell()
```asm
    hcf io_file_tell
```
Description.

- Since v1.0.0
___
### io_file_read
io_file_read()
```asm
    hcf io_file_read
```
Description.

- Since v1.0.0
___
### io_file_write
io_file_write()
```asm
    hcf io_file_write
```
Description.

- Since v1.0.0
___
### io_file_close
io_file_close()
```asm
    hcf io_file_close
```
Description.

- Since v1.0.0
___
### io_file_print_string
r0|d0:uint64 result io_file_print_string(r8|a0:FILE\* stream, r9|a1:char const\* string)
```asm
    move.q  stream, a0
    lea     message, a1
    hcf     io_file_print_string
```
Writes the string pointed to by r9|a1 to the open stream pointed to by r8|a0. No newline is added.

- When successful, #ERR_NONE is returned in r0|d0.
- If either r8|a0 or r9|a1 are zero, #ERR_NULL_PTR is returned un r0|d0.
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_byte
r0|d0:uint64 result io_file_print_byte(r8|a0:FILE\* stream, r0|d0:uint8 value, r9|a1:char const\* format)
```asm

    move.q  stream, a0
    move.b  value, d0
    hcf     io_file_print_byte
```
Writes the byte integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for bytes.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_word
r0|d0:uint64 result io_file_print_word(r8|a0:FILE\* stream, r0|d0:uint16 value, r9|a1:char const\* format)
```asm
    move.q  stream, a0
    move.w  value, d0
    hcf     io_file_print_word
```
Writes the word integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for words.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_long
r0|d0:uint64 result io_file_print_long(r8|a0:FILE\* stream, r0|d0:uint32 value, r9|a1:char const\* format)
```asm
    move.q  stream, a0
    move.l  value, d0
    hcf     io_file_print_long
```
Writes the long integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for longs.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_quad
r0|d0:uint64 result io_file_print_quad(r8|a0:FILE\* stream, r0|d0:uint64 value, r9|a1:char const\* format)
```asm
    move.q  stream, a0
    move.q  value, d0
    hcf     io_file_print_quad
```
Writes the quad integer in r0|d0 to the stream output pointed to by r8|a0, applying the global formatting template for longs.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_single
r0|d0:uint64 result io_file_print_single(r8|a0:FILE\* stream, fp0:float32 value, r9|a1:char const\* format)
```asm
    move.q  stream, a0
    fmove.s value, fp0
    hcf     io_file_print_single
```
Writes the single precision floating point value in fp0 to the stream output pointed to by r8|a0, applying the global formatting template for singles.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___
### io_file_print_double
r0|d0:uint64 result io_file_print_double(r8|a0:FILE\* stream, fp0:float64 value, r9|a1:char const\* format)
```asm
    move.q  stream, a0
    fmove.d value, fp0
    hcf     io_file_print_double
```
Writes the double precision floating point value in fp0 to the stream output pointed to by r8|a0, applying the global formatting template for doubles.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_file_parse_byte
r0|d0:int8 value, r1|d1 uint64 result io_file_parse_byte(r8|a0:FILE\* stream, r9|a1:char char const\* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_file_parse_byte
    bnz     d1, .error
    move.b  d0, value
```
Reads the stream input pointed to by r8|a0 and attempts to parse a byte value using either the supplied format string in r9|a1 or the current global formatting template for byte integers.

- When successful, #ERR_NONE is returned in r1|d1 and the byte value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_file_parse_word
r0|d0:int16 value, r1|d1 uint64 result io_file_parse_word(r8|a0:FILE\* stream, r9|a1:char char const\* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_file_parse_word
    bnz     d1, .error
    move.w  d0, value
```
Reads the stream input pointed to by r8|a0 and attempts to parse a byte value using either the supplied format string in r9|a1 or the current global formatting template for word integers.

- When successful, #ERR_NONE is returned in r1|d1 and the word value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_file_parse_long
r0|d0:int32 value, r1|d1 uint64 result io_file_parse_long(r8|a0:FILE\* stream, r9|a1:char char const\* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_file_parse_long
    bnz     d1, .error
    move.l  d0, value
```
Reads the stream input pointed to by r8|a0 and attempts to parse a long value using either the supplied format string in r9|a1 or the current global formatting template for long integers.

- When successful, #ERR_NONE is returned in r1|d1 and the long value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_file_parse_quad
r0|d0:int64 value, r1|d1 uint64 result io_file_parse_quad(r8|a0:FILE\* stream, r9|a1:char char const\* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_file_parse_quad
    bnz     d1, .error
    move.q  d0, value
```
Reads the stream input pointed to by r8|a0 and attempts to parse a quad value using either the supplied format string in r9|a1 or the current global formatting template for quad integers.

- When successful, #ERR_NONE is returned in r1|d1 and the quad value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___


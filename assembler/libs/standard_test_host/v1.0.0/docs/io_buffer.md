[Standard Test Host Library](../README.md) > [IO Functions](./io.md)

# Character Buffer Input/Output
These routines handle conversion between text and scalar values.

### io_cbuf_format_byte
r0|d0:uint64 result io_cbuf_format_byte(char\* buffer, r0|d0:int8 value, r9|a1:char const\* format)
```asm
    move.q  buffer, a0
    move.b  value, d0
    hcf     io_cbuf_format_byte
```
Formats the byte integer in r0|d0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for byte integers if r9|a1 is zero.

- Only the lowest 8 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_format_word
r0|d0:uint64 result io_cbuf_format_word(char\* buffer, r0|d0:int16 value, r9|a1:char const\* format)
```asm
    move.q  buffer, a0
    move.w  value, d0
    hcf     io_cbuf_format_word
```
Formats the word integer in r0|d0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for word integers if r9|a1 is zero.

- Only the lowest 16 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_format_long
r0|d0:uint64 result io_cbuf_format_long(char\* buffer, r0|d0:int32 value, r9|a1:char const\* format)
```asm
    move.q  buffer, a0
    move.l  value, d0
    hcf     io_cbuf_format_long
```
Formats the long integer in r0|d0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for long integers if r9|a1 is zero.

- Only the lowest 32 bits of r0|d0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_format_quad
r0|d0:uint64 result io_cbuf_format_quad(char\* buffer, r0|d0:int32 value, r9|a1:char const\* format)
```asm
    move.q  buffer, a0
    move.l  value, d0
    hcf     io_cbuf_format_quad
```
Formats the quad integer in r0|d0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for quad integers if r9|a1 is zero.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_format_single
r0|d0:uint64 result io_cbuf_format_single(char\* buffer, fp0:float32 value, r9|a1:char const\* format)
```asm
    move.q  buffer, a0
    fmove.s value, fp0
    hcf     io_cbuf_format_single
```
Formats the single precision float in fp0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for quad integers if r9|a1 is zero.

- Only the lowest 32 bits of fp0 are used, all higher bits are ignored.
- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_format_double
r0|d0:uint64 result io_cbuf_format_double(char* buffer, fp0:float64 value, r9|a1:char const* format)
```asm
    move.q  buffer, a0
    fmove.d value, fp0
    hcf     io_cbuf_format_double
```
Formats the quad integer in r0|d0 into to the character buffer pointed to by r8|a0. Conversion uses either the supplied format template pointed to by r9|a1 or the current global formatting template for quad integers if r9|a1 is zero.

- When successful, #ERR_NONE is returned in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r0|d0
- If any other issue occurs, #ERR_WRITE is returned in r0|d0.
- Since v1.0.0
___

### io_cbuf_parse_byte
r0|d0:int8 value, r1|d1 uint64 result io_cbuf_parse_byte(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  buffer, a0
    lea     format, a1
    hcf     io_cbuf_parse_byte
    move.b  d0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for byte integers if r9|d1 is zero. The parsed byte value is returned in r0|d0.

- When successful, #ERR_NONE is returned in r1|d1 and the byte value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_cbuf_parse_word
r0|d0:int16 value, r1|d1 uint64 result io_cbuf_parse_word(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_cbuf_parse_word
    move.w  d0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for word integers if r9|d1 is zero. The parsed word value is returned in r0|d0.

- When successful, #ERR_NONE is returned in r1|d1 and the word value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_cbuf_parse_long
r0|d0:int32 value, r1|d1 uint64 result io_cbuf_parse_long(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_cbuf_parse_long
    move.l  d0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for long integers if r9|d1 is zero. The parsed long value is returned in r0|d0.

- When successful, #ERR_NONE is returned in r1|d1 and the long value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_cbuf_parse_quad
r0|d0:int64 value, r1|d1 uint64 result io_cbuf_parse_quad(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  stream, a0
    lea     format, a1
    hcf     io_cbuf_parse_quad
    move.q  d0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for quad integers if r9|d1 is zero. The parsed quad value is returned in r0|d0.

- When successful, #ERR_NONE is returned in r1|d1 and the quad value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_cbuf_parse_single
fp0:float32 value, r1|d1 uint64 result io_cbuf_parse_single(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  buffer, a0
    lea     format, a1
    hcf     io_cbuf_parse_single
    fmove.s fp0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for single precision floats if r9|d1 is zero. The parsed single value is returned in fp0.

- When successful, #ERR_NONE is returned in r1|d1 and the byte value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

### io_cbuf_parse_double
fp0:float64 value, r1|d1 uint64 result io_cbuf_parse_double(r8|a0:char const* buffer, r9|a1:char const* format)
```asm
    move.q  buffer, a0
    lea     format, a1
    hcf     io_cbuf_parse_double
    fmove.d fp0, value
```
Parses the text in the character buffer pointed to by r8|a0 using the supplied format template pointed to by r9|a1 or the current global formatting template for double precision floats if r9|d1 is zero. The parsed double value is returned in fp0.

- When successful, #ERR_NONE is returned in r1|d1 and the byte value in r0|d0.
- If r8|a0 contains zero, #ERR_NULL_PTR is returned in r1|d1
- If any other issue occurs, #ERR_READ is returned in r1|d1.
- Since v1.0.0
___

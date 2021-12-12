[Standard Test Host Library](../README.md) > [IO Functions](./IO.md)

# Default Format Control

### io_set_fmt_byte
void io_set_fmt_byte(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_byte
```
Sets the default global formatting template for byte integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_byte or io_file_print_byte.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_word
void io_set_fmt_word(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_word
```
Sets the default global formatting template for word integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_word or io_file_print_word.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_long
void io_set_fmt_long(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_long
```
Sets the default global formatting template for long integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_long or io_file_print_long.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_quad
void io_set_fmt_quad(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_quad
```
Sets the default global formatting template for quad integer values. The formatting template follows C standard library conventions and allows any signed or unsigned qualifiers.

- Only affects values written using io_print_quad or io_file_print_quad.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_single
void io_set_fmt_single(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_single
```
Sets the default global formatting template for single precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_single or io_file_print_single.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___
### io_set_fmt_double
void io_set_fmt_double(r8|a0:char const\* format)
```asm
    lea format, a0
    hcf io_set_fmt_double
```
Sets the default global formatting template for double precision floating point values. The formatting template follows C standard library conventions.

- Only affects values written using io_print_double or io_file_print_double.
- When r8|a0 contains zero, formatting is returned to the default for the type.
- Since v1.0.0
___

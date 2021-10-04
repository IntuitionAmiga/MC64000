
;  888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
;  8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
;  88888b.d88888 888    888 888          d8P 888  888  d8P
;  888Y88888P888 888        888d888b.   d8P  888  888d88K
;  888 Y888P 888 888        888P "Y88b d88   888  8888888b
;  888  Y8P  888 888    888 888    888 8888888888 888  Y88b
;  888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
;  888       888  "Y8888P"   "Y8888P"        888  888    Y88b
;
;   - 64-bit 680x0-inspired Virtual Machine and assembler -
;
; Stub library for the standard test host IO routines

; Defines used by the IO ABI

    ; file open
    @globdef IO_MODE_READ          0
    @globdef IO_MODE_WRITE         1
    @globdef IO_MODE_APPEND        2
    @globdef IO_MODE_READ_UPDATE   3
    @globdef IO_MODE_WRITE_UPDATE  4
    @globdef IO_MODE_APPEND_UPDATE 5

    ; file errors
    @globdef IO_ERR_NONE           0
    @globdef IO_ERR_EOF            1
    @globdef IO_ERR_OPEN           2
    @globdef IO_ERR_CREATE         3
    @globdef IO_ERR_READ           4
    @globdef IO_ERR_WRITE          5
    @globdef IO_ERR_CLOSE          6

; Break out to the host, reset the stack and return.
.invoke:
    hcf #0
    add.q #1, sp
    rts

; (Re) initialise the IO state
io_init:
    move.b #0, -(sp)
    bra.b .invoke

; (Re) initialise the IO state
io_done:
    move.b #1, -(sp)
    bra.b .invoke

; Print the string pointed to by r0
io_print_string:
    move.b #2, -(sp)
    bra.b .invoke

; Print the byte in r0 using the current format template for byte integer
io_print_byte:
    move.b #3, -(sp)
    bra.b .invoke

; Print the word in r0 using the current format template for word integer
io_print_word:
    move.b #4, -(sp)
    bra.b .invoke

; Print the long in r0 using the current format template for long integer
io_print_long:
    move.b #5, -(sp)
    bra.b .invoke

; Print the quad in r0 using the current format template for quad integer
io_print_quad:
    move.b #6, -(sp)
    bra.b .invoke

; Print the single in fp0 using the current format template for single float
io_print_single:
    move.b #7, -(sp)
    bra.b .invoke

; Print the double in fp0 using the current format template for single float
io_print_double:
    move.b #8, -(sp)
    bra.b .invoke

; Set the format template for byte integer to the string pointed to by r0
io_set_fmt_byte:
    move.b #9, -(sp)
    bra.b .invoke

; Set the format template for word integer to the string pointed to by r0
io_set_fmt_word:
    move.b #10, -(sp)
    bra.b .invoke

; Set the format template for long integer to the string pointed to by r0
io_set_fmt_long:
    move.b #11, -(sp)
    bra.b .invoke

; Set the format template for quad integer to the string pointed to by r0
io_set_fmt_quad:
    move.b #12, -(sp)
    bra.b .invoke

; Set the format template for single float to the string pointed to by r0
io_set_fmt_single:
    move.b #13, -(sp)
    bra.b .invoke

; Set the format template for single float to the string pointed to by r0
io_set_fmt_double:
    move.b #14, -(sp)
    bra.b .invoke

; Revert to the default format template for byte integer
io_clr_fmt_byte:
    move.b #15, -(sp)
    bra.b .invoke

; Revert to the default format template for word integer
io_clr_fmt_word:
    move.b #16, -(sp)
    bra.b .invoke

; Revert to the default format template for long integer
io_clr_fmt_long:
    move.b #17, -(sp)
    bra.b .invoke

; Revert to the default format template for quad integer
io_clr_fmt_quad:
    move.b #18, -(sp)
    bra.b .invoke

; Revert to the default format template for single float
io_clr_fmt_single:
    move.b #19, -(sp)
    bra.b .invoke

; Revert to the default format template for double float
io_clr_fmt_double:
    move.b #20, -(sp)
    bra.b .invoke

; Repeat invoke here to keep branches short
.invoke_1:
    hcf #0
    add.q #1, sp
    rts

io_file_open:
    move.b #21, -(sp)
    bra.b .invoke_1

io_file_seek:
    move.b #22, -(sp)
    bra.b .invoke_1

io_file_tell:
    move.b #23, -(sp)
    bra.b .invoke_1

io_file_read:
    move.b #24, -(sp)
    bra.b .invoke_1

io_file_write:
    move.b #25, -(sp)
    bra.b .invoke_1

io_file_close:
    move.b #26, -(sp)
    bra.b .invoke_1

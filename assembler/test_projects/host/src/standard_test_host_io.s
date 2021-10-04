
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


; Break out to the host, reset the stack and return.
.invoke:
    hcf #0
    add.q #1, sp
    rts

; (Re) initialise the IO state
io_init:
    move.b #0, -(sp)
    bra .invoke

; Print the string pointed to by r0
io_print_string:
    move.b #1, -(sp)
    bra .invoke

; Print the byte in r0 using the current format template for byte integer
io_print_byte:
    move.b #2, -(sp)
    bra .invoke

; Print the word in r0 using the current format template for word integer
io_print_word:
    move.b #3, -(sp)
    bra .invoke

; Print the long in r0 using the current format template for long integer
io_print_long:
    move.b #4, -(sp)
    bra .invoke

; Print the quad in r0 using the current format template for quad integer
io_print_quad:
    move.b #5, -(sp)
    bra .invoke

; Print the single in fp0 using the current format template for single float
io_print_single:
    move.b #6, -(sp)
    bra .invoke

; Print the double in fp0 using the current format template for single float
io_print_double:
    move.b #7, -(sp)
    bra .invoke

; Set the format template for byte integer to the string pointed to by r0
io_setfmt_byte:
    move.b #8, -(sp)
    bra .invoke

; Set the format template for word integer to the string pointed to by r0
io_setfmt_word:
    move.b #9, -(sp)
    bra .invoke

; Set the format template for long integer to the string pointed to by r0
io_setfmt_long:
    move.b #10, -(sp)
    bra .invoke

; Set the format template for quad integer to the string pointed to by r0
io_setfmt_quad:
    move.b #11, -(sp)
    bra .invoke

; Set the format template for single float to the string pointed to by r0
io_setfmt_single:
    move.b #12, -(sp)
    bra .invoke

; Set the format template for single float to the string pointed to by r0
io_setfmt_double:
    move.b #13, -(sp)
    bra .invoke

; Revert to the default format template for byte integer
io_clrfmt_byte:
    move.b #14, -(sp)
    bra .invoke

; Revert to the default format template for word integer
io_clrfmt_word:
    move.b #15, -(sp)
    bra .invoke

; Revert to the default format template for long integer
io_clrfmt_long:
    move.b #16, -(sp)
    bra .invoke

; Revert to the default format template for quad integer
io_clrfmt_quad:
    move.b #17, -(sp)
    bra .invoke

; Revert to the default format template for single float
io_clrfmt_single:
    move.b #18, -(sp)
    bra .invoke

; Revert to the default format template for double float\
io_clrfmt_double:
    move.b #19, -(sp)
    bra .invoke

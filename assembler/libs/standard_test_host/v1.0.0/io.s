
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
    @equ IO_OPEN_READ          0
    @equ IO_OPEN_WRITE         1
    @equ IO_OPEN_APPEND        2
    @equ IO_OPEN_READ_UPDATE   3
    @equ IO_OPEN_WRITE_UPDATE  4
    @equ IO_OPEN_APPEND_UPDATE 5

    ; file seekmodes
    @equ IO_SEEK_START         0
    @equ IO_SEEK_END           1
    @equ IO_SEEK_CURRENT       2

    ; io specific errors
    @equ ERR_FILE              200
    @equ ERR_OPEN              201
    @equ ERR_READ              202
    @equ ERR_WRITE             203

; Break out to the host, reset the stack and return.
.invoke_0:
    hcf #0
    add.q #1, sp
    rts

io_init:
    move.b #0, -(sp)
    bra.b .invoke_0

io_done:
    move.b #1, -(sp)
    bra.b .invoke_0

io_print_string:
    move.b #2, -(sp)
    bra.b .invoke_0

io_print_byte:
    move.b #3, -(sp)
    bra.b .invoke_0

io_print_word:
    move.b #4, -(sp)
    bra.b .invoke_0

io_print_long:
    move.b #5, -(sp)
    bra.b .invoke_0

io_print_quad:
    move.b #6, -(sp)
    bra.b .invoke_0

io_print_single:
    move.b #7, -(sp)
    bra.b .invoke_0

io_print_double:
    move.b #8, -(sp)
    bra.b .invoke_0

io_set_fmt_byte:
    move.b #9, -(sp)
    bra.b .invoke_0

io_set_fmt_word:
    move.b #10, -(sp)
    bra.b .invoke_0

io_set_fmt_long:
    move.b #11, -(sp)
    bra.b .invoke_0

io_set_fmt_quad:
    move.b #12, -(sp)
    bra.b .invoke_0

io_set_fmt_single:
    move.b #13, -(sp)
    bra.b .invoke_0

io_set_fmt_double:
    move.b #14, -(sp)
    bra.b .invoke_0

io_file_open:
    move.b #15, -(sp)
    bra.b .invoke_0

io_file_seek:
    move.b #16, -(sp)
    bra.b .invoke_0

io_file_tell:
    move.b #17, -(sp)
    bra.b .invoke_0

io_file_read:
    move.b #18, -(sp)
    bra.b .invoke_0

io_file_write:
    move.b #19, -(sp)
    bra.b .invoke_0

io_file_close:
    move.b #20, -(sp)
    bra.b .invoke_0

; We run out of short branch distance after the first 20 stubs.
.invoke_1:
    hcf #0
    add.q #1, sp
    rts

io_file_print_string:
    move.b #21, -(sp)
    bra.b .invoke_1

io_file_print_byte:
    move.b #22, -(sp)
    bra.b .invoke_1

io_file_print_word:
    move.b #23, -(sp)
    bra.b .invoke_1

io_file_print_long:
    move.b #24, -(sp)
    bra.b .invoke_1

io_file_print_quad:
    move.b #25, -(sp)
    bra.b .invoke_1

io_file_print_single:
    move.b #26, -(sp)
    bra.b .invoke_1

io_file_print_double:
    move.b #27, -(sp)
    bra.b .invoke_1

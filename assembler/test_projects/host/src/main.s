
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
;   Hello world.

    @export main x
main:
    ; set up how we want our countdown number to look
    lea    count_fmt, r0
    bsr    io_setfmt_long

    lea    hello, r2
    move.l #10, r1
.loop:
    move.q r1, r0
    bsr    io_print_long
    move.q r2, r0
    bsr    io_print_string
    dbnz   r1, .loop

    @export exit x
exit:
    rts

    @align 0, 8
hello:
    dc.b " >>>> Hello outside world!\n\0"

count_fmt:
    dc.b "%3d\0"


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
; My first project - main.s

    @export main
    @export my_exported_func
    @define my_native_func #0x69
    @stacksize 128

main:
    move.q #1, r0
    bsr my_exported_func
    move.q #2, r1
    rts

my_exported_func:
    move.q #3, r2
    bsr my_hidden_func
    move.q #4, r3
    rts

my_hidden_func:
    move.q #5, r4
    hcf my_native_func
    move.q #6, r5
    rts

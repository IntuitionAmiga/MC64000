
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
    @export do_something
    ;@import my_external_reference
main:
    lea test_global, r5 ; 7 bytes: [command] [dst ea] [src ea] [disp 0] [disp 1] [disp 2] [disp 3]

    move.b test_global, r4

    ; insert a list of 10 one byte instructions. The reference to the test label should be 10 bytes after the PC
    rts
    rts
    rts
    rts
    rts
    rts
    rts
    rts
    rts
    rts

test_global:
    rts

do_something:
    lea main, r3
    rts

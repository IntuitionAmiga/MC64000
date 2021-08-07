
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

    @enable log_label_iemode

    @import my_unusued_reference rw
    @import my_external_reference x
    @import my_other_external_reference

    @import my_unusued_reference rwx

    @export main x
main:
    move.q #1, r0
    bsr danger

    ; self modifying code! Change danger to a simple noop
    move.b #7, danger
    bsr danger

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
    lea main, r3
    rts

    @export do_something x
do_something:
    lea main, r3
    move.q my_external_reference, r9
    rts

    @export danger rwx
danger:
    add.q r0, r0
    rts


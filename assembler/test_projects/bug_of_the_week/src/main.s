
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
; Bug Of The Week project - main.s

    @def nanotime dc.b 0xF0 ; super undocumented opcodes ftw

    @equ FP_ZERO 0.0
    @equ FP_MINUS_HALF -0.5
    @equ FP_MINUS_ONE -1.0

main:
    hcf io_init

    fbgt.d fp0, fp1, .blah

    fsgt.d fp1, fp0, d2

exit:
    hcf io_done
    rts

.blah:
    bra.s exit

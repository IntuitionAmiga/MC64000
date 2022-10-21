
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


main:

    link    a6, #-64

    move.q  a6, a5

    fmove.s #1.0, fp0
    fmove.s #2.0, fp1,
    fmove.s #3.0, fp2
    fmacc.s fp1, fp2, fp0 ; expect 7.0 in fp0

    fmove.s #0.5, fp3
    fmadd.s fp0, fp1, fp3, fp4

    unlk a6

    bpl.q d1, main
    fbnz.s fp15, main

exit:
    rts



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

; Defines used by the Mem ABI

    ; Mem specific errors
    @equ ERR_NO_MEM  100
    @equ ERR_MEM     101

    @equ mem_init       #0, #1
    @equ mem_done       #1, #1
    @equ mem_alloc      #2, #1
    @equ mem_free       #3, #1
    @equ mem_copy       #4, #1
    @equ mem_fill_byte  #5, #1
    @equ mem_fill_word  #6, #1
    @equ mem_fill_long  #7, #1
    @equ mem_fill_quad  #8, #1

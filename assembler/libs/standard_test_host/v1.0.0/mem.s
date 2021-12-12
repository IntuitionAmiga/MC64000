
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

    @def mem_vector      #1

    @equ mem_init        #0, mem_vector
    @equ mem_done        #1, mem_vector
    @equ mem_alloc       #2, mem_vector
    @equ mem_free        #3, mem_vector
    @equ mem_copy        #4, mem_vector
    @equ mem_fill_byte   #5, mem_vector
    @equ mem_fill_word   #6, mem_vector
    @equ mem_fill_long   #7, mem_vector
    @equ mem_fill_quad   #8, mem_vector

    @equ mem_and_byte   #9,  mem_vector
    @equ mem_and_word   #10, mem_vector
    @equ mem_and_long   #11, mem_vector
    @equ mem_and_quad   #12, mem_vector

    @equ mem_or_byte    #13, mem_vector
    @equ mem_or_word    #14, mem_vector
    @equ mem_or_long    #15, mem_vector
    @equ mem_or_quad    #16, mem_vector

    @equ mem_eor_byte   #17, mem_vector
    @equ mem_eor_word   #18, mem_vector
    @equ mem_eor_long   #19, mem_vector
    @equ mem_eor_quad   #20, mem_vector

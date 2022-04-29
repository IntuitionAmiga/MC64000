
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

    @def mem_vector         #1

    @equ mem_init           #0, mem_vector
    @equ mem_done           #1, mem_vector
    @equ mem_alloc          #2, mem_vector
    @equ mem_free           #3, mem_vector

    @equ mem_alloc_buffer   #4, mem_vector
    @equ mem_free_buffer    #5, mem_vector
    @equ mem_alloc_element  #6, mem_vector
    @equ mem_free_element   #7, mem_vector

    @equ mem_copy           #8, mem_vector
    @equ mem_swap_word      #9, mem_vector
    @equ mem_swap_long     #10, mem_vector
    @equ mem_swap_quad     #11, mem_vector


    @equ mem_fill_byte     #12, mem_vector
    @equ mem_fill_word     #13, mem_vector
    @equ mem_fill_long     #14, mem_vector
    @equ mem_fill_quad     #15, mem_vector

    @equ mem_and_byte      #16, mem_vector
    @equ mem_and_word      #17, mem_vector
    @equ mem_and_long      #18, mem_vector
    @equ mem_and_quad      #19, mem_vector

    @equ mem_or_byte       #20, mem_vector
    @equ mem_or_word       #21, mem_vector
    @equ mem_or_long       #22, mem_vector
    @equ mem_or_quad       #23, mem_vector

    @equ mem_eor_byte      #24, mem_vector
    @equ mem_eor_word      #25, mem_vector
    @equ mem_eor_long      #26, mem_vector
    @equ mem_eor_quad      #27, mem_vector

    @equ mem_find_byte     #28, mem_vector
    @equ mem_find_word     #29, mem_vector
    @equ mem_find_long     #30, mem_vector
    @equ mem_find_quad     #31, mem_vector

    @equ mem_strlen        #32, mem_vector
    @equ mem_strcmp        #33, mem_vector

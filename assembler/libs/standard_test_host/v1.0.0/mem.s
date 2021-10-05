
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

; Break out to the host, reset the stack and return.
.invoke:
    hcf #1
    add.q #1, sp
    rts

mem_init:
    move.b #0, -(sp)
    bra.b .invoke

mem_done:
    move.b #1, -(sp)
    bra.b .invoke

mem_alloc:
    move.b #2, -(sp)
    bra.b .invoke

mem_free:
    move.b #3, -(sp)
    bra.b .invoke

mem_copy:
    move.b #4, -(sp)
    bra.b .invoke

mem_fill_byte:
    move.b #5, -(sp)
    bra.b .invoke

mem_fill_word:
    move.b #6, -(sp)
    bra.b .invoke

mem_fill_long:
    move.b #7, -(sp)
    bra.b .invoke

mem_fill_quad:
    move.b #8, -(sp)
    bra.b .invoke

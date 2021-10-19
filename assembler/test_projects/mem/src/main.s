
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
; Empty project - main.s

    @def MEM_BUFFER_SIZE 16777216

main:
    move.q  #MEM_BUFFER_SIZE, d7
    move.q  d7, d0
    bsr     mem_alloc
    bnz.q   a0, .allocated
    lea     .err_no_memory_1, a0
    bsr     io_print_string
    move.q  d7, d0
    bsr     io_print_quad
    lea     .err_no_memory_2, a0
    bsr     io_print_string
    bra     exit

.allocated:
    move.l  #255,  d6

.loop:
    move.q  a0,     a6
    move.q  d6,     d0
    move.q  d7,     d1
    bsr     mem_fill_byte
    dbnz    d6,     .loop

exit:
    ; free buffer (null safe)
    move.q      a6,     a0
    bsr         mem_free

    rts

.err_no_memory_1:
    dc.b "Error: Couldn't allocate \0"
.err_no_memory_2:
    dc.b " bytes of memory for buffer.\n\0"

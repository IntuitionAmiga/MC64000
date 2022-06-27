
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

    @def nanotime dc.b 0xF0 ; super undocumented opcodes ftw

main:
    hcf io_init
    hcf mem_init

    move.l  #$f0003, d0

    hcf mem_alloc_buffer
    move.q  a0, a2

    move.l  #1024, d2
.bad:
    move.q  a2, a0
    hcf     mem_alloc_element
    dbnz    d2, .bad

    move.q a2, a0
    hcf mem_free_buffer

exit:
    hcf mem_done
    hcf io_done
    rts


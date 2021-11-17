
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
;   Hello world.

main:
    hcf     io_init
    move.q  host_cli_num_params, d2
    move.q  host_cli_params, a2
.loop:
    move.q  (a2)+, a0
    hcf     io_print_string
    lea     .newline, a0
    hcf     io_print_string
    dbnz    d2, .loop

    fbif.s  #FLOAT_NAN, fp0, .never

exit:
    hcf     io_done
    rts

.never:
    move.q  #1, d0
    bra.b   exit

.newline:
    dc.b "\n\0"


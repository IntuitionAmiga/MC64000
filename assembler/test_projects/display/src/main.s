
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

main:
    hcf     io_init
    hcf     display_init

    lea     on_frame, r8

    hcf     display_begin

    move.q  #0xabadcafe, r7

    lea     .exit_message, r8
    hcf     io_print_string

exit:
    move.q  #0xdeadbeef, r6
    hcf     display_done
    hcf     io_done
    rts

on_frame:
    add.q   #1, r5
    lea     .frame_message, r8
    hcf     io_print_string
    rts

.frame_message:
    dc.b "VM frame callback called\n\0"

.exit_message:
    dc.b "Exited from native loop\n\0"

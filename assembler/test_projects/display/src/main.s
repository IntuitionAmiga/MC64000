
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

main:
    hcf     io_init
    hcf     display_init

    ; try to open the display
    move.q  .display_properties, r0
    hcf     display_open
    biz.q   a0, exit
    move.q  a0, -(sp) ; save the context

    ; Populate the callback handlers
    lea     on_frame, (a0)  ; this is called every frame
    lea     on_event, 8(a0) ; this is called per input event

    ; Begin the main event loop
    hcf     display_begin

    ; Close the display
    move.q  (sp)+, a0
    hcf     display_close

exit:
    hcf     display_done
    hcf     io_done
    rts

on_frame:
    lea     .frame_message, a0
    hcf     io_print_string
    rts

on_event:
    lea     .event_message, a0
    hcf     io_print_string
    rts

    @align  0, 8
.display_properties:
    dc.w 640, 480, PXL_ARGB, 0x0000 ; width, height, format, flags

.frame_message:
    dc.b "VM timer callback\n\0"

.event_message:
    dc.b "VM event callback\n\0"

.exit_message:
    dc.b "Exited from native loop\n\0"

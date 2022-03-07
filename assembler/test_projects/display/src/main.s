
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
    biz.q   a0, exit  ; no display?
    move.q  a0, -(sp) ; save the context

    ; Populate the callback handlers
    lea     on_frame,      DISPLAY_REG_CALL_FRAME(a0)
    lea     on_key_down,   DISPLAY_REG_CALL_KEY_PRESS(a0)
    lea     on_key_up,     DISPLAY_REG_CALL_KEY_RELEASE(a0)
    lea     on_mouse_down, DISPLAY_REG_CALL_BUTTON_PRESS(a0)
    lea     on_mouse_up,   DISPLAY_REG_CALL_BUTTON_RELEASE(a0)

    clr.q   d2

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
    move.l  DISPLAY_REG_SOFT_BUFFER_PIXELS(a0), d0
    move.q  DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a0
    move.l  #1, d1
.fill:
    move.l  d2, (a0)+
    add.l   d1, d2
    dbnz    d0, .fill
    rts

on_key_down:
    move.q  a0, a2
    lea     .key_down_message, a0
    bra     .button

on_key_up:
    move.q  a0, a2
    lea     .key_up_message, a0
    bra     .button

on_mouse_down:
    move.q  a0, a2
    lea     .mouse_down_message, a0
    bra     .button

on_mouse_up:
    move.q  a0, a2
    lea     .mouse_up_message, a0
.button:
    hcf     io_print_string
    clr.q   d0
    clr.q   a0
    move.w  DISPLAY_REG_EVENT_CODE(a2), d0 ; get the raw key code in d0
    hcf     io_print_long
    lea     .newline, a0
    hcf     io_print_string
    clr.l   d1
    move.q  DISPLAY_REG_SOFT_BUFFER_ADDRESS(a2), a0
    move.w  DISPLAY_REG_EVENT_CODE(a2), d0
    move.l  DISPLAY_REG_SOFT_BUFFER_BYTES(a2), d1
    hcf     mem_fill_byte
    rts

    @align  0, 8
.display_properties:
    ; width, height, flags
    dc.w 640, 480, 0x000F
    ; format, target refresh Hz
    dc.b PXL_ARGB, 60

.frame_message:
    dc.b "VM frame\n\0"
.key_down_message:
    dc.b "VM key pressed \0"
.key_up_message:
    dc.b "VM key released \0"
.mouse_down_message:
    dc.b "VM button pressed \0"
.mouse_up_message:
    dc.b "VM button released \0"
.newline:
    dc.b "\n\0"


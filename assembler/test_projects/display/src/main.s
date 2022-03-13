
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

    move.q  DISPLAY_REG_PALETTE_ADDRESS(a0), a1
    move.q  #256, d0
    move.q  #0, d1,
.grad:
    move.l  d1, (a1)+
    add.l   #0x010204, d1
    dbnz    d0, .grad

    ; Populate the callback handlers
    lea     on_frame,      DISPLAY_REG_CALL_FRAME(a0)
    lea     on_key_down,   DISPLAY_REG_CALL_KEY_PRESS(a0)

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
    move.b  d2, (a0)+
    add.l   d1, d2
    dbnz    d0, .fill
    rts

; handlers
on_key_down: ; a0 contains display context
    bclr.w     #DISPLAY_BIT_EVENT_LOOP_RUNNING, DISPLAY_REG_FLAGS(a0)
    rts

    @align  0, 8
.display_properties:
    ; width, height, flags
    dc.w 256, 256, 1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES
    ; format, target refresh Hz
    dc.b PXL_CLUT_8, 30


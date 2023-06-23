
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
    hcf     display_init

    ; try to open the display. The vector expects the parameters packed into d0
    lea     .display_properties, a0
    hcf     display_open
    biz.q   a0, exit  ; no display?
    move.q  a0, -(sp) ; save the context on the stack for safe keeping

    ; Populate the callback handlers
    lea     on_frame,      DISPLAY_REG_CALL_FRAME(a0)     ; called every frame before refresh
    lea     on_key_down,   DISPLAY_REG_CALL_KEY_PRESS(a0) ; called before every frame for keypresses


    move.q  DISPLAY_REG_PALETTE_ADDRESS(a0), a1

    ; Don't need black, white or greys as these can be set as 111xxxxx ham pixels

    ; blues
    move.w  #%0000000000000001, (a1)+
    move.w  #%0000000000000011, (a1)+
    move.w  #%0000000000000111, (a1)+
    move.w  #%0000000000001111, (a1)+
    move.w  #%0000000000011111, (a1)+

    ; greens
    move.w  #%0000000000100000, (a1)+
    move.w  #%0000000001100000, (a1)+
    move.w  #%0000000011100000, (a1)+
    move.w  #%0000000111100000, (a1)+
    move.w  #%0000001111100000, (a1)+

    ; cyans
    move.w  #%0000000000100001, (a1)+
    move.w  #%0000000001100011, (a1)+
    move.w  #%0000000011100111, (a1)+
    move.w  #%0000000111101111, (a1)+
    move.w  #%0000001111111111, (a1)+

    ; reds
    move.w  #%0000010000000000, (a1)+
    move.w  #%0000110000000000, (a1)+
    move.w  #%0001110000000000, (a1)+
    move.w  #%0011110000000000, (a1)+
    move.w  #%0111110000000000, (a1)+

    ; magentas
    move.w  #%0000010000000001, (a1)+
    move.w  #%0000110000000011, (a1)+
    move.w  #%0001110000000111, (a1)+
    move.w  #%0011110000001111, (a1)+
    move.w  #%0111110000011111, (a1)+

    ; yellows
    move.w  #%0000010000100000, (a1)+
    move.w  #%0000110001100000, (a1)+
    move.w  #%0001110011100000, (a1)+
    move.w  #%0011110111100000, (a1)+
    move.w  #%0111111111100000, (a1)+

    move.l  DISPLAY_REG_SOFT_BUFFER_PIXELS(a0), d0
    move.q  DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a1

    clr.q   d1
    clr.q   d2
.fill:
    and.b   #%00011111,d2
    //or.b    #%01100000,d2
    move.b  d2, (a1)+
    add.b   #1,d1
    move.b  d1,d2
    dbnz    d0, .fill

    ; Begin the main event loop
    hcf     display_begin

    ; Close the display
    move.q  (sp)+, a0
    hcf     display_close

exit:
    ; We are done.
    hcf     display_done
    rts

on_frame:
    ; We are called every frame before redrawing
    ; Display context is in a0

    move.l  DISPLAY_REG_SOFT_BUFFER_PIXELS(a0), d0
    move.q  DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a1

    ; render something here!


    ; done
    rts


on_key_down:
    ; We are called every keypress, before the main frame render
    ; a0 contains display context

    ; Clear the event loop running flag
    bclr.w  #DISPLAY_BIT_EVENT_LOOP_RUNNING, DISPLAY_REG_FLAGS(a0)
    rts

    @align  0, 8
.display_properties:
    ; view dimensions
    dc.w 320, 240

    ; buffer dimensions and view offset
    dc.w 320, 240, 0, 0

    ; flags
    dc.w 1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES

    ; pixel format
    dc.b PXL_HAM_555

    ; target refresh rate (Hz)
    dc.b 60



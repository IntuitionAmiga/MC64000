
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
    hcf     mem_init
    hcf     display_init

    ; try to open the display. The vector expects the parameters packed into d0
    lea     .display_properties, a0
    hcf     display_open
    biz.q   a0, exit  ; no display?
    move.q  a0, -(sp) ; save the context on the stack for safe keeping

    ; Display context structure (X denotes read-only field)
    ;
    ; DISPLAY_REG_CALL_FRAME             address - if set, called once per frame, before refresh
    ; DISPLAY_REG_CALL_KEY_PRESS         address - if set, called per event before frame
    ; DISPLAY_REG_CALL_KEY_RELEASE       address - if set, called per event before frame
    ; DISPLAY_REG_CALL_BUTTON_PRESS      address - if set, called per event before frame
    ; DISPLAY_REG_CALL_BUTTON_RELEASE    address - if set, called per event before frame
    ; DISPLAY_REG_CALL_MOVEMENT          address - if set, called per event before frame
    ;
    ; ; Display properties
    ; DISPLAY_REG_SOFT_BUFFER_ADDRESS  X address - contains the address of the framebuffer
    ; DISPLAY_REG_PALETTE_ADDRESS        address - contains the address of the palette, for 256 colour
    ;
    ; DISPLAY_REG_SOFT_BUFFER_PIXELS   X long    - contains the number of pixels in the framebuffer
    ; DISPLAY_REG_SOFT_BUFFER_BYTES    X long    - contains the size of the framebuffer, in bytes
    ;
    ; DISPLAY_REG_WIDTH                X word    - contains the width of the framebuffer in pixels
    ; DISPLAY_REG_HEIGHT               X word    - contains the height of the framebuffer in pixels
    ; DISPLAY_REG_FLAGS                  word    - contains flags
    ; DISPLAY_REG_PXL_FORMAT           X byte    - contains the pixel format
    ; DISPLAY_REG_REFRESH_HZ           X byte    - contains the target refresh rate in Hz
    ;
    ; DISPLAY_REG_EVENT_CODE           X word    - contains the raw event code for a key/mouse press
    ; DISPLAY_REG_EVENT_MASK           X word    - contains the mask of pressed buttons / modifiers
    ; DISPLAY_REG_POSITION_X           X word    - contains the current cursor X position
    ; DISPLAY_REG_POSITION_Y           X word    - contains the current cursor Y positoin

    ; Populate the callback handlers
    lea     on_key_down,   DISPLAY_REG_CALL_KEY_PRESS(a0) ; called before every frame for keypresses

    ; Fixed palette
    lea     .palette,      DISPLAY_REG_PALETTE_ADDRESS(a0)
    lea     .filth_script, DISPLAY_REG_FILTH(a0)

    ; paint the initial image into the buffer
    clr.q   d0
    move.q  a0, a2
    move.q  DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a1
    move.l  DISPLAY_REG_SOFT_BUFFER_BYTES(a0), d0
    lea     .image, a0
    hcf     mem_copy

    ; Begin the main event loop
    move.q  a2, a0
    hcf     display_begin

    ; Close the display
    move.q  (sp)+, a0
    hcf     display_close

exit:
    ; We are done.
    hcf     display_done
    hcf     mem_done
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

    ; buffer dimensions and offset registers
    dc.w 320, 240, 0, 0

    ; flags
    dc.w 1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES

    ; pixel format
    dc.b PXL_CLUT_8

    ; target refresh rate (Hz)
    dc.b 60

    ; Static Data...

    ; palette : 256 32-bit xRGB entries
    @align  0, 8
.palette:
    @incbin "../res/pal.bin"

    ; image : 320 x 240 x 8-bit colour index
.image:
    @incbin "../res/img.bin"

    @align  0, 8
.filth_script:

    ; Slice up the screen into horizontal bands
    dc.w    0, 0                  ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 5)

    dc.b    DISPLAY_FC_WAIT       ; wait until...
    dc.w    0, 92                 ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 13)

    dc.b    DISPLAY_FC_WAIT       ; wait until...
    dc.w    0, 110                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 21)

    dc.b    DISPLAY_FC_WAIT       ; wait until...
    dc.w    0, 125                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 29)

    dc.b    DISPLAY_FC_WAIT       ; wait until...
    dc.w    0, 154                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 37)

    dc.b    DISPLAY_FC_WAIT       ; wait until...
    dc.w    0, 176                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 45)

    dc.b    DISPLAY_FC_WAIT       ; wait until ...
    dc.w    0, 192                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 53)

    dc.b    DISPLAY_FC_WAIT       ; wait until ...
    dc.w    0, 220                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 61)

    dc.b    DISPLAY_FC_WAIT       ; wait until ...
    dc.w    0, 230                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value (script offset 69)

    ; Update the X offset of each slice
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    5, 5                   ; increment word at offset 5 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    13, 4                  ; increment word at offset 13 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    21, 3                  ; increment word at offset 21 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    29, 2                  ; increment word at offset 29 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    37, 1                  ; increment word at offset 37 the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    45, 2                  ; increment word at offset 45 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    53, 3                  ; increment word at offset 53 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    61, 5                  ; increment word at offset 61 (the x offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    69, 8                  ; increment word at offset 69 (the x offset value)
    dc.b    DISPLAY_FC_END         ; end


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
    move.q  .display_properties, d0
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



    ; Initialise palette here
    ;
    ; You can update palette entries at any time.
    ; You can chante the pointer to point at your own palette if you prefer but it has to have
    ; 256 32-bit entries.
    move.q  DISPLAY_REG_PALETTE_ADDRESS(a0), a1

    ; mock WB2.0 4-colour palette
    move.l  #$999999, (a1)+ ; grey
    move.l  #$101010, (a1)+ ; almost black
    move.l  #$EEEEEE, (a1)+ ; almost white
    move.l  #$6060CC, (a1)+ ; blueish



    ; Populate the callback handlers
    lea     on_frame,      DISPLAY_REG_CALL_FRAME(a0)     ; called every frame before refresh
    lea     on_key_down,   DISPLAY_REG_CALL_KEY_PRESS(a0) ; called before every frame for keypresses

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
    ; dimensions
    dc.w 320, 240

    ; flags
    dc.w 1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES

    ; pixel format
    dc.b PXL_CLUT_8

    ; target refresh rate (Hz)
    dc.b 30


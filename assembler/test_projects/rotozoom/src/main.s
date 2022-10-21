
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

    @enable log_expression_eval

    @def STEP         0.01
    @def ZOOM         1.0
    @def DISTANCE     0.5
    @def TEXTURE_DIM  7
    @def TEXTURE_SIZE 1 << TEXTURE_DIM
    @def MAX_TEXEL    TEXTURE_SIZE - 1

main:
    ; try to open the display
    hcf     display_init
    lea     .display_properties, a0
    hcf     display_open
    biz.q   a0, exit  ; no display?
    move.q  a0, -(sp) ; save the context on the stack

    ; Populate the callback handlers
    lea     on_frame,    DISPLAY_REG_CALL_FRAME(a0)
    lea     on_key_down, DISPLAY_REG_CALL_KEY_PRESS(a0)
    ;lea     .filth,      DISPLAY_REG_FILTH(a0)

    ; Set parameters
    lea     .texel_data,  a2
    fmove.s #STEP,        fp15
    fmove.s #ZOOM,        fp14
    fmove.s #DISTANCE,    fp13
    fmove.s #0.0,         fp12 ; current angle

    move.l  #MAX_TEXEL,   d4
    move.l  #TEXTURE_DIM, d5
    clr.q   d2
    clr.q   d3

    ; Begin the main event loop
    hcf     display_begin

    ; Close the display
    move.q  (sp)+, a0
    hcf     display_close

exit:
    hcf     display_done
    hcf     io_done
    rts

; renderer
on_frame: ; a0 contains display context
    fmove.s     fp12, fp10
    fsincos.s   fp10, fp11  ; fp10 = sin(angle), fp11 = cos(angle)
    fadd.s      fp15, fp12  ; angle += step
    fmove.s     fp14, fp9
    fmul.s      fp10, fp9
    fadd.s      fp13, fp9   ; fp9 = scale = (fsin * zoom) + dist

    ; set address and counters
    move.q      DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a1
    move.w      DISPLAY_REG_HEIGHT(a0), d2

.yloop:
    move.w      DISPLAY_REG_WIDTH(a0), d3
    fmovel.s    d2, fp8     ; fp8 = y
.xloop:
    fmovel.s    d3, fp7     ; fp7 = x

    ; calculate texel U
    fmove.s     fp11, fp6
    fmul.s      fp7,  fp6   ; fp6 = (fcos * x)
    fmove.s     fp10, fp5
    fmul.s      fp8,  fp5   ; fp5 = (fsin * y)
    fsub.s      fp5,  fp6   ; fp6 = (fcos * x) - (fsin * y)
    fmul.s      fp9,  fp6   ; fp6 = scale * ((fcos * x) - (fsin * y))
    fmoves.l    fp6,  d0    ; d0 = tex_u
    and.l       d4,   d0    ; d0 = tex_u & (TEXTURE_SIZE - 1)
    ; calculate texel V
    fmove.s     fp10, fp6
    fmul.s      fp7,  fp6   ; fp6 = (fsin * x)
    fmove.s     fp11, fp5
    fmul.s      fp8,  fp5   ; fp5 = (fcos * y)
    fadd.s      fp5,  fp6   ; fp6 = (fsin * x) + (fcos * y)
    fmul.s      fp9,  fp6   ; fp6 = scale * ((fsin * x) + (fcos * y))
    fmoves.l    fp6,  d1    ; d1 = tex_v
    and.l       d4,   d1    ; d1 = tex_v & (TEXTURE_SIZE - 1)
    ; calculate texel address
    lsl.l       d5,   d1
    or.l        d1,   d0    ; d0 = tex_v << 7 | tex_u
    ; plot
    move.l      (a2, d0.l * 4), (a1)+

    dbnz d3, .xloop
    dbnz d2, .yloop
    rts

; handlers
on_key_down: ; a0 contains display context
    bclr.w     #DISPLAY_BIT_EVENT_LOOP_RUNNING, DISPLAY_REG_FLAGS(a0)
    rts

; data
    @align  0, 8
.display_properties:
    ; view width, height
    dc.w 640, 480

    ; buffer width, height, offsets
    dc.w 640, 480, 0, 0

    ; flags
    dc.w [1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES]
    ; format, target refresh Hz
    dc.b PXL_ARGB, 60

.texel_data:
    @incbin "../res/img.bin"

.filth:
    dc.w    0, 80                 ; beam position 0, 80
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value
    dc.b    DISPLAY_FC_WAIT       ; wait
    dc.w    0, 160                ; beam position
    dc.b    DISPLAY_FC_SET_VIEW_X ; set view x offset
    dc.w    0                     ; x offset value
    ;dc.b    DISPLAY_FC_ADD_WORD
    ;dc.w    2, 1                  ; increment word at offset 2 (the uppser beam y offset value)
    dc.b    DISPLAY_FC_ADD_WORD
    dc.w    5, 2                  ; increment word at offset 5 (the x offset value)
    ;dc.b    DISPLAY_FC_SUB_WORD
    ;dc.w    10, 1                 ; increment word at offset 9 (the lower beam y offset value)
    dc.b    DISPLAY_FC_END        ; end

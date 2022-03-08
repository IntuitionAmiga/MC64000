
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

    @def STEP         #0.01
    @def ZOOM         #1.0
    @def DISTANCE     #0.5
    @def TEXTURE_SIZE #128

main:
    hcf     io_init
    hcf     display_init

    ; try to open the display
    move.q  .display_properties, r0
    hcf     display_open
    biz.q   a0, exit  ; no display?
    move.q  a0, -(sp) ; save the context on the stack

    ; Populate the callback handlers
    lea     on_frame,    DISPLAY_REG_CALL_FRAME(a0)
    lea     on_key_down, DISPLAY_REG_CALL_KEY_PRESS(a0)

    ; Set parameters
    lea     .texel_data, a2
    fmove.s STEP,       fp15
    fmove.s ZOOM,       fp14
    fmove.s DISTANCE,   fp13
    fmove.s #0.0,       fp12 ; current angle

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
    ; a0 contains display context

    fmove.s     fp12, fp10
    fsincos.s   fp10, fp11 ; fp10 = sin(angle), fp11 = cos(angle)

    fadd.s      fp15, fp12 ; angle += step

    fmove.s     fp14, fp9
    fmul.s      fp10, fp9
    fadd.s      fp13, fp9 ; fp9 = scale = (fsin * zoom) + dist

    ; set address and counters
    move.q      DISPLAY_REG_SOFT_BUFFER_ADDRESS(a0), a1
    clr.q       d2
    clr.q       d3
    move.w      DISPLAY_REG_HEIGHT(a0), d2

    move.l      #127, d4
    move.l      #7,   d5

.yloop:
    move.w      DISPLAY_REG_WIDTH(a0), d3
    fmovel.s    d2, fp8 ; fp8 = y
.xloop:
    fmovel.s    d3, fp7 ; fp7 = x

    ; calculate texel coordinates
    fmove.s     fp11, fp6
    fmul.s      fp7,  fp6 ; fp6 = (fcos * x)
    fmove.s     fp10, fp5
    fmul.s      fp8,  fp5 ; fp5 = (fsin * y)
    fsub.s      fp5,  fp6 ; fp6 = (fcos * x) - (fsin * y)
    fmul.s      fp9,  fp6 ; fp6 = scale * ((fcos * x) - (fsin * y))
    fmoves.l    fp6,  d0  ; d0 = tex_u
    and.l       d4,   d0  ; d0 = tex_u & (TEXTURE_SIZE - 1)

    fmove.s     fp10, fp6
    fmul.s      fp7,  fp6 ; fp6 = (fsin * x)
    fmove.s     fp11, fp5
    fmul.s      fp8,  fp5 ; fp5 = (fcos * y)
    fadd.s      fp5,  fp6 ; fp6 = (fsin * x) + (fcos * y)
    fmul.s      fp9,  fp6 ; fp6 = scale * ((fsin * x) + (fcos * y))
    fmoves.l    fp6,  d1  ; d1 = tex_v
    and.l       d4,   d1  ; d1 = tex_v & (TEXTURE_SIZE - 1)

    lsl.l       d5,   d1
    or.l        d1,   d0  ; d0 = tex_v << 7 | tex_u

    move.l      (a2, d0.l * 4),   (a1)+

    dbnz d3, .xloop
    dbnz d2, .yloop
    rts

on_key_down:
    bclr.w     #DISPLAY_BIT_EVENT_LOOP_RUNNING, DISPLAY_REG_FLAGS(a0)
    rts


    @align  0, 8
.display_properties:
    ; width, height, flags
    dc.w 640, 480, 0x000F
    ; format, target refresh Hz
    dc.b PXL_ARGB, 30

.texel_data:
    @incbin "../res/img.bin"

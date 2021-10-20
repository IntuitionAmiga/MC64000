
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
;


    @align 0, 8
.f15:
    dc.s #0.0
.f14:
    dc.s #0.0
.f13:
    dc.s #0.0
.f12:
    dc.s #0.0
.f11:
    dc.s #0.0
.f10:
    dc.s #0.0
.f9:
    dc.s #0.0
.f8:
    dc.s #0.0
.f7:
    dc.s #0.0
.f6:
    dc.s #0.0
.f5:
    dc.s #0.0

calculate:

; calculate ranges
    ; .f15 fMinReal | fMinImaginary
    ; .f14 fStep
    ; .f13 fBailout
    ; .f12 fMaxImaginary

    fmove.s     #MAX_ORDINATE, .f14
    fmove.s     #BAILOUT,      .f13
    fmove.s     .f14,   .f12        ; fCurrImaginary = fMaxImaginary
    fneg.s      .f14,   .f15        ; fMinImaginary  = -fMaxImaginary
    fadd.s      .f14,   .f14        ; fRange         = 2*fMaxImaginary
    move.l      #1,     d6
    lsl.l       #IMAGE_SIZE_DIM, d6 ; iHeight = iWidth = (1 << IMAGE_SIZE_DIM)
    fmovel.s    d6,     .f11        ; fWidth  = (float) iWidth
    fdiv.s      .f11,   .f14        ; fStep   = fRange / fWidth
    move.l      d6,     d5          ; iY      = iHeight
    move.l      #MAX_ITERATION, d3  ;
    fadd.s      #REAL_OFFSET, .f15  ; adjust the real coordinate to better centre the set

    ; .f12 fCurrImaginary
    ; d7   num pixels
    ; d6   edge size (width/height)
    ; d5   height countdown
    ; d3   max iteration
.yloop:

    ; .f11 fCurrReal

    fmove.s     .f15,   .f11        ; fCurrReal = fMinR
    move.l      d6,     d4          ; iX = iWidth

    ; .f10 fZImaginary
    ; .f9  fZReal
    ; .f8  fZImaginarySquared | fZRealSquared + fZImaginarySquared (arg length squared)
    ; .f7  fZRealSquared
    ; .f6  fNewZReal

    ; d4   width countdown
.xloop:
    fmove.s     .f12,   .f10        ; fZImaginary = fCurrImaginary
    fmove.s     .f11,   .f9         ; fZReal      = fCurrReal
    move.l      d3,     d2

    ; d2   iteration countdown
.iteration:

    fmove.s     .f10,   .f8
    fmul.s      .f8,    .f8         ; fZImaginarySqared = fZImaginary*fZImaginary
    fmove.s     .f9,    .f7
    fmul.s      .f7,    .f7         ; fZRealSquares = fZReal * fZReal
    fmove.s     .f11,   .f6         ; fNewZReal     = fCurrReal
    fadd.s      .f7,    .f6         ; fNewZReal    += fZRSquared
    fsub.s      .f8,    .f6         ; fNewZReal    -= fZISquared
    fadd.s      .f7,    .f8         ; fZRealSquared + fZImaginarySquared (for bailout test)

    ; .f5 fTemp

    fmove.s     .f9,    .f5         ; fTemp = fZReal
    fmul.s      .f10,   .f5         ; fTemp = fZReal * fZImaginary
    fadd.s      .f5,    .f5         ; fTemp = 2 * fZReal * fZImaginary
    fadd.s      .f12,   .f5
    fmove.s     .f5,   .f10         ; fZImaginary = 2 * fZReal * fZImaginay + fCurrImaginary
    fmove.s     .f6,    .f9         ; fZReal      = fNewZReal
    fbgt.s      .f8,    .f13, .bailout
    dbnz        d2,     .iteration

.bailout:
    fadd.s      .f14,   .f11        ; fCurrReal += fStep

    ; Calculate the iteration count and square for better gradients
    neg.l       d2,     d2
    add.l       d3,     d2          ; iteration count = max iterations - iteration countdown
    mulu.w      d2,     d2
    move.b      d2,     (a0)+

    dbnz        d4,     .xloop      ; while --iX
    fsub.s      .f14,   .f12        ; fCurrImaginary -= fStep
    dbnz        d5,     .yloop      ; while --iY

    rts

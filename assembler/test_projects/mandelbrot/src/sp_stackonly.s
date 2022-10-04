
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
calculate:

    lea      -64(sp), a5

; calculate ranges
    ; 40(a5) fMinReal | fMinImaginary
    ; 36(a5) fStep
    ; 32(a5) fBailout
    ; 28(a5) fMaxImaginary

    fmove.s     #MAX_ORDINATE,  36(a5)
    fmove.s     #BAILOUT,       32(a5)
    fmove.s     36(a5), 28(a5)        ; fCurrImaginary = fMaxImaginary
    fneg.s      36(a5), 40(a5)        ; fMinImaginary  = -fMaxImaginary
    fadd.s      36(a5), 36(a5)        ; fRange         = 2*fMaxImaginary
    move.l      #1,     d6
    lsl.l       #IMAGE_SIZE_DIM, d6 ; iHeight = iWidth = (1 << IMAGE_SIZE_DIM)
    fmovel.s    d6,     24(a5)        ; fWidth  = (float) iWidth
    fdiv.s      24(a5), 36(a5)        ; fStep   = fRange / fWidth
    move.l      d6,     d5          ; iY      = iHeight
    move.l      #MAX_ITERATION, d3  ;
    fadd.s      #REAL_OFFSET,   40(a5)  ; adjust the real coordinate to better centre the set


    ; 28(a5) fCurrImaginary
    ; d7   num pixels
    ; d6   edge size (width/height)
    ; d5   height countdown
    ; d3   max iteration
.yloop:

    ; 24(a5) fCurrReal

    fmove.s     40(a5), 24(a5)        ; fCurrReal = fMinR
    move.l      d6,     d4          ; iX = iWidth

    ; 20(a5)  fZImaginary
    ; 16(a5)  fZReal
    ; 12(a5)  fZImaginarySquared | fZRealSquared + fZImaginarySquared (arg length squared)
    ;  8(a5)  fZRealSquared
    ;  4(a5)  fNewZReal

    ; d4   width countdown
.xloop:
    fmove.s     28(a5), 20(a5)        ; fZImaginary = fCurrImaginary
    fmove.s     24(a5), 16(a5)         ; fZReal      = fCurrReal
    move.l      d3,     d2

    ; d2   iteration countdown
.iteration:

    fmove.s     20(a5), 12(a5)
    fmul.s      12(a5), 12(a5)         ; fZImaginarySqared = fZImaginary*fZImaginary
    fmove.s     16(a5),  8(a5)
    fmul.s       8(a5),  8(a5)         ; fZRealSquares = fZReal * fZReal
    fmove.s     24(a5),  4(a5)         ; fNewZReal     = fCurrReal
    fadd.s       8(a5),  4(a5)         ; fNewZReal    += fZRSquared
    fsub.s      12(a5),  4(a5)         ; fNewZReal    -= fZISquared
    fadd.s       8(a5), 12(a5)         ; fZRealSquared + fZImaginarySquared (for bailout test)

    ; (a5) fTemp

    fmove.s     16(a5),   (a5)         ; fTemp = fZReal
    fmul.s      20(a5),   (a5)         ; fTemp = fZReal * fZImaginary
    fadd.s        (a5),   (a5)         ; fTemp = 2 * fZReal * fZImaginary
    fadd.s      28(a5),   (a5)
    fmove.s       (a5), 20(a5)         ; fZImaginary = 2 * fZReal * fZImaginay + fCurrImaginary
    fmove.s      4(a5), 16(a5)         ; fZReal      = fNewZReal
    fbgt.s      12(a5), 32(a5), .bailout
    dbnz        d2,     .iteration

.bailout:
    fadd.s      36(a5), 24(a5)        ; fCurrReal += fStep

    ; Calculate the iteration count and square for better gradients
    neg.l       d2,     d2
    add.l       d3,     d2          ; iteration count = max iterations - iteration countdown
    mulu.l      d2,     d2
    move.b      d2,     (a0)+

    dbnz        d4,     .xloop      ; while --iX
    fsub.s      36(a5),  28(a5)        ; fCurrImaginary -= fStep
    dbnz        d5,     .yloop      ; while --iY

    rts

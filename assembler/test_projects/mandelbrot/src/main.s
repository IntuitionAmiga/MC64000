
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
;   Mandelbrot test. This is not intended to be an efficient implementation,
;   rather it is designed to measure execution throughput for a compute
;   intensive workload.

    @def IMAGE_SIZE_DIM 10      ; Edge size, as a power of 2, e.g. 1024
    @def IMAGE_AREA_DIM 20      ; Area size for square output
    @def MAX_ORDINATE   1.6     ; imaginary axis extents
    @def REAL_OFFSET    -0.75   ; adjustment along real axis for better centering
    @def MAX_ITERATION  256     ; maximum iterations per pixel
    @def BAILOUT        16.0    ; bailout value, > 4 chosen for better bifurcations

    @export main x
main:
    ; a6 : pixel buffer
    ; a5 : output file handle
    ; d7 : total number of pixels
    move.q  #1, d7
    lsl.q   #IMAGE_AREA_DIM, d7

; allocate image buffer
; strictly, we could just directly output each pixel but it's nice to test allocation routines too.
    move.q  d7, d0
    bsr     mem_alloc
    bnz.q   a0, .allocated
    lea     .err_no_memory_1, a0
    bsr     io_print_string
    move.q  d7, d0
    bsr     io_print_quad
    lea     .err_no_memory_2, a0
    bsr     io_print_string
    bra     exit

.allocated:
    ; save the buffer handle in a6
    move.q  a0, a6

; calculate ranges
    ; fp15 fMinReal | fMinImaginary
    ; fp14 fStep
    ; fp13 fBailout
    ; fp12 fMaxImaginary

    fmove.s     #MAX_ORDINATE, fp14
    fmove.s     #BAILOUT,      fp13
    fmove.s     fp14,   fp12        ; fCurrImaginary = fMaxImaginary
    fneg.s      fp14,   fp15        ; fMinImaginary  = -fMaxImaginary
    fadd.s      fp14,   fp14        ; fRange         = 2*fMaxImaginary
    move.l      #1,     d6
    lsl.l       #IMAGE_SIZE_DIM, d6 ; iHeight = iWidth = (1 << IMAGE_SIZE_DIM)
    fmovel.s    d6,     fp11        ; fWidth  = (float) iWidth
    fdiv.s      fp11,   fp14        ; fStep   = fRange / fWidth
    move.l      d6,     d5          ; iY      = iHeight
    move.l      #MAX_ITERATION, d3  ;
    fadd.s      #REAL_OFFSET, fp15  ; adjust the real coordinate to better centre the set

    ; fp12 fCurrImaginary
    ; d7   num pixels
    ; d6   edge size (width/height)
    ; d5   height countdown
    ; d3   max iteration
.yloop:

    ; fp11 fCurrReal

    fmove.s     fp15,   fp11        ; fCurrReal = fMinR
    move.l      d6,     d4          ; iX = iWidth

    ; fp10 fZImaginary
    ; fp9  fZReal
    ; fp8  fZImaginarySquared | fZRealSquared + fZImaginarySquared (arg length squared)
    ; fp7  fZRealSquared
    ; fp6  fNewZReal

    ; d4   width countdown
.xloop:
    fmove.s     fp12,   fp10        ; fZImaginary = fCurrImaginary
    fmove.s     fp11,   fp9         ; fZReal      = fCurrReal
    move.l      d3,     d2

    ; d2   iteration countdown
.iteration:

    fmove.s     fp10,   fp8
    fmul.s      fp8,    fp8         ; fZImaginarySqared = fZImaginary*fZImaginary
    fmove.s     fp9,    fp7
    fmul.s      fp7,    fp7         ; fZRealSquares = fZReal * fZReal
    fmove.s     fp11,   fp6         ; fNewZReal     = fCurrReal
    fadd.s      fp7,    fp6         ; fNewZReal    += fZRSquared
    fsub.s      fp8,    fp6         ; fNewZReak    -= fZISquared
    fadd.s      fp7,    fp8         ; fZRealSquared + fZImaginarySquared (for bailout test)

    ; fp5 fTemp

    fmove.s     fp9,    fp5         ; fTemp = fZReal
    fmul.s      fp10,   fp5         ; fTemp = fZReal * fZImaginary
    fadd.s      fp5,    fp5         ; fTemp = 2 * fZReal * fZImaginary
    fadd.s      fp12,   fp5
    fmove.s     fp5,   fp10         ; fZImaginary = 2 * fZReal * fZImaginay + fCurrImaginary
    fmove.s     fp6,    fp9         ; fZReal      = fNewZReal
    fbgt.s      fp8,    fp13, .bailout
    dbnz        d2,     .iteration

.bailout:
    fadd.s      fp14,   fp11        ; fCurrReal += fStep

    ; Calculate the iteration count and square for better gradients
    neg.l       d2,     d2
    add.l       d3,     d2          ; iteration count = max iterations - iteration countdown
    mulu.w      d2,     d2
    move.b      d2,     (a0)+

    dbnz        d4,     .xloop      ; while --iX
    fsub.s      fp14,   fp12        ; fCurrImaginary -= fStep
    dbnz        d5,     .yloop      ; while --iY

; open the output file
    lea         .file_name, a0
    move.b      #IO_OPEN_WRITE, d0
    bsr         io_file_open
    bnz.q       a0, .opened
    lea         .err_file_open_1, a0
    bsr         io_print_string
    lea         .file_name, a0
    bsr         io_print_string
    lea         .err_file_open_2, a0
    bsr         io_print_string
    bra         exit

.opened:
    ; save the file handle in a5
    move.q      a0, a5

.save_file:
    ; write the PGM header
    lea         .pgm_int_fmt, a0
    bsr         io_set_fmt_long
    move.q      a5,     a0
    lea         .pgm_head, a1
    bsr         io_file_print_string
    move.l      #1,     d0
    lsl.l       #IMAGE_SIZE_DIM, d0
    bsr         io_file_print_long
    bsr         io_file_print_long
    move.l      #255,   d0
    bsr         io_file_print_long

    ; write the block data
    move.q      a6,     a1
    move.q      d7,     d0
    bsr         io_file_write

    @export exit x
exit:
    ; free buffer (null safe)
    move.q      a6,     a0
    bsr         mem_free

    ; close file (null safe)
    move.q      a5,     a0
    bsr         io_file_close
    rts

.file_name:
    dc.b "mandelbrot.pgm\0"
.pgm_head:
    dc.b "P5\n\0"
.pgm_int_fmt:
    dc.b "%d\n\0"

.err_no_memory_1:
    dc.b "Error: Couldn't allocate \0"
.err_no_memory_2:
    dc.b " bytes of memory for buffer.\n\0"

.err_file_open_1:
    dc.b "Error: Couldn't open \0"
.err_file_open_2:
    dc.b " for writing.\n\0"

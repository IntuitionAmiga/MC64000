
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

    @def IMAGE_SIZE_DIM 10
    @def IMAGE_AREA_DIM 20
    @def MAX_ORDINATE   2.0
    @def MAX_ITERATION  256
    @def BAILOUT        16.0

    @export main x
main:
; a6 : pixel buffer
; a5 : output file handle
; d7 : num pixels

    ; save the pixel count in d7
    move.q  #1, d7
    lsl.q   #IMAGE_AREA_DIM, d7

; allocate image buffer
    move.q  d7, d0
    bsr     mem_alloc
    bnz.q   a0, .allocated
    lea     err_no_memory_1, a0
    bsr     io_print_string
    move.q  d7, d0
    bsr     io_print_quad
    lea     err_no_memory_2, a0
    bsr     io_print_string
    bra     exit

.allocated:
    ; save the buffer handle in a6
    move.q  a0, a6


; calculate ranges
    ; min X/Y  in fp15
    ; max X/Y  in fp14
    ; step X/Y in fp13
    fmove.s     #MAX_ORDINATE, fp14
    fneg.s      fp14, fp15
    fmove.s     fp14, fp13
    fadd.s      fp13, fp13
    move.l      #1,   d6
    lsl.l       #IMAGE_SIZE_DIM, d6
    fmovel.s    d6, fp0
    fdiv.s      fp0, fp13

    move.l      d6,  d5
.yloop:
    ; cy in fp14
    fmove.s     fp15, fp12
    move.l      d6,  d4

.xloop:
    ; cx in fp12
    fadd.s      fp13, fp12

    ; filler for now
    move.b      #0xCC, (a0)+

    dbnz        d4, .xloop
    fsub.s      fp13, fp14
    dbnz        d5, .yloop

; open the output file
    lea         file_name, a0
    move.b      #IO_OPEN_WRITE, d0
    bsr         io_file_open
    bnz.q       a0, .opened
    lea         err_file_open_1, a0
    bsr         io_print_string
    lea         file_name, a0
    bsr         io_print_string
    lea         err_file_open_2, a0
    bsr         io_print_string
    bra         exit

.opened:
    ; save the file handle in a5
    move.q      a0, a5

.save_file:
    ; write the PGM header
    lea         pgm_int_fmt, a0
    bsr         io_set_fmt_long
    move.q      a5, a0
    lea         pgm_head, a1
    bsr         io_file_print_string
    move.l      #1, d0
    lsl.l       #IMAGE_SIZE_DIM, d0
    bsr         io_file_print_long
    bsr         io_file_print_long
    move.l      #255, d0
    bsr         io_file_print_long

    ; write the block data
    move.q      a6, a1
    move.q      d7, d0
    bsr         io_file_write

    @export exit x
exit:
    move.q      a6, a0
    bsr         mem_free
    move.q      a5, a0
    bsr         io_file_close
    rts

file_name:
    dc.b "mandelbrot.pgm\0"
pgm_head:
    dc.b "P5\n\0"
pgm_int_fmt:
    dc.b "%d\n\0"

err_no_memory_1:
    dc.b "Error: Couldn't allocate \0"
err_no_memory_2:
    dc.b " bytes of memory for buffer.\n\0"

err_file_open_1:
    dc.b "Error: Couldn't open \0"
err_file_open_2:
    dc.b " for writing.\n\0"

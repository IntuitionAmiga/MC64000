
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
; Empty project - main.s

    @def nanotime dc.b 0xF0 ; super undocumented opcodes ftw

    @def MEM_BUFFER_SIZE 1 << 28
    @def LOOP_COUNT      1 << 8

    @def TOTAL_BYTES MEM_BUFFER_SIZE * LOOP_COUNT
    @def TOTAL_GIGABYTES TOTAL_BYTES >> 30

main:
    hcf io_init
    hcf mem_init

    move.q  #MEM_BUFFER_SIZE, d2
    move.q  d2, d0
    hcf     mem_alloc
    biz.q   a0, .bad_alloc
    move.q  a0, a2

    lea     .txt_memory_1, a0
    hcf     io_print_string
    clr.q   a0
    move.q  d2, d0
    hcf     io_print_quad
    lea     .txt_memory_2, a0
    hcf     io_print_string

    clr.q   d3
    move.q  #LOOP_COUNT, d4
    move.q  #1, d5
    clr.q   r12

.loop:
    move.q  a2, a0
    move.q  d2, d1
    ;lsr.q   #2, d1 ; // longs
    move.b  d3, d0
    nanotime                  ; writes current nanotime to r14
    move.q  r14, r13          ; start
    hcf     mem_fill_byte
    nanotime
    sub.q   r13,  r14         ; elapsed
    add.q   r14,  r12         ; total
    add.q   d5, d3
    dbnz    d4, .loop

    lea     .txt_elapsed_1, a0
    hcf     io_print_string
    clr.q   a0
    move.q  r12, d0
    hcf     io_print_quad
    lea     .txt_elapsed_2, a0
    hcf     io_print_string

    lea     .txt_memory_3, a0
    hcf     io_print_string
    move.q  #TOTAL_GIGABYTES, d0
    clr.q   a0
    hcf     io_print_quad
    lea     .txt_memory_4, a0
    hcf     io_print_string

    fmoveq.d  #TOTAL_GIGABYTES, fp0
    fmoveq.d  r12, fp1
    fmul.d    #1.e-9, fp1 ; nanoseconds to seconds
    fdiv.d    fp1, fp0

    lea     .txt_through_1, a0
    hcf     io_print_string
    clr.q   a0
    hcf     io_print_double
    lea     .txt_through_2, a0
    hcf     io_print_string

    move.q  a2, a0
    hcf     mem_free
    bra     exit

.bad_alloc:
    lea     .txt_err_no_memory, a0
    hcf     io_print_string
    clr.q   a0
    move.q  d2, d0
    hcf     io_print_quad
    lea     .txt_memory_2, a0
    hcf     io_print_string
    bra     exit

exit:
    hcf mem_done
    hcf io_done
    rts

.txt_err_no_memory:
    dc.b "Error: Couldn't allocate \0"
.txt_memory_1:
    dc.b "Allocated \0"
.txt_memory_2:
    dc.b " bytes of memory for buffer.\n\0"
.txt_memory_3:
    dc.b "Total size written \0"
.txt_memory_4:
    dc.b " GiB\n\0"
.txt_elapsed_1:
    dc.b "Took \0"
.txt_elapsed_2:
    dc.b " nanoseconds\n\0"
.txt_through_1:
    dc.b "Throughput \0"
.txt_through_2:
    dc.b "GiB/s\n\0"

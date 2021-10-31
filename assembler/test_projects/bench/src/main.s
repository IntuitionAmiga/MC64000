
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

    @equ nanotime               dc.b 0xF0 ; super undocumented opcodes ftw

    @equ max_loops          #20000000

    @equ loop_counter       r2
    @equ time_recorded      r14
    @equ time_started       r13
    @equ calibration_time   r12
    @equ scale_relative     fp15

main:
    bsr         io_init
    lea         .info_txt_0,        r8
    bsr         io_print_string
    clr.q       r8
    move.q      max_loops,          r0
    bsr         io_print_quad
    lea         .info_txt_1,        r8
    bsr         io_print_string

    bsr         calibrate_loop
    bsr         baseline
    bsr         bench_add_reg_to_reg_ind
    bsr         bench_add_reg_ind_to_reg_ind
    bsr         bench_bsr_ret
    bsr         bench_bsr_b_ret
    bsr         bench_hcf


exit:
    bsr         io_done
    rts

report_elapsed:
    lea         .report_elapsed_txt_0,  r8
    bsr         io_print_string
    move.q      time_recorded,          r0
    lea         .report_elapsed_fmt,    r8
    bsr         io_print_quad
    lea         .report_elapsed_txt_1,  r8
    bsr         io_print_string
    rts

report_relative:
    fmoveq.d    time_recorded,          fp0
    fmul.d      scale_relative,         fp0
    lea         .report_relative_fmt,   r8
    bsr         io_print_double
    lea         .report_relative_txt,   r8
    bsr         io_print_string
    rts

.info_txt_0:
    dc.b "Benchmarking 10x unrolled using \0"
.info_txt_1:
    dc.b " iterations\n\0"
.newline:
    dc.b "\n\0"


.add_ind_dst_txt:
    dc.b "add.q r1, (r10)\n\0"

.add_ind_dst_disp_txt:
    dc.b "add.q r1, 8(r10)\n\0"

.add_ind_src_dst_same_txt:
    dc.b "and.q (r10), (r10)\n\0"

.report_elapsed_txt_0:
    dc.b        "\ttook: \0"
.report_elapsed_txt_1:
    dc.b        " nanoseconds \0"
.report_elapsed_fmt:
    dc.b        "%12ld\0"
.report_relative_txt:
    dc.b        " relative\n\0"
.report_relative_fmt:
    dc.b        "%6.4f\0"

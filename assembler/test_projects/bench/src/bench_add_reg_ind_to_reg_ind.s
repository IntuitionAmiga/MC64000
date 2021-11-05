
    @align  0, 8

bench_add_reg_ind_to_reg_ind:
; Instruction, add.q (r11), (r10)
    lea         .benchmark_info,    r8
    bsr         io_print_string

    ; Allocate a non-adjacent pair of locations on the stack in r10/r11
    lea         -128(sp),            r10
    lea         64(r10),             r11
    clr.q       (r10)
    move.q      #1,                 (r11)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    add.q       (r11),     (r10)
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking add.q (r11), (r10)\n\0"

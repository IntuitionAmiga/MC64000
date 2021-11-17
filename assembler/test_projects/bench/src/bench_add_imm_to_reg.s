
    @align  0, 8
bench_small_imm_to_reg:
    lea         .benchmark_info_0, r8
    hcf         io_print_string
    move.q      #1,     r1
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_0:
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    add.q       #1,     r0
    dbnz        loop_counter,       .benchmark_loop_0
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info_0:
    dc.b "Benchmarking: add.q #1, r0\n\0"


    @align  0, 8

bench_bsr_ret:
    lea         .benchmark_info,    r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.b      #0, -(sp)
    move.q      time_recorded,      time_started
.benchmark_loop:
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    add.q       #1, sp
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
.dummy:
    rts

bench_bsr_b_ret:
    lea         .benchmark_info_2,  r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.b      #0, -(sp)
    move.q      time_recorded,      time_started
.benchmark_loop_2:
    bsr.b       .dummy
    bsr.b       .dummy
    bsr.b       .dummy
    bsr.b       .dummy
    bsr.b       .dummy
    dbnz        loop_counter,       .benchmark_loop_2
    nanotime
    add.q       #1, sp
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking: bsr/ret\n\0"

.benchmark_info_2:
    dc.b "Benchmarking: bsr.b/ret (short negative displacement)\n\0"


    @align  0, 8
bench_hcf:
    lea         .benchmark_info,    r8
    hcf         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    hcf         #0, #0
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking: hcf #0, #0\n\0"


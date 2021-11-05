
    @align  0, 8
bench_hcf:
    lea         .benchmark_info,    r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.b      #0, -(sp)
    move.q      time_recorded,      time_started
.benchmark_loop:
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    hcf         #0
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    add.q       #1, sp
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8
bench_hcf_stub:
    lea         .benchmark_info_1,    r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_1:
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    bsr         io_init
    dbnz        loop_counter,       .benchmark_loop_1
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking: hcf #0\n\0"

.benchmark_info_1:
    dc.b "Benchmarking: stub call to hcf\n\0"

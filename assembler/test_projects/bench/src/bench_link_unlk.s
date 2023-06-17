    @align  0, 8
bench_link_unlk:

    ; benchmarks forward branch/return
    lea         .benchmark_info,    r8
    hcf         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    link        r5, #-64
    unlk        r5

    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
.dummy:
    rts

.benchmark_info:
    dc.b "Benchmarking: link r5, #-64/unlk r5\n\0"

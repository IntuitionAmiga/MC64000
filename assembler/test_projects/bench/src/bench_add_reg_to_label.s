
    @align  0, 8

bench_add_reg_to_label:
    lea         .benchmark_info, r8
    hcf         io_print_string
    move.q      #1,     r1
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    add.q       r1,     .test_label
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative

    rts

    @align      0, 8
.test_label:
    dc.q        0

.benchmark_info:
    dc.b "Benchmarking: add.q r1, label\n\0"

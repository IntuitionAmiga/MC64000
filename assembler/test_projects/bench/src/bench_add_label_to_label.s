
    @align  0, 8

bench_add_label_to_label:

    rts ; todo - debug segfault

    lea         .benchmark_info, r8
    hcf         io_print_string
    ;move.q      #1,                 .test_label_src
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    add.q       .test_label_src,    .test_label_dst
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative

    rts

    @align      0, 8
.test_label_dst:
    dc.q        0
.test_label_src:
    dc.q        1

.benchmark_info:
    dc.b "Benchmarking: add.q label_src, label_dst\n\0"

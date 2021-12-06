
    @align  0, 8

bench_biz_int_taken:
    lea         .benchmark_info,    r8
    hcf         io_print_string

    move.q      max_loops,          loop_counter
    clr.q       r0
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    biz.q       r0, .next1
    rts

.next1:
    biz.q       r0, .next2
    rts

.next2:
    biz.q       r0, .next3
    rts

.next3:
    biz.q       r0, .next4
    rts

.next4:
    biz.q       r0, .next5
    rts

.next5:
    biz.q       r0, .next6
    rts

.next6:
    biz.q       r0, .next7
    rts

.next7:
    biz.q       r0, .next8
    rts

.next8:
    biz.q       r0, .next9
    rts

.next9:
    biz.q       r0, .next10
    rts

.next10:
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8

bench_biz_int_not_taken:
    lea         .benchmark_info_1,    r8
    hcf         io_print_string

    move.q      max_loops,          loop_counter
    move.q      #1,                 r0
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_1:
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    biz.q       r0, .not_taken
    dbnz        loop_counter,       .benchmark_loop_1
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
.not_taken:
    rts

    @align  0, 8

.benchmark_info:
    dc.b "Benchmarking: biz.q r0, label (when taken)\n\0"

.benchmark_info_1:
    dc.b "Benchmarking: biz.q r0, label (when not taken)\n\0"

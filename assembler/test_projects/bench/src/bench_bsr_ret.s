    @align  0, 8
bench_bsr_ret:
    bsr .bench_bsr_ret_fwd
    bsr .bench_bsr_ret_misaligned_fwd
    bsr .bench_bsr_b_ret_rev
    bsr .bench_bsr_b_ret_misaligned_rev
    rts

    @align  0, 8
.bench_bsr_ret_fwd:
    ; benchmarks forward branch/return
    lea         .benchmark_info,    r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    bsr         .dummy
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
.dummy:
    rts

    @align  0, 8
.bench_bsr_ret_misaligned_fwd:
    ; benchmarks forward branch/return where the stack is misalgined
    lea         .benchmark_info_1,  r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    sub.q       #7, sp
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_1:
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    bsr         .dummy_1
    dbnz        loop_counter,       .benchmark_loop_1
    nanotime
    add.q       #7, sp
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
.dummy_1:
    rts

    @align  0, 8

.bench_bsr_b_ret_rev:
    ; benchmarks reverse short branch/return
    lea         .benchmark_info_2,  r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_2:
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    dbnz        loop_counter,       .benchmark_loop_2
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8
.bench_bsr_b_ret_misaligned_rev:
    ; benchmarks reverse short branch/return where the stack is misaligned
    lea         .benchmark_info_3,  r8
    bsr         io_print_string

    move.q      max_loops,          loop_counter
    sub.q       #7, sp
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_3:
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    bsr.b       .dummy_1
    dbnz        loop_counter,       .benchmark_loop_3
    nanotime
    add.q       #7, sp
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking: bsr/ret\n\0"

.benchmark_info_1:
    dc.b "Benchmarking: bsr/ret (stack misaligned)\n\0"

.benchmark_info_2:
    dc.b "Benchmarking: bsr.b/ret (short negative displacement)\n\0"

.benchmark_info_3:
    dc.b "Benchmarking: bsr.b/ret (short negative displacement, stack misaligned)\n\0"

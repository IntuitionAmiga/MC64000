
    @align  0, 8

bench_add_reg_to_reg_ind:
; Instruction, add.q r1, (r10)
    lea         .benchmark_info, r8
    bsr         io_print_string
    move.q      #1,     r1
    lea         -64(sp), r10
    clr.q       (r10)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    add.q       r1,     (r10)
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative

    rts

.benchmark_info:
    dc.b "Benchmarking: add.q r1, (r10)\n\0"

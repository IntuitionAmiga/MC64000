
    @align  0, 8

.add_vec3f:
    fadd.s  (r8),  (r9)
    fadd.s  4(r8), 4(r9)
    fadd.s  8(r8), 8(r9)
    rts


    @align  0, 8
bench_add_vec3f:
    lea         .benchmark_info,    r8
    bsr         io_print_string

    lea         -128(sp),   r10
    lea         12(sp),     r11
    clr.q       (r11)
    clr.q       4(r11)
    clr.q       8(r11)
    fmove.s     #1.0e-6,    fp0
    fmove.s     fp0,        (r10)
    fmove.s     fp0,        4(r10)
    fmove.s     fp0,        8(r10)
    move.q      r10,        r8
    move.q      r11,        r9

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:

    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f
    bsr         .add_vec3f

    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info:
    dc.b "Benchmarking add_vec3f r8, r9\n\0"

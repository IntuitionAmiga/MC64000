
    @align  0, 8
bench_vec3f_func:
    lea         .benchmark_info,    r8
    hcf         io_print_string

    lea         -128(sp),           r8
    lea         12(r8),             r9

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd
    bsr         .vec3fadd

    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8

.vec3fadd:
    fadd.s      (r8), (r9)
    fadd.s      4(r8), 4(r9)
    fadd.s      8(r8), 8(r9)
    rts

.benchmark_info:
    dc.b "Benchmarking: bsr .vec3fadd\n\0"


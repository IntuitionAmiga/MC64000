
    @align  0, 8
bench_vec3f_hcf:
    lea         .benchmark_info,    r8
    hcf         io_print_string

    lea         -128(sp),           r8
    lea         12(r8),             r9

    fmove.s     #1.0, fp0
    hcf         vec3f_splat

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    hcf         vec3f_norm
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8

.benchmark_info:
    dc.b "Benchmarking: vec3f_add_assign\n\0"


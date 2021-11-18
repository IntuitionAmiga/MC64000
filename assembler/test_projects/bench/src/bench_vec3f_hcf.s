
    @align  0, 8
bench_vec3f_hcf:
    lea         .benchmark_info,    r8
    hcf         io_print_string

    lea         -128(sp),           r8
    lea         12(r8),             r9

    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop:
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    vec3f_add_as
    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8

.benchmark_info:
    dc.b "Benchmarking: vec3f_add_as intrinsic\n\0"


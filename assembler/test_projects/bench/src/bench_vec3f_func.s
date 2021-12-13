
    @align  0, 8
bench_vec3f_func:
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
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm
    bsr         .vec3fnorm

    dbnz        loop_counter,       .benchmark_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8

.vec3fnorm:                 ; (r8) = &{ x, y, z }
    fmove.s     (r8)+,  fp1
    fmul.s      fp1,    fp1 ; fp1 = x*x
    fmove.s     (r8)+,  fp0
    fmul.s      fp0,    fp0 ; fp0 = y*y
    fadd.s      fp0,    fp1 ; fp1 = x*x + y*y
    fmove.s     (r8)+,  fp0
    fmul.s      fp0,    fp0 ; fp0 = z*z
    fadd.s      fp0,    fp1 ; fp1 = x*x + y*y + z*z
    fsqrt.s     fp1,    fp1 ; fp1 = sqrt(x*x + y*y + z*z)
    fmove.s     #1.0,   fp0 ;
    fdiv.s      fp1,    fp0 ; fp0 = 1.0 / sqrt(x*x + y*y + z*z)
    fmul.s      fp0,    -(r8)
    fmul.s      fp0,    -(r8)
    fmul.s      fp0,    -(r8)
    rts

.benchmark_info:
    dc.b "Benchmarking: bsr .vec3fnorm\n\0"


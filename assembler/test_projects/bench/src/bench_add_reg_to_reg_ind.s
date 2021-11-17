    @align  0, 8
bench_add_reg_to_indirect:
    bsr     .bench_add_reg_to_reg_ind
    bsr     .bench_add_reg_to_reg_ind_post_mod
    bsr     .bench_add_reg_to_reg_ind_pre_mod
    bsr     .bench_add_reg_to_reg_ind_disp
    rts

    @align  0, 8
.bench_add_reg_to_reg_ind:
    lea         .benchmark_info_0, r8
    hcf         io_print_string
    move.q      #1,     r1
    lea         -64(sp), r10
    clr.q       (r10)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_0:
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
    dbnz        loop_counter,       .benchmark_loop_0
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8
.bench_add_reg_to_reg_ind_post_mod:
    lea         .benchmark_info_1, r8
    hcf         io_print_string
    move.q      #1,     r1
    lea         -64(sp), r10
    clr.q       (r10)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_1:
    add.q       r1,     (r10)+
    add.q       r1,     (r10)-
    add.q       r1,     (r10)+
    add.q       r1,     (r10)-
    add.q       r1,     (r10)+
    add.q       r1,     (r10)-
    add.q       r1,     (r10)+
    add.q       r1,     (r10)-
    add.q       r1,     (r10)+
    add.q       r1,     (r10)-
    dbnz        loop_counter,       .benchmark_loop_1
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8
.bench_add_reg_to_reg_ind_pre_mod:
    lea         .benchmark_info_2, r8
    hcf         io_print_string
    move.q      #1,     r1
    lea         -64(sp), r10
    clr.q       (r10)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_2:
    add.q       r1,     +(r10)
    add.q       r1,     -(r10)
    add.q       r1,     +(r10)
    add.q       r1,     -(r10)
    add.q       r1,     +(r10)
    add.q       r1,     -(r10)
    add.q       r1,     +(r10)
    add.q       r1,     -(r10)
    add.q       r1,     +(r10)
    add.q       r1,     -(r10)
    dbnz        loop_counter,       .benchmark_loop_2
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

    @align  0, 8
.bench_add_reg_to_reg_ind_disp:
    lea         .benchmark_info_3, r8
    hcf         io_print_string
    move.q      #1,     r1
    lea         -64(sp), r10
    clr.q       (r10)
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.benchmark_loop_3:
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    add.q       r1,     8(r10)
    dbnz        loop_counter,       .benchmark_loop_3
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed
    bsr         report_relative
    rts

.benchmark_info_0:
    dc.b "Benchmarking: add.q r1, (r10)\n\0"

.benchmark_info_1:
    dc.b "Benchmarking: add.q r1, (r10)+/-\n\0"

.benchmark_info_2:
    dc.b "Benchmarking: add.q r1, +/-(r10)\n\0"

.benchmark_info_3:
    dc.b "Benchmarking: add.q r1, 8(r10)\n\0"

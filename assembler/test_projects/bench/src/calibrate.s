    @align      0,8
calibration:
    ; calibration loop is not unrolled
    move.q      max_loops,          r0
    muls.q      #1000,              r0
    fmoveq.d    r0,                 scale_mips

    bsr         .calibrate_loop

    ; all test loops are unrolled
    move.q      max_loops,          r0
    muls.q      loop_scale,         r0
    fmoveq.d    r0,                 scale_mips

    bsr         .baseline

    rts

    @align      0,8
.calibrate_loop:
    ; Calibration time...
    lea         .calibration_txt,   r8
    bsr         io_print_string
    move.q      max_loops,          loop_counter
    nanotime
    move.q      time_recorded,      time_started
.calibration_loop:
    dbnz        loop_counter,       .calibration_loop
    nanotime
    sub.q       time_started,       time_recorded
    move.q      time_recorded,      calibration_time
    bsr         report_elapsed
    lea         .newline,           r8
    bsr         io_print_string
    rts

    @align      0,8
.baseline:
; Reference instruction, add.q r1, r0
    lea         .baseline_txt,  r8
    bsr         io_print_string
    move.q      #1,             r1
    clr.q       r0
    move.q      max_loops,      loop_counter
    nanotime
    move.q      time_recorded,  time_started
.baseline_loop:
    add.q       r1,     r0  ; 01
    add.q       r1,     r0  ; 02
    add.q       r1,     r0  ; 03
    add.q       r1,     r0  ; 04
    add.q       r1,     r0  ; 05
    add.q       r1,     r0  ; 06
    add.q       r1,     r0  ; 07
    add.q       r1,     r0  ; 08
    add.q       r1,     r0  ; 09
    add.q       r1,     r0  ; 10
    dbnz        r2,     .baseline_loop
    nanotime
    sub.q       calibration_time,   time_recorded
    sub.q       time_started,       time_recorded
    bsr         report_elapsed

    fmoveq.d    time_recorded,      fp14
    fmoveb.d    #1,                 scale_relative
    fdiv.d      fp14,               scale_relative
    bsr         report_relative
    rts

.calibration_txt:
    dc.b "Loop Calibration\n\0"
.newline:
    dc.b "\n\0"
.baseline_txt:
    dc.b "Baseline: add.q r1, r0\n\0"



    @def    MIN_DIFF 1.0e-12

main:
    hcf         io_init

    bsr         vec2f_tests
    bsr         mat2x2f_tests
    bsr         mat3x3f_tests
    bsr         mat4x4f_tests

    bsr         mat2x2d_tests
    bsr         mat3x3d_tests
    bsr         mat4x4d_tests

exit:
    hcf         io_done
    rts

; Compare (r8)+, (r9)+ as singles, count in r0
verify_array_singles:
    fmove.s     #MIN_DIFF, fp1
.verify_single_loop:
    fmove.s     (r8)+, fp0
    fsub.s      (r9)+, fp0
    fabs.s      fp0,   fp0
    fbgt.s      fp0,   fp1, .verify_single_fail
    dbnz        r0,    .verify_single_loop
    move.b      #1,     r0
    rts
.verify_single_fail:
    move.b      #0,     r0
    rts

verify_array_doubles:
    fmove.d     #MIN_DIFF, fp1
.verify_double_loop:
    fmove.d     (r8)+, fp0
    fsub.d      (r9)+, fp0
    fabs.d      fp0,   fp0
    fbgt.d      fp0,   fp1, .verify_double_fail
    dbnz        r0,    .verify_double_loop
    move.b      #1,     r0
    rts
.verify_double_fail:
    move.b      #0,     r0
    rts

report_result:
    lea         .txt_fail, r8
    biz.b        r0, .has_failed
    lea         .txt_pass, r8
.has_failed:
    hcf         io_print_string
    rts

.txt_pass:
    dc.b "\x1b[32\x3b1mPassed.\x1b[0m\n\0"
.txt_fail:
    dc.b "\x1b[31\x3b1mFailed!\x1b[0m\n\0"

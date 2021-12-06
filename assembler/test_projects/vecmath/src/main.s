
    @def    MIN_DIFF 1.0e-20

main:
    hcf         io_init

    bsr         vec2f_tests

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
    rts

report_result:
    lea         .txt_fail, r8
    biz.b        r0, .has_failed
    lea         .txt_pass, r8
.has_failed:
    hcf         io_print_string
    rts

.txt_pass:
    dc.b "Passed.\n\0"
.txt_fail:
    dc.b "Failed.\n\0"

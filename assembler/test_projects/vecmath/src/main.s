
main:
    hcf         io_init

    bsr         vec2f_tests

exit:
    hcf         io_done
    rts

verify_array_singles:

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

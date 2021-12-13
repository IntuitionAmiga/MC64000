
vec2f_tests:
    lea     .test_group_header,    r8
    hcf     io_print_string
    bsr     .test_splat
    bsr     .test_copy
    bsr     .test_add_assign
    rts

.test_splat:
    lea     .test_splat_header,    r8
    hcf     io_print_string
    lea     -128(sp), r8
    fmove.s #0.5, fp0
    hcf     vec2f_splat
    lea     .vec2_test_data_0, r9
    move.q  #2,     r0
    bsr     verify_array_singles
    bsr     report_result
    rts

.test_copy:
    lea     .test_copy_header,    r8
    hcf     io_print_string
    lea     -128(sp), r9
    lea     .vec2_test_data_2, r8
    hcf     vec2f_copy
    move.q  #2,     r0
    bsr     verify_array_singles
    bsr     report_result
    rts


.test_add_assign:
    lea     .test_add_assign_header,    r8
    hcf     io_print_string
    lea     -128(sp), r9
    lea     .vec2_test_data_0, r8
    move.q  (r8), (r9)
    lea     .vec2_test_data_1, r8
    hcf     vec2f_add_assign
    lea     .vec2_test_data_2, r8
    move.q  #2,     r0
    bsr     verify_array_singles
    bsr     report_result

    rts

    @align 0, 8
.vec2_test_data_0:
    dc.s    0.5, 0.5

.vec2_test_data_1:
    dc.s    1.0, 2.0

.vec2_test_data_2:
    dc.s    1.5, 2.5

.test_group_header:
    dc.b    "Testing vec2f operations...\n\0"

.test_splat_header:
    dc.b    "\tvec2f_splat: \0"

.test_copy_header:
    dc.b    "\tvec2f_copy: \0"

.test_add_assign_header:
    dc.b    "\tvec2f_add_assign: \0"

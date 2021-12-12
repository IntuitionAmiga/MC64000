
vec2f_tests:
    lea     .test_group_header,    r8
    hcf     io_print_string

    bsr     .test_splat

    rts

.test_splat:
    lea     -16(sp), r8
    fmove.s #0.5, fp0
    hcf     vec2f_splat
    lea     .vec2_test_data, r9
    move.q  (r9), 8(r8)

;    move.q  #2,     r0
;    bsr     verify_array_singles
;    bsr     report_result

    rts

    @align 0, 8
.vec2_test_data:
    dc.b    1,2,3,4,5,6,7,8
    dc.s    #0.5, #0.5
    dc.s    #1.0, #2.0
    dc.s    #1.5, #2.5

.test_group_header:
    dc.b    "Testing vec2f operations...\n\0"

.test_splat_header:
    dc.b    "Testing vec2f_splat...\0"



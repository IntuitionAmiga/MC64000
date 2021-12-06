
vec2f_tests:
    lea     .test_group_header,    r8
    hcf     io_print_string

    bsr     .test_splat

    rts

.test_splat:
    lea     -8(sp), r8
    fmove.s #0.5, fp0
    hcf     vec2f_splat



    rts

.vec2_test_data:
    dc.s    #0.5, #0.5
    dc.s    #1.0, #2.0
    dc.s    #1.5, #2.5

.test_group_header:
    dc.b    "Testing vec2f operations...\n\0"

.test_splat_header:
    dc.b    "Testing vec2f_splat...\0"



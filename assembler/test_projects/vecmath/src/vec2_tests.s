
vec2f_tests:
    lea     .test_vec2f_header,    r8
    hcf     io_print_string
    move.b  #1, r0
    rts

.test_vec2f_header:
    dc.b "Testing vec2f operations\n\0"


    @def VECTOR_SIZE 2
    @def SIZE_BYTES 16
    @def STACK_BUFFER -24(sp) ; Room for one vector and the function return address

vec2d_tests:
    lea     .test_group_header, r8
    hcf     io_print_string
    bsr     .test_splat
    bsr     .test_copy
    bsr     .test_add_assign
    bsr     .test_add
    bsr     .test_sub_assign
    bsr     .test_sub
    bsr     .test_dot
    bsr     .test_magn
    bsr     .test_norm_assign
    bsr     .test_norm
    bsr     .test_lerp
    rts


.test_splat:
    lea     .test_splat_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r8
    fmove.d #0.5, fp0
    hcf     vec2d_splat
    lea     .test_splat_out, r9
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_copy:
    lea     .test_copy_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r9
    lea     .test_splat_out, r8
    hcf     vec2d_copy
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_add_assign:
    lea     .test_add_assign_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r9
    lea     .test_add_in_a, r8

    move.q  #SIZE_BYTES, r0
    hcf     mem_copy

    lea     .test_add_in_b, r8
    hcf     vec2d_add_assign
    lea     .test_add_out, r8
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_add:
    lea     .test_add_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r10
    lea     .test_add_in_a, r9
    lea     .test_add_in_b, r8
    hcf     vec2d_add
    lea     .test_add_out, r8
    lea     STACK_BUFFER,  r9
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_sub_assign:
    lea     .test_sub_assign_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r9
    lea     .test_sub_in_a, r8

    move.q  #SIZE_BYTES, r0
    hcf     mem_copy

    lea     .test_sub_in_b, r8
    hcf     vec2d_sub_assign
    lea     .test_sub_out, r8
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_sub:
    lea     .test_sub_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r10
    lea     .test_sub_in_a, r9
    lea     .test_sub_in_b, r8
    hcf     vec2d_sub
    lea     .test_sub_out, r8
    lea     STACK_BUFFER,  r9
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_dot:
    lea     .test_dot_header,    r8
    hcf     io_print_string
    lea     .test_dot_in_a, r9
    lea     .test_dot_in_b, r8
    hcf     vec2d_dot
    move.q  #1, r0
    fbeq.d  #1.5, fp0, .dot_pass
    clr.q   r0
.dot_pass:
    bsr     report_result
    rts

.test_magn:
    lea     .test_magn_header,    r8
    hcf     io_print_string
    lea     .test_magn_in, r8
    hcf     vec2d_magn

    move.q  #1, r0
    fbeq.d  #5.0, fp0, .magn_pass
    clr.q   r0
.magn_pass:
    bsr     report_result
    rts

.test_norm_assign:
    lea     .test_norm_assign_header,    r8
    hcf     io_print_string
    lea     STACK_BUFFER, r9
    lea     .test_norm_in, r8

    move.q  #SIZE_BYTES, r0
    hcf     mem_copy

    exg     r8, r9

    hcf     vec2d_norm_assign
    lea     .test_norm_out, r9
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts

.test_norm:
    lea     .test_norm_header,    r8
    hcf     io_print_string
    lea     STACK_BUFFER, r9
    lea     .test_norm_in, r8
    hcf     vec2d_norm
    lea     .test_norm_out, r8
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts


.test_lerp:
    lea     .test_lerp_header, r8
    hcf     io_print_string
    lea     STACK_BUFFER, r10
    lea     .test_lerp_in_a, r9
    lea     .test_lerp_in_b, r8
    fmove.d #0.5, fp0
    hcf     vec2d_lerp
    lea     .test_lerp_out, r8
    lea     STACK_BUFFER,  r9
    move.q  #VECTOR_SIZE, r0
    bsr     verify_array_doubles
    bsr     report_result
    rts

    @align 0, 8
.
.test_splat_out:
.test_add_in_a:
.test_sub_out:
.test_dot_in_a:
    dc.d    0.5, 0.5

.test_add_in_b:
.test_sub_in_b:
.test_dot_in_b:
.test_lerp_in_b:
    dc.d    1.0, 2.0

.test_add_out:
.test_sub_in_a:
    dc.d    1.5, 2.5

.test_magn_in:
.test_norm_in:
.test_lerp_in_a:
    dc.d    3.0, 4.0

.test_norm_out:
    dc.d    0.6, 0.8

.test_lerp_out:
    dc.d    2.0, 3.0

.test_group_header:
    dc.b    "Testing vec2d operations...\n\0"

.test_splat_header:
    dc.b    "\tvec2d_splat: (r8) = fp0 \0"

.test_copy_header:
    dc.b    "\tvec2d_copy: (r9) = (r8) \0"

.test_add_assign_header:
    dc.b    "\tvec2d_add_assign: (r9) += (r8) \0"

.test_add_header:
    dc.b    "\tvec2d_add: (r10) = (r9) + (r8) \0"

.test_sub_assign_header:
    dc.b    "\tvec2d_sub_assign: (r9) -= (r8) \0"

.test_sub_header:
    dc.b    "\tvec2d_sub: (r10) = (r9) - (r8) \0"

.test_dot_header:
    dc.b    "\tvec2d_dot: fp0 = (r8) . (r9) \0"

.test_magn_header:
    dc.b    "\tvec2d_magn: fp0 = |(r8)| \0"

.test_norm_assign_header:
    dc.b    "\tvec2d_norm_assign: (r8) *= 1.0 / |(r8)| \0"

.test_norm_header:
    dc.b    "\tvec2d_norm: (r9) = (r8) * 1.0 / |(r8)| \0"

.test_lerp_header:
    dc.b    "\tvec2d_lerp: (r10) = interpolate((r9), (r8), fp0) \0"


    @def MATRIX_SIZE 9
    @def SIZE_BYTES 36
    @def STACK_BUFFER -44(sp) ; Room for one matrix and the return address of the verify call

; This is the main entry point for the float 3x3 matrix tests
mat3x3f_tests:
    lea     .test_group_header, r8
    hcf     io_print_string
    bsr     .test_identity
    bsr     .test_copy
    bsr     .test_scale_assign
    bsr     .test_scale
    bsr     .test_add_assign
    bsr     .test_add
    bsr     .test_sub_assign
    bsr     .test_sub
    bsr     .test_mul_assign
    bsr     .test_mul
    bsr     .test_trans
    bsr     .test_det
    bsr     .test_inv
    rts

; Common code to compare two 3x3 matrices and report whether or not they evaluate the same
.verify:
    move.q  #MATRIX_SIZE, r0
    bsr     verify_array_singles
    bsr     report_result
    rts

; Tests M = Identity
.test_identity:
    lea     .test_identity_header, r8
    hcf     io_print_string

    lea     STACK_BUFFER, r8
    hcf     mat3x3f_identity

    lea     .test_identity_out, r9
    bra     .verify

; tests M2 = M1
.test_copy:
    lea     .test_copy_header, r8
    hcf     io_print_string

    lea     STACK_BUFFER, r9
    lea     .test_copy_in_out, r8
    move.q  #SIZE_BYTES, r0
    hcf     mat3x3f_copy

    bra    .verify

; tests M *= scalar
.test_scale_assign:
    lea     .test_scale_assign_header, r8
    hcf     io_print_string

    lea     .test_scale_in, r8
    lea     STACK_BUFFER, r9
    move.q  #SIZE_BYTES, r0
    hcf     mem_copy
    lea     STACK_BUFFER, r8

    fmove.s #0.5, fp0
    hcf     mat3x3f_scale_assign

    lea     .test_scale_out, r9
    bra     .verify

; tests M2 = scalar * M1
.test_scale:
    lea     .test_scale_header, r8
    hcf     io_print_string

    fmove.s #0.5, fp0
    lea     .test_scale_in, r8 ; source
    lea     STACK_BUFFER, r9       ; target
    hcf     mat3x3f_scale
    lea     .test_scale_out, r8

    bra     .verify

; tests M1 += M2
.test_add_assign:
    lea     .test_add_assign_header, r8
    hcf     io_print_string

    lea     .test_add_in_a, r8
    lea     STACK_BUFFER, r9
    move.q  #SIZE_BYTES, r0
    hcf     mem_copy
    lea     .test_add_in_b, r8
    hcf     mat3x3f_add_assign

    lea     .test_add_out, r8
    bra     .verify

; tests M3 = M1 + M2
.test_add:
    lea     .test_add_header, r8
    hcf     io_print_string

    ; add
    lea     .test_add_in_a, r8
    lea     .test_add_in_b, r9
    lea     STACK_BUFFER, r10
    hcf     mat3x3f_add

    ; check the result
    lea     .test_add_out, r9
    move.q  r10, r8
    bra     .verify

; tests M1 -= M2
.test_sub_assign:
    lea     .test_sub_assign_header, r8
    hcf     io_print_string

    ; copy the identity matrix to the stack
    lea     .test_sub_in_a, r8
    lea     STACK_BUFFER, r9
    move.q  #SIZE_BYTES, r0
    hcf     mem_copy

    ; subtract in place
    lea     .test_sub_in_b, r8
    hcf     mat3x3f_sub_assign

    ; check the result
    lea     .test_sub_out, r8
    bra     .verify

; tests M3 = M1 - M2
.test_sub:
    lea     .test_sub_header, r8
    hcf     io_print_string

    ; add
    lea     .test_sub_in_a, r9
    lea     .test_sub_in_b, r8
    lea     STACK_BUFFER, r10
    hcf     mat3x3f_sub

    lea     .test_sub_out, r9
    move.q  r10, r8
    bra     .verify

; tests M1 *= M2
.test_mul_assign:
    lea     .test_mul_assign_header, r8
    hcf     io_print_string

    ; copy the identity matrix to the stack
    lea     .test_mul_in_a, r8
    lea     STACK_BUFFER, r9
    move.q  #SIZE_BYTES, r0
    hcf     mem_copy

    lea     .test_mul_in_b, r8
    hcf     mat3x3f_mul_assign

    lea     .test_mul_out, r8
    bra     .verify

; tests M3 = M1 * M2
.test_mul:
    lea     .test_mul_header, r8
    hcf     io_print_string

    lea     .test_mul_in_a, r9
    lea     .test_mul_in_b, r8
    lea     STACK_BUFFER, r10
    hcf     mat3x3f_mul

    lea     .test_mul_out, r9
    move.q  r10, r8
    bra     .verify

; tests M2 = Trans(M1)
.test_trans:
    lea     .test_trans_header, r8
    hcf     io_print_string

    lea     .test_trans_in, r8
    lea     STACK_BUFFER, r9
    hcf     mat3x3f_trans

    lea     .test_trans_out, r8
    bra     .verify

; tests det(M)
.test_det:
    lea     .test_det_header, r8
    hcf     io_print_string

    lea     .test_det_in, r8
    hcf     mat3x3f_det

    move.q  #1, r0
    fbeq.s  #10.0, fp0, .det_pass
    clr.q   r0
.det_pass:
    bsr     report_result
    rts

; tests M1 = inv(M1)
.test_inv:
    lea     .test_inv_header, r8
    hcf     io_print_string

    lea     .test_inv_in, r8
    lea     STACK_BUFFER, r9
    hcf     mat3x3f_inv

    lea     .test_inv_out, r8
    bra     .verify

    @align 0, 8
.test_identity_out:
.test_copy_in_out:
.test_scale_in:
.test_add_in_a:
.test_sub_in_b:
    dc.s    1.0, 0.0, 0.0
    dc.s    0.0, 1.0, 0.0
    dc.s    0.0, 0.0, 1.0

.test_scale_out:
.test_add_in_b:
.test_sub_out:
    dc.s    0.5, 0.0, 0.0
    dc.s    0.0, 0.5, 0.0
    dc.s    0.0, 0.0, 0.5

.test_add_out:
.test_sub_in_a:
    dc.s    1.5, 0.0, 0.0
    dc.s    0.0, 1.5, 0.0
    dc.s    0.0, 0.0, 1.5

.test_mul_in_a:
.test_trans_in:
    dc.s    1.0, 2.0, 3.0
    dc.s    4.0, 5.0, 6.0
    dc.s    7.0, 8.0, 9.0

.test_trans_out:
    dc.s    1.0, 4.0, 7.0
    dc.s    2.0, 5.0, 8.0
    dc.s    3.0, 6.0, 9.0


.test_mul_in_b:
    dc.s    9.0,  8.0, 7.0
    dc.s    6.0, -5.0, 4.0
    dc.s   -3.0,  2.0, 1.0

.test_mul_out:
    dc.s    12.0,  4.0, 18.0
    dc.s    48.0, 19.0, 54.0
    dc.s    84.0, 34.0, 90.0

.test_det_in:
.test_inv_in:
    dc.s   1.0,  3.0, 5.0
    dc.s   2.0,  4.0, 6.0
    dc.s   4.0,  7.0, 5.0

.test_inv_out:
    dc.s  -2.2,  2.0, -0.2
    dc.s   1.4, -1.5,  0.4
    dc.s  -0.2,  0.5, -0.2

.test_group_header:
    dc.b    "Testing mat3x3f operations...\n\0"

.test_identity_header:
    dc.b    "\tmat3x3f_identity: (r8) = I \0"

.test_copy_header:
    dc.b    "\tmat3x3f_copy: (r9) = (r8) \0"

.test_scale_assign_header:
    dc.b    "\tmat3x3f_scale_assign: (r8) *= fp0 \0"

.test_scale_header:
    dc.b    "\tmat3x3f_scale: (r9) = fp0 * (r8) \0"

.test_add_assign_header:
    dc.b    "\tmat3x3f_add_assign: (r9) += (r8) \0"

.test_add_header:
    dc.b    "\tmat3x3f_add: (r10) = (r9) + (r8) \0"

.test_sub_assign_header:
    dc.b    "\tmat3x3f_sub_assign: (r9) -= (r8) \0"

.test_sub_header:
    dc.b    "\tmat3x3f_sub: (r10) = (r9) - (r8) \0"

.test_mul_assign_header:
    dc.b    "\tmat3x3f_mul_assign: (r9) *= (r8) \0"

.test_mul_header:
    dc.b    "\tmat3x3f_mul: (r10) = (r9) * (r8) \0"

.test_trans_header:
    dc.b    "\tmat3x3f_trans: (r9) = T(r8) \0"

.test_det_header:
    dc.b    "\tmat3x3f_det: fp0 = det(r8) \0"

.test_inv_header:
    dc.b    "\tmat3x3f_inv: (r9) = inv(r8) \0"



;  888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
;  8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
;  88888b.d88888 888    888 888          d8P 888  888  d8P
;  888Y88888P888 888        888d888b.   d8P  888  888d88K
;  888 Y888P 888 888        888P "Y88b d88   888  8888888b
;  888  Y8P  888 888    888 888    888 8888888888 888  Y88b
;  888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
;  888       888  "Y8888P"   "Y8888P"        888  888    Y88b
;
;   - 64-bit 680x0-inspired Virtual Machine and assembler -
;
; Stub library for the standard test host IO routines

; Defines used by the Vec3 ABI. There are no initialisation or finalisation calls in this extension.


    @def vecmath_vector #2

    @equ vec2f_splat          #0, vecmath_vector
    @equ vec2f_copy           #1, vecmath_vector
    @equ vec2f_scale_assign   #2, vecmath_vector
    @equ vec2f_scale          #3, vecmath_vector
    @equ vec2f_add_assign     #4, vecmath_vector
    @equ vec2f_add            #5, vecmath_vector
    @equ vec2f_sub_assign     #6, vecmath_vector
    @equ vec2f_sub            #7, vecmath_vector
    @equ vec2f_dot            #8, vecmath_vector
    @equ vec2f_magn          #09, vecmath_vector
    @equ vec2f_norm_assign   #10, vecmath_vector
    @equ vec2f_norm          #11, vecmath_vector
    @equ vec2f_lerp          #12, vecmath_vector
    @equ vec2f_xfrm_2x2      #13, vecmath_vector
    @equ vec2f_0_xfrm_3x3    #14, vecmath_vector
    @equ vec2f_1_xfrm_3x3    #15, vecmath_vector
    @equ vec2f_to_vec3f      #16, vecmath_vector

    @equ vec3f_splat         #17, vecmath_vector
    @equ vec3f_copy          #18, vecmath_vector
    @equ vec3f_scale_assign  #19, vecmath_vector
    @equ vec3f_scale         #20, vecmath_vector
    @equ vec3f_add_assign    #21, vecmath_vector
    @equ vec3f_add           #22, vecmath_vector
    @equ vec3f_sub_assign    #23, vecmath_vector
    @equ vec3f_sub           #24, vecmath_vector
    @equ vec3f_dot           #25, vecmath_vector
    @equ vec3f_cross_assign  #26, vecmath_vector
    @equ vec3f_cross         #27, vecmath_vector
    @equ vec3f_magn          #28, vecmath_vector
    @equ vec3f_norm_as       #29, vecmath_vector
    @equ vec3f_norm          #30, vecmath_vector
    @equ vec3f_lerp          #31, vecmath_vector
    @equ vec3f_xfrm_3x3      #32, vecmath_vector
    @equ vec3f_0_xfrm_4x4    #33, vecmath_vector
    @equ vec3f_1_xfrm_4x4    #34, vecmath_vector
    @equ vec3f_to_vec4f      #35, vecmath_vector
    @equ vec4f_xfrm_4x4      #36, vecmath_vector

    @equ mat2x2f_identity     #37, vecmath_vector
    @equ mat2x2f_copy         #38, vecmath_vector
    @equ mat2x2f_scale_assign #39, vecmath_vector
    @equ mat2x2f_scale        #40, vecmath_vector
    @equ mat2x2f_add_assign   #41, vecmath_vector
    @equ mat2x2f_add          #42, vecmath_vector
    @equ mat2x2f_sub_assign   #43, vecmath_vector
    @equ mat2x2f_sub          #44, vecmath_vector
    @equ mat2x2f_mul_assign   #45, vecmath_vector
    @equ mat2x2f_mul          #46, vecmath_vector
    @equ mat2x2f_trans_assign #47, vecmath_vector
    @equ mat2x2f_trans        #48, vecmath_vector
    @equ mat2x2f_inv_assign   #49, vecmath_vector
    @equ mat2x2f_inv          #50, vecmath_vector
    @equ mat2x2f_det          #51, vecmath_vector

    @equ mat3x3f_identity     #52, vecmath_vector
    @equ mat3x3f_copy         #53, vecmath_vector
    @equ mat3x3f_scale_assign #54, vecmath_vector
    @equ mat3x3f_scale        #55, vecmath_vector
    @equ mat3x3f_add_assign   #56, vecmath_vector
    @equ mat3x3f_add          #57, vecmath_vector
    @equ mat3x3f_sub_assign   #58, vecmath_vector
    @equ mat3x3f_sub          #59, vecmath_vector
    @equ mat3x3f_mul_assign   #60, vecmath_vector
    @equ mat3x3f_mul          #61, vecmath_vector
    @equ mat3x3f_trans_assign #62, vecmath_vector
    @equ mat3x3f_trans        #63, vecmath_vector
    @equ mat3x3f_inv_assign   #64, vecmath_vector
    @equ mat3x3f_inv          #65, vecmath_vector
    @equ mat3x3f_det          #66, vecmath_vector

    @equ mat4x4f_identity     #67, vecmath_vector
    @equ mat4x4f_copy         #68, vecmath_vector
    @equ mat4x4f_scale_assign #69, vecmath_vector
    @equ mat4x4f_scale        #70, vecmath_vector
    @equ mat4x4f_add_assign   #71, vecmath_vector
    @equ mat4x4f_add          #72, vecmath_vector
    @equ mat4x4f_sub_assign   #73, vecmath_vector
    @equ mat4x4f_sub          #74, vecmath_vector
    @equ mat4x4f_mul_assign   #75, vecmath_vector
    @equ mat4x4f_mul          #76, vecmath_vector
    @equ mat4x4f_trans_assign #77, vecmath_vector
    @equ mat4x4f_trans        #78, vecmath_vector
    @equ mat4x4f_inv_assign   #79, vecmath_vector
    @equ mat4x4f_inv          #80, vecmath_vector
    @equ mat4x4f_det          #81, vecmath_vector


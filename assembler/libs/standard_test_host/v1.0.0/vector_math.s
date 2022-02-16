
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

    @equ ERR_DIV_ZERO  1000

    @equ vec2f_splat           #0, vecmath_vector
    @equ vec2f_copy            #1, vecmath_vector
    @equ vec2f_scale_assign    #2, vecmath_vector
    @equ vec2f_scale           #3, vecmath_vector
    @equ vec2f_add_assign      #4, vecmath_vector
    @equ vec2f_add             #5, vecmath_vector
    @equ vec2f_sub_assign      #6, vecmath_vector
    @equ vec2f_sub             #7, vecmath_vector
    @equ vec2f_dot             #8, vecmath_vector
    @equ vec2f_magn            #9, vecmath_vector
    @equ vec2f_norm_assign    #10, vecmath_vector
    @equ vec2f_norm           #11, vecmath_vector
    @equ vec2f_lerp           #12, vecmath_vector
    @equ vec2f_xfrm_2x2       #13, vecmath_vector
    @equ vec2f_0_xfrm_3x3     #14, vecmath_vector
    @equ vec2f_1_xfrm_3x3     #15, vecmath_vector
    @equ vec2f_to_vec3f       #16, vecmath_vector

    @equ vec3f_splat          #17, vecmath_vector
    @equ vec3f_copy           #18, vecmath_vector
    @equ vec3f_scale_assign   #19, vecmath_vector
    @equ vec3f_scale          #20, vecmath_vector
    @equ vec3f_add_assign     #21, vecmath_vector
    @equ vec3f_add            #22, vecmath_vector
    @equ vec3f_sub_assign     #23, vecmath_vector
    @equ vec3f_sub            #24, vecmath_vector
    @equ vec3f_dot            #25, vecmath_vector
    @equ vec3f_cross_assign   #26, vecmath_vector
    @equ vec3f_cross          #27, vecmath_vector
    @equ vec3f_magn           #28, vecmath_vector
    @equ vec3f_norm_as        #29, vecmath_vector
    @equ vec3f_norm           #30, vecmath_vector
    @equ vec3f_lerp           #31, vecmath_vector
    @equ vec3f_xfrm_3x3       #32, vecmath_vector
    @equ vec3f_0_xfrm_4x4     #33, vecmath_vector
    @equ vec3f_1_xfrm_4x4     #34, vecmath_vector
    @equ vec3f_to_vec4f       #35, vecmath_vector
    @equ vec4f_xfrm_4x4       #36, vecmath_vector

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
    @equ mat2x2f_trans        #47, vecmath_vector
    @equ mat2x2f_det          #48, vecmath_vector
    @equ mat2x2f_inv          #49, vecmath_vector

    @equ mat3x3f_identity     #50, vecmath_vector
    @equ mat3x3f_copy         #51, vecmath_vector
    @equ mat3x3f_scale_assign #52, vecmath_vector
    @equ mat3x3f_scale        #53, vecmath_vector
    @equ mat3x3f_add_assign   #54, vecmath_vector
    @equ mat3x3f_add          #55, vecmath_vector
    @equ mat3x3f_sub_assign   #56, vecmath_vector
    @equ mat3x3f_sub          #57, vecmath_vector
    @equ mat3x3f_mul_assign   #58, vecmath_vector
    @equ mat3x3f_mul          #59, vecmath_vector
    @equ mat3x3f_trans        #60, vecmath_vector
    @equ mat3x3f_det          #61, vecmath_vector
    @equ mat3x3f_inv          #62, vecmath_vector

    @equ mat4x4f_identity     #63, vecmath_vector
    @equ mat4x4f_copy         #64, vecmath_vector
    @equ mat4x4f_scale_assign #65, vecmath_vector
    @equ mat4x4f_scale        #66, vecmath_vector
    @equ mat4x4f_add_assign   #67, vecmath_vector
    @equ mat4x4f_add          #68, vecmath_vector
    @equ mat4x4f_sub_assign   #69, vecmath_vector
    @equ mat4x4f_sub          #70, vecmath_vector
    @equ mat4x4f_mul_assign   #71, vecmath_vector
    @equ mat4x4f_mul          #72, vecmath_vector
    @equ mat4x4f_trans        #73, vecmath_vector
    @equ mat4x4f_det          #74, vecmath_vector
    @equ mat4x4f_inv          #75, vecmath_vector


    @equ vec2d_splat          #76, vecmath_vector
    @equ vec2d_copy           #77, vecmath_vector
    @equ vec2d_scale_assign   #78, vecmath_vector
    @equ vec2d_scale          #79, vecmath_vector
    @equ vec2d_add_assign     #80, vecmath_vector
    @equ vec2d_add            #81, vecmath_vector
    @equ vec2d_sub_assign     #82, vecmath_vector
    @equ vec2d_sub            #83, vecmath_vector
    @equ vec2d_dot            #84, vecmath_vector
    @equ vec2d_magn           #85, vecmath_vector
    @equ vec2d_norm_assign    #86, vecmath_vector
    @equ vec2d_norm           #87, vecmath_vector
    @equ vec2d_lerp           #88, vecmath_vector
    @equ vec2d_xfrm_2x2       #89, vecmath_vector
    @equ vec2d_0_xfrm_3x3     #90, vecmath_vector
    @equ vec2d_1_xfrm_3x3     #91, vecmath_vector
    @equ vec2d_to_vec3f       #92, vecmath_vector

    @equ vec3d_splat          #93, vecmath_vector
    @equ vec3d_copy           #94, vecmath_vector
    @equ vec3d_scale_assign   #95, vecmath_vector
    @equ vec3d_scale          #96, vecmath_vector
    @equ vec3d_add_assign     #97, vecmath_vector
    @equ vec3d_add            #98, vecmath_vector
    @equ vec3d_sub_assign     #99, vecmath_vector
    @equ vec3d_sub            #100, vecmath_vector
    @equ vec3d_dot            #101, vecmath_vector
    @equ vec3d_cross_assign   #102, vecmath_vector
    @equ vec3d_cross          #103, vecmath_vector
    @equ vec3d_magn           #104, vecmath_vector
    @equ vec3d_norm_as        #105, vecmath_vector
    @equ vec3d_norm           #106, vecmath_vector
    @equ vec3d_lerp           #107, vecmath_vector
    @equ vec3d_xfrm_3x3       #108, vecmath_vector
    @equ vec3d_0_xfrm_4x4     #109, vecmath_vector
    @equ vec3d_1_xfrm_4x4     #110, vecmath_vector
    @equ vec3d_to_vec4f       #111, vecmath_vector
    @equ vec4d_xfrm_4x4       #112, vecmath_vector

    @equ mat2x2d_identity     #113, vecmath_vector
    @equ mat2x2d_copy         #114, vecmath_vector
    @equ mat2x2d_scale_assign #115, vecmath_vector
    @equ mat2x2d_scale        #116, vecmath_vector
    @equ mat2x2d_add_assign   #117, vecmath_vector
    @equ mat2x2d_add          #118, vecmath_vector
    @equ mat2x2d_sub_assign   #119, vecmath_vector
    @equ mat2x2d_sub          #120, vecmath_vector
    @equ mat2x2d_mul_assign   #121, vecmath_vector
    @equ mat2x2d_mul          #122, vecmath_vector
    @equ mat2x2d_trans        #123, vecmath_vector
    @equ mat2x2d_det          #124, vecmath_vector
    @equ mat2x2d_inv          #125, vecmath_vector

    @equ mat3x3d_identity     #126, vecmath_vector
    @equ mat3x3d_copy         #127, vecmath_vector
    @equ mat3x3d_scale_assign #128, vecmath_vector
    @equ mat3x3d_scale        #129, vecmath_vector
    @equ mat3x3d_add_assign   #130, vecmath_vector
    @equ mat3x3d_add          #131, vecmath_vector
    @equ mat3x3d_sub_assign   #132, vecmath_vector
    @equ mat3x3d_sub          #133, vecmath_vector
    @equ mat3x3d_mul_assign   #134, vecmath_vector
    @equ mat3x3d_mul          #135, vecmath_vector
    @equ mat3x3d_trans        #136, vecmath_vector
    @equ mat3x3d_det          #137, vecmath_vector
    @equ mat3x3d_inv          #138, vecmath_vector

    @equ mat4x4d_identity     #139, vecmath_vector
    @equ mat4x4d_copy         #140, vecmath_vector
    @equ mat4x4d_scale_assign #141, vecmath_vector
    @equ mat4x4d_scale        #142, vecmath_vector
    @equ mat4x4d_add_assign   #143, vecmath_vector
    @equ mat4x4d_add          #144, vecmath_vector
    @equ mat4x4d_sub_assign   #145, vecmath_vector
    @equ mat4x4d_sub          #146, vecmath_vector
    @equ mat4x4d_mul_assign   #147, vecmath_vector
    @equ mat4x4d_mul          #148, vecmath_vector
    @equ mat4x4d_trans        #149, vecmath_vector
    @equ mat4x4d_det          #150, vecmath_vector
    @equ mat4x4d_inv          #151, vecmath_vector

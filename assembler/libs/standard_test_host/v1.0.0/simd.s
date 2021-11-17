
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

; Defines used by the SIMD ABI. There are no initialisation or finalisation calls in this extension.


    @def simd_vector #2

    @equ vec3f_splat     hcf  #0, simd_vector
    @equ vec3f_copy      hcf  #1, simd_vector
    @equ vec3f_scale_as  hcf  #2, simd_vector
    @equ vec3f_scale     hcf  #3, simd_vector
    @equ vec3f_add_as    hcf  #4, simd_vector
    @equ vec3f_add       hcf  #5, simd_vector
    @equ vec3f_sub_as    hcf  #6, simd_vector
    @equ vec3f_sub       hcf  #7, simd_vector
    @equ vec3f_dot       hcf  #8, simd_vector
    @equ vec3f_cross_as  hcf  #9, simd_vector
    @equ vec3f_cross     hcf #10, simd_vector
    @equ vec3f_magn      hcf #11, simd_vector
    @equ vec3f_norm_as   hcf #12, simd_vector
    @equ vec3f_norm      hcf #13, simd_vector
    @equ vec3f_lerp      hcf #14, simd_vector
    @equ vec3f_xform     hcf #15, simd_vector

    @equ vec3d_splat     hcf #16, simd_vector
    @equ vec3d_copy      hcf #17, simd_vector
    @equ vec3d_scale_as  hcf #18, simd_vector
    @equ vec3d_scale     hcf #19, simd_vector
    @equ vec3d_add_as    hcf #20, simd_vector
    @equ vec3d_add       hcf #21, simd_vector
    @equ vec3d_sub_as    hcf #22, simd_vector
    @equ vec3d_sub       hcf #23, simd_vector
    @equ vec3d_dot       hcf #24, simd_vector
    @equ vec3d_cross_as  hcf #25, simd_vector
    @equ vec3d_cross     hcf #26, simd_vector
    @equ vec3d_magn      hcf #27, simd_vector
    @equ vec3d_norm_as   hcf #28, simd_vector
    @equ vec3d_norm      hcf #29, simd_vector
    @equ vec3d_lerp      hcf #30, simd_vector
    @equ vec3d_xform     hcf #31, simd_vector

<?php

/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

namespace ABadCafe\MC64K\Defs\EffectiveAddress;

/**
 * Effective Adddress Modes
 *
 * Enumerates the expected bytecode for an effective address format.
 */
interface IOther {

    const
        OFS_GPR_DIR          = 0,
        OFS_GPR_IND          = 16,
        OFS_GPR_IND_POST_INC = 32,
        OFS_GPR_IND_PRE_DEC  = 48,
        OFS_GPR_IND_DSP      = 64,
        OFS_FPU_DIR          = 80,
        OFS_OTHER            = 96
    ;

    /**
     * Bytecode definitions
     *
     * An effective address mode, (not including the specific registers or other operands) is encoded into one of the following
     * byte values.
     */
    const
        INT_LIT            = self::OFS_OTHER + 0, // #1234           OR #-1234
        ABS_W              = self::OFS_OTHER + 1, // 1234.w
        ABS_L              = self::OFS_OTHER + 2, // 1234.l
        ABS_Q              = self::OFS_OTHER + 3, // 1234.q


        END = 255
/*
        REG_IND_IDXB       =  9, // (r7, r0.b)
        REG_IND_IDXW       = 10, // (r7, r0.w)
        REG_IND_IDXL       = 11, // (r7, r0.l)
        REG_IND_IDXQ       = 12, // (r7, r0.q)

        REG_IND_IDXB_DSP   = 13, // -2(r7, r0.b)    OR (-2, r7, r0.b)
        REG_IND_IDXW_DSP   = 14, // -2(r7, r0.w)    OR (-2, r7, r0.w)
        REG_IND_IDXL_DSP   = 15, // -2(r7, r0.l)    OR (-2, r7, r0.l)
        REG_IND_IDXQ_DSP   = 16, // -2(r7, r0.q)    OR (-2, r7, r0.q)

        REG_IND_IDXB_2     = 17, // (r7, r0.b*2)
        REG_IND_IDXW_2     = 18, // (r7, r0.w*2)
        REG_IND_IDXL_2     = 19, // (r7, r0.l*2)
        REG_IND_IDXQ_2     = 20, // (r7, r0.q*2)

        REG_IND_IDXB_2_DSP = 21, // 8(r7, r0.b*2)   OR (8, r7, r0.b*2)
        REG_IND_IDXW_2_DSP = 22, // 8(r7, r0.w*2)   OR (8, r7, r0.w*2)
        REG_IND_IDXL_2_DSP = 23, // 8(r7, r0.l*2)   OR (8, r7, r0.l*2)
        REG_IND_IDXQ_2_DSP = 24, // 8(r7, r0.q*2)   OR (8, r7, r0.q*2)

        REG_IND_IDXB_4     = 25, // (r7, r0.b*4)
        REG_IND_IDXW_4     = 26, // (r7, r0.w*4)
        REG_IND_IDXL_4     = 27, // (r7, r0.l*4)
        REG_IND_IDXQ_4     = 28, // (r7, r0.q*4)

        REG_IND_IDXB_4_DSP = 29, // -6(r7, r0.b*4)  OR (-2, r7, r0.b*4)
        REG_IND_IDXW_4_DSP = 30, // -6(r7, r0.w*4)  OR (-2, r7, r0.w*4)
        REG_IND_IDXL_4_DSP = 31, // -6(r7, r0.l*4)  OR (-2, r7, r0.l*4)
        REG_IND_IDXQ_4_DSP = 32, // -6(r7, r0.q*4)  OR (-2, r7, r0.q*4)

        REG_IND_IDXB_8     = 33, // (r7, r0.b*8)
        REG_IND_IDXW_8     = 34, // (r7, r0.w*8)
        REG_IND_IDXL_8     = 35, // (r7, r0.l*8)
        REG_IND_IDXQ_8     = 36, // (r7, r0.q*8)

        REG_IND_IDXB_8_DSP = 37, // 2(r7, r0.b*8)   OR (2, r7, r0.b*8)
        REG_IND_IDXW_8_DSP = 38, // 2(r7, r0.w*8)   OR (2, r7, r0.w*8)
        REG_IND_IDXL_8_DSP = 39, // 2(r7, r0.l*8)   OR (2, r7, r0.l*8)
        REG_IND_IDXQ_8_DSP = 40, // 2(r7, r0.q*8)   OR (2, r7, r0.q*8)

        PC_IND_DSP         = 41, // 20(pc)          OR (20, pc)

        PC_IND_IDXB        = 42, // (pc, r0.b)
        PC_IND_IDXW        = 43, // (pc, r0.w)
        PC_IND_IDXL        = 44, // (pc, r0.l)
        PC_IND_IDXQ        = 45, // (pc, r0.q)

        PC_IND_IDXB_DSP    = 46, // 2(pc, r0.b)   OR (2, pc, r0.b)
        PC_IND_IDXW_DSP    = 47, // 2(pc, r0.w)   OR (2, pc, r0.w)
        PC_IND_IDXL_DSP    = 48, // 2(pc, r0.l)   OR (2, pc, r0.l)
        PC_IND_IDXQ_DSP    = 49, // 2(pc, r0.q)   OR (2, pc, r0.q)

        PC_IND_IDXB_2      = 50, // (pc, r0.b*2)
        PC_IND_IDXW_2      = 51, // (pc, r0.w*2)
        PC_IND_IDXL_2      = 52, // (pc, r0.l*2)
        PC_IND_IDXQ_2      = 53, // (pc, r0.q*2)

        PC_IND_IDXB_2_DSP  = 54, // 2(pc, r0.b*2)   OR (2, pc, r0.b*2)
        PC_IND_IDXW_2_DSP  = 55, // 2(pc, r0.w*2)   OR (2, pc, r0.w*2)
        PC_IND_IDXL_2_DSP  = 56, // 2(pc, r0.l*2)   OR (2, pc, r0.l*2)
        PC_IND_IDXQ_2_DSP  = 57, // 2(pc, r0.q*2)   OR (2, pc, r0.q*2)

        PC_IND_IDXB_4      = 58, // (pc, r0.b*4)
        PC_IND_IDXW_4      = 59, // (pc, r0.w*4)
        PC_IND_IDXL_4      = 60, // (pc, r0.l*4)
        PC_IND_IDXQ_4      = 61, // (pc, r0.q*4)

        PC_IND_IDXB_4_DSP  = 62, // -2(pc, r0.b*4)  OR (-2, pc, r0.b*4)
        PC_IND_IDXW_4_DSP  = 63, // -2(pc, r0.w*4)  OR (-2, pc, r0.w*4)
        PC_IND_IDXL_4_DSP  = 64, // -2(pc, r0.l*4)  OR (-2, pc, r0.l*4)
        PC_IND_IDXQ_4_DSP  = 65, // -2(pc, r0.q*4)  OR (-2, pc, r0.q*4)

        PC_IND_IDXB_8      = 66, // (pc, r0.b*8)
        PC_IND_IDXW_8      = 67, // (pc, r0.w*8)
        PC_IND_IDXL_8      = 68, // (pc, r0.l*8)
        PC_IND_IDXQ_8      = 69, // (pc, r0.q*8)

        PC_IND_IDXB_8_DSP  = 70, // -2(pc, r0.b*8)  OR (-2, pc, r0.b*8)
        PC_IND_IDXW_8_DSP  = 71, // -2(pc, r0.w*8)  OR (-2, pc, r0.w*8)
        PC_IND_IDXL_8_DSP  = 72, // -2(pc, r0.l*8)  OR (-2, pc, r0.l*8)
        PC_IND_IDXQ_8_DSP  = 73 // -2(pc, r0.q*8)  OR (-2, pc, r0.q*8)
*/

    ;

    /**
     * Bytecode limits
     */
    const
        MIN_BYTE = self::INT_LIT,
        MAX_BYTE = self::ABS_Q
    ;


    /**
     * Mode names. The effective address mode byte mapped to a human readable description.
     */
    const NAMES = [
        self::INT_LIT            => "Integer Literal", // #1234           OR #-1234
        self::REG                => "GPR Direct", // r0
        self::REG_FLT            => "FPU Direct", // fp1
        self::REG_IND            => "GPR Indirect", // (r1)
        self::REG_IND_POST_INC   => "GPR Indirect, Post Increment", // (r2)+
        self::REG_IND_POST_DEC   => "GPR Indirect, Post Decrement", // (r3)-
        self::REG_IND_PRE_INC    => "GPR Indirect, Pre Increment", // +(r4)
        self::REG_IND_PRE_DEC    => "GPR Indirect, Pre Decrement", // -(r5)
        self::REG_IND_DSP        => "GPR Indirect with Signed Displacement", // 2(r6)           OR (2, r6)
        self::REG_IND_IDXB       => "GPR Indirect with Signed 8-bit Index", // (r7, r0.b)
        self::REG_IND_IDXW       => "GPR Indirect with Signed 16-bit Index", // (r7, r0.w)
        self::REG_IND_IDXL       => "GPR Indirect with Signed 32-bit Index", // (r7, r0.l)
        self::REG_IND_IDXQ       => "GPR Indirect with Signed 64-bit Index", // (r7, r0.q)
        self::REG_IND_IDXB_DSP   => "GPR Indirect with Signed 8-bit Index and Signed Displacement", // -2(r7, r0.b)    OR (-2, r7, r0.b)
        self::REG_IND_IDXW_DSP   => "GPR Indirect with Signed 16-bit Index and Signed Displacement", // -2(r7, r0.w)    OR (-2, r7, r0.w)
        self::REG_IND_IDXL_DSP   => "GPR Indirect with Signed 32-bit Index and Signed Displacement", // -2(r7, r0.l)    OR (-2, r7, r0.l)
        self::REG_IND_IDXQ_DSP   => "GPR Indirect with Signed 64-bit Index and Signed Displacement", // -2(r7, r0.q)    OR (-2, r7, r0.q)
        self::REG_IND_IDXB_2     => "GPR Indirect with 16-bit Scaled Signed 8-bit Index", // (r7, r0.b*2)
        self::REG_IND_IDXW_2     => "GPR Indirect with 16-bit Scaled Signed 16-bit Index", // (r7, r0.w*2)
        self::REG_IND_IDXL_2     => "GPR Indirect with 16-bit Scaled Signed 32-bit Index", // (r7, r0.l*2)
        self::REG_IND_IDXQ_2     => "GPR Indirect with 16-bit Scaled Signed 64-bit Index", // (r7, r0.q*2)
        self::REG_IND_IDXB_2_DSP => "GPR Indirect with 16-bit Scaled Signed 8-bit Index and Signed Displacement", // 8(r7, r0.b*2)   OR (8, r7, r0.b*2)
        self::REG_IND_IDXW_2_DSP => "GPR Indirect with 16-bit Scaled Signed 16-bit Index and Signed Displacement", // 8(r7, r0.w*2)   OR (8, r7, r0.w*2)
        self::REG_IND_IDXL_2_DSP => "GPR Indirect with 16-bit Scaled Signed 32-bit Index and Signed Displacement", // 8(r7, r0.l*2)   OR (8, r7, r0.l*2)
        self::REG_IND_IDXQ_2_DSP => "GPR Indirect with 16-bit Scaled Signed 64-bit Index and Signed Displacement", // 8(r7, r0.q*2)   OR (8, r7, r0.q*2)
        self::REG_IND_IDXB_4     => "GPR Indirect with 32-bit Scaled Signed 8-bit Index", // (r7, r0.b*4)
        self::REG_IND_IDXW_4     => "GPR Indirect with 32-bit Scaled Signed 16-bit Index", // (r7, r0.w*4)
        self::REG_IND_IDXL_4     => "GPR Indirect with 32-bit Scaled Signed 32-bit Index", // (r7, r0.l*4)
        self::REG_IND_IDXQ_4     => "GPR Indirect with 32-bit Scaled Signed 64-bit Index", // (r7, r0.q*4)
        self::REG_IND_IDXB_4_DSP => "GPR Indirect with 32-bit Scaled Signed 8-bit Index and Signed Displacement", // -6(r7, r0.b*4)  OR (-2, r7, r0.b*4)
        self::REG_IND_IDXW_4_DSP => "GPR Indirect with 32-bit Scaled Signed 16-bit Index and Signed Displacement", // -6(r7, r0.w*4)  OR (-2, r7, r0.w*4)
        self::REG_IND_IDXL_4_DSP => "GPR Indirect with 32-bit Scaled Signed 32-bit Index and Signed Displacement", // -6(r7, r0.l*4)  OR (-2, r7, r0.l*4)
        self::REG_IND_IDXQ_4_DSP => "GPR Indirect with 32-bit Scaled Signed 64-bit Index and Signed Displacement", // -6(r7, r0.q*4)  OR (-2, r7, r0.q*4)
        self::REG_IND_IDXB_8     => "GPR Indirect with 64-bit Scaled Signed 8-bit Index", // (r7, r0.b*8)
        self::REG_IND_IDXW_8     => "GPR Indirect with 64-bit Scaled Signed 16-bit Index", // (r7, r0.w*8)
        self::REG_IND_IDXL_8     => "GPR Indirect with 64-bit Scaled Signed 32-bit Index", // (r7, r0.l*8)
        self::REG_IND_IDXQ_8     => "GPR Indirect with 64-bit Scaled Signed 64-bit Index", // (r7, r0.q*8)
        self::REG_IND_IDXB_8_DSP => "GPR Indirect with 64-bit Scaled Signed 8-bit Index and Signed Displacement", // 2(r7, r0.b*8)   OR (2, r7, r0.b*8)
        self::REG_IND_IDXW_8_DSP => "GPR Indirect with 64-bit Scaled Signed 16-bit Index and Signed Displacement", // 2(r7, r0.w*8)   OR (2, r7, r0.w*8)
        self::REG_IND_IDXL_8_DSP => "GPR Indirect with 64-bit Scaled Signed 32-bit Index and Signed Displacement", // 2(r7, r0.l*8)   OR (2, r7, r0.l*8)
        self::REG_IND_IDXQ_8_DSP => "GPR Indirect with 64-bit Scaled Signed 64-bit Index and Signed Displacement", // 2(r7, r0.q*8)   OR (2, r7, r0.q*8)
        self::PC_IND_DSP         => "PC with Signed Displacement", // 20(pc)          OR (20, pc)
        self::PC_IND_IDXB        => "PC with Signed 8-bit Index", // (pc, r0.b)
        self::PC_IND_IDXW        => "PC with Signed 16-bit Index", // (pc, r0.w)
        self::PC_IND_IDXL        => "PC with Signed 32-bit Index", // (pc, r0.l)
        self::PC_IND_IDXQ        => "PC with Signed 64-bit Index", // (pc, r0.q)
        self::PC_IND_IDXB_DSP    => "PC with Signed 8-bit Index and Signed Displacement", // 2(pc, r0.b)   OR (2, pc, r0.b)
        self::PC_IND_IDXW_DSP    => "PC with Signed 16-bit Index and Signed Displacement", // 2(pc, r0.w)   OR (2, pc, r0.w)
        self::PC_IND_IDXL_DSP    => "PC with Signed 32-bit Index and Signed Displacement", // 2(pc, r0.l)   OR (2, pc, r0.l)
        self::PC_IND_IDXQ_DSP    => "PC with Signed 16-bit Index and Signed Displacement", // 2(pc, r0.q)   OR (2, pc, r0.q)
        self::PC_IND_IDXB_2      => "PC with 16-bit Scaled Signed 8-bit Index", // (pc, r0.b*2)
        self::PC_IND_IDXW_2      => "PC with 16-bit Scaled Signed 16-bit Index", // (pc, r0.w*2)
        self::PC_IND_IDXL_2      => "PC with 16-bit Scaled Signed 32-bit Index", // (pc, r0.l*2)
        self::PC_IND_IDXQ_2      => "PC with 16-bit Scaled Signed 64-bit Index", // (pc, r0.q*2)
        self::PC_IND_IDXB_2_DSP  => "PC with 16-bit Scaled Signed 8-bit Index and Signed Displacement", // 2(pc, r0.b*2)   OR (2, pc, r0.b*2)
        self::PC_IND_IDXW_2_DSP  => "PC with 16-bit Scaled Signed 16-bit Index and Signed Displacement", // 2(pc, r0.w*2)   OR (2, pc, r0.w*2)
        self::PC_IND_IDXL_2_DSP  => "PC with 16-bit Scaled Signed 32-bit Index and Signed Displacement", // 2(pc, r0.l*2)   OR (2, pc, r0.l*2)
        self::PC_IND_IDXQ_2_DSP  => "PC with 16-bit Scaled Signed 64-bit Index and Signed Displacement", // 2(pc, r0.q*2)   OR (2, pc, r0.q*2)
        self::PC_IND_IDXB_4      => "PC with 32-bit Scaled Signed 8-bit Index", // (pc, r0.b*4)
        self::PC_IND_IDXW_4      => "PC with 32-bit Scaled Signed 16-bit Index", // (pc, r0.w*4)
        self::PC_IND_IDXL_4      => "PC with 32-bit Scaled Signed 32-bit Index", // (pc, r0.l*4)
        self::PC_IND_IDXQ_4      => "PC with 32-bit Scaled Signed 64-bit Index", // (pc, r0.q*4)
        self::PC_IND_IDXB_4_DSP  => "PC with 32-bit Scaled Signed 8-bit Index and Signed Displacement", // -2(pc, r0.b*4)  OR (-2, pc, r0.b*4)
        self::PC_IND_IDXW_4_DSP  => "PC with 32-bit Scaled Signed 16-bit Index and Signed Displacement", // -2(pc, r0.w*4)  OR (-2, pc, r0.w*4)
        self::PC_IND_IDXL_4_DSP  => "PC with 32-bit Scaled Signed 32-bit Index and Signed Displacement", // -2(pc, r0.l*4)  OR (-2, pc, r0.l*4)
        self::PC_IND_IDXQ_4_DSP  => "PC with 32-bit Scaled Signed 64-bit Index and Signed Displacement", // -2(pc, r0.q*4)  OR (-2, pc, r0.q*4)
        self::PC_IND_IDXB_8      => "PC with 64-bit Scaled Signed 8-bit Index", // (pc, r0.b*8)
        self::PC_IND_IDXW_8      => "PC with 64-bit Scaled Signed 16-bit Index", // (pc, r0.w*8)
        self::PC_IND_IDXL_8      => "PC with 64-bit Scaled Signed 32-bit Index", // (pc, r0.l*8)
        self::PC_IND_IDXQ_8      => "PC with 64-bit Scaled Signed 64-bit Index", // (pc, r0.q*8)
        self::PC_IND_IDXB_8_DSP  => "PC with 64-bit Scaled Signed 8-bit Index and Signed Displacement", // -2(pc, r0.b*8)  OR (-2, pc, r0.b*8)
        self::PC_IND_IDXW_8_DSP  => "PC with 64-bit Scaled Signed 16-bit Index and Signed Displacement", // -2(pc, r0.w*8)  OR (-2, pc, r0.w*8)
        self::PC_IND_IDXL_8_DSP  => "PC with 64-bit Scaled Signed 32-bit Index and Signed Displacement", // -2(pc, r0.l*8)  OR (-2, pc, r0.l*8)
        self::PC_IND_IDXQ_8_DSP  => "PC with 64-bit Scaled Signed 64-bit Index and Signed Displacement", // -2(pc, r0.q*8)  OR (-2, pc, r0.q*8)
        self::ABS_W              => "Absolute 16-bit Location", // 1234.w
        self::ABS_L              => "Absolute 32-bit Location", // 1234.l
        self::ABS_Q              => "Absolute 64-bit Location"  // 1234.q
    ];
}

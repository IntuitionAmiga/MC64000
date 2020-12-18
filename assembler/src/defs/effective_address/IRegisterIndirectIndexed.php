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

declare(strict_types = 1);

namespace ABadCafe\MC64K\Defs\EffectiveAddress;

/**
 * IRegisterIndirectIndexed
 *
 * Enumerates the bytecodes for register indirect with index addressing modes
 */
interface IRegisterIndirectIndexed extends IByteCodeGroups {

    const
        // Register Indirect with 8-bit scaled index
        REG_IND_IDXB       = self::OFS_GPR_IDX +  0, // (r7, r0.b)
        REG_IND_IDXW       = self::OFS_GPR_IDX +  1, // (r7, r0.w)
        REG_IND_IDXL       = self::OFS_GPR_IDX +  2, // (r7, r0.l)
        REG_IND_IDXQ       = self::OFS_GPR_IDX +  3, // (r7, r0.q)

        // Register Indirect with 16-bit scaled index
        REG_IND_IDXB_2     = self::OFS_GPR_IDX +  4, // (r7, r0.b*2)
        REG_IND_IDXW_2     = self::OFS_GPR_IDX +  5, // (r7, r0.w*2)
        REG_IND_IDXL_2     = self::OFS_GPR_IDX +  6, // (r7, r0.l*2)
        REG_IND_IDXQ_2     = self::OFS_GPR_IDX +  7, // (r7, r0.q*2)

        // Register Indirect with 32-bit scaled index
        REG_IND_IDXB_4     = self::OFS_GPR_IDX +  8, // (r7, r0.b*4)
        REG_IND_IDXW_4     = self::OFS_GPR_IDX +  9, // (r7, r0.w*4)
        REG_IND_IDXL_4     = self::OFS_GPR_IDX + 10, // (r7, r0.l*4)
        REG_IND_IDXQ_4     = self::OFS_GPR_IDX + 11, // (r7, r0.q*4)

        // Register Indirect with 64-bit scaled index
        REG_IND_IDXB_8     = self::OFS_GPR_IDX + 12, // (r7, r0.b*8)
        REG_IND_IDXW_8     = self::OFS_GPR_IDX + 13, // (r7, r0.w*8)
        REG_IND_IDXL_8     = self::OFS_GPR_IDX + 14, // (r7, r0.l*8)
        REG_IND_IDXQ_8     = self::OFS_GPR_IDX + 15, // (r7, r0.q*8)

        // Register Indirect with 8-bit scaled index and displacement
        REG_IND_IDXB_DSP   = self::OFS_GPR_IDX_DSP +  0, // -2(r7, r0.b)    OR (-2, r7, r0.b)
        REG_IND_IDXW_DSP   = self::OFS_GPR_IDX_DSP +  1, // -2(r7, r0.w)    OR (-2, r7, r0.w)
        REG_IND_IDXL_DSP   = self::OFS_GPR_IDX_DSP +  2, // -2(r7, r0.l)    OR (-2, r7, r0.l)
        REG_IND_IDXQ_DSP   = self::OFS_GPR_IDX_DSP +  3, // -2(r7, r0.q)    OR (-2, r7, r0.q)

        // Register Indirect with 16-bit scaled index and displacement
        REG_IND_IDXB_2_DSP = self::OFS_GPR_IDX_DSP +  4, // 8(r7, r0.b*2)   OR (8, r7, r0.b*2)
        REG_IND_IDXW_2_DSP = self::OFS_GPR_IDX_DSP +  5, // 8(r7, r0.w*2)   OR (8, r7, r0.w*2)
        REG_IND_IDXL_2_DSP = self::OFS_GPR_IDX_DSP +  6, // 8(r7, r0.l*2)   OR (8, r7, r0.l*2)
        REG_IND_IDXQ_2_DSP = self::OFS_GPR_IDX_DSP +  7, // 8(r7, r0.q*2)   OR (8, r7, r0.q*2)

        // Register Indirect with 32-bit scaled index and displacement
        REG_IND_IDXB_4_DSP = self::OFS_GPR_IDX_DSP +  8, // -6(r7, r0.b*4)  OR (-2, r7, r0.b*4)
        REG_IND_IDXW_4_DSP = self::OFS_GPR_IDX_DSP +  9, // -6(r7, r0.w*4)  OR (-2, r7, r0.w*4)
        REG_IND_IDXL_4_DSP = self::OFS_GPR_IDX_DSP + 10, // -6(r7, r0.l*4)  OR (-2, r7, r0.l*4)
        REG_IND_IDXQ_4_DSP = self::OFS_GPR_IDX_DSP + 11, // -6(r7, r0.q*4)  OR (-2, r7, r0.q*4)

        // Register Indirect with 64-bit scaled index and displacement
        REG_IND_IDXB_8_DSP = self::OFS_GPR_IDX_DSP + 12, // 2(r7, r0.b*8)   OR (2, r7, r0.b*8)
        REG_IND_IDXW_8_DSP = self::OFS_GPR_IDX_DSP + 13, // 2(r7, r0.w*8)   OR (2, r7, r0.w*8)
        REG_IND_IDXL_8_DSP = self::OFS_GPR_IDX_DSP + 14, // 2(r7, r0.l*8)   OR (2, r7, r0.l*8)
        REG_IND_IDXQ_8_DSP = self::OFS_GPR_IDX_DSP + 15, // 2(r7, r0.q*8)   OR (2, r7, r0.q*8)

        // Program Counter Indirect with 8-bit scaled index
        PC_IND_IDXB        = self::OFS_PC_IND_IDX +  0, // (pc, r0.b)
        PC_IND_IDXW        = self::OFS_PC_IND_IDX +  1, // (pc, r0.w)
        PC_IND_IDXL        = self::OFS_PC_IND_IDX +  2, // (pc, r0.l)
        PC_IND_IDXQ        = self::OFS_PC_IND_IDX +  3, // (pc, r0.q)

        // Program Counter Indirect with 16-bit scaled index
        PC_IND_IDXB_2      = self::OFS_PC_IND_IDX +  4, // (pc, r0.b*2)
        PC_IND_IDXW_2      = self::OFS_PC_IND_IDX +  5, // (pc, r0.w*2)
        PC_IND_IDXL_2      = self::OFS_PC_IND_IDX +  6, // (pc, r0.l*2)
        PC_IND_IDXQ_2      = self::OFS_PC_IND_IDX +  7, // (pc, r0.q*2)

        // Program Counter Indirect with 32-bit scaled index
        PC_IND_IDXB_4      = self::OFS_PC_IND_IDX +  8, // (pc, r0.b*4)
        PC_IND_IDXW_4      = self::OFS_PC_IND_IDX +  9, // (pc, r0.w*4)
        PC_IND_IDXL_4      = self::OFS_PC_IND_IDX + 10, // (pc, r0.l*4)
        PC_IND_IDXQ_4      = self::OFS_PC_IND_IDX + 11, // (pc, r0.q*4)

        // Program Counter Indirect with 64-bit scaled index
        PC_IND_IDXB_8      = self::OFS_PC_IND_IDX + 12, // (pc, r0.b*8)
        PC_IND_IDXW_8      = self::OFS_PC_IND_IDX + 13, // (pc, r0.w*8)
        PC_IND_IDXL_8      = self::OFS_PC_IND_IDX + 14, // (pc, r0.l*8)
        PC_IND_IDXQ_8      = self::OFS_PC_IND_IDX + 15, // (pc, r0.q*8)

        // Program Counter Indirect with 8-bit scaled index and displacement
        PC_IND_IDXB_DSP    = self::OFS_PC_IND_IDX_DSP +  0, // 2(pc, r0.b)   OR (2, pc, r0.b)
        PC_IND_IDXW_DSP    = self::OFS_PC_IND_IDX_DSP +  1, // 2(pc, r0.w)   OR (2, pc, r0.w)
        PC_IND_IDXL_DSP    = self::OFS_PC_IND_IDX_DSP +  2, // 2(pc, r0.l)   OR (2, pc, r0.l)
        PC_IND_IDXQ_DSP    = self::OFS_PC_IND_IDX_DSP +  3, // 2(pc, r0.q)   OR (2, pc, r0.q)

        // Program Counter Indirect with 16-bit scaled index and displacement
        PC_IND_IDXB_2_DSP  = self::OFS_PC_IND_IDX_DSP +  4, // 2(pc, r0.b*2)   OR (2, pc, r0.b*2)
        PC_IND_IDXW_2_DSP  = self::OFS_PC_IND_IDX_DSP +  5, // 2(pc, r0.w*2)   OR (2, pc, r0.w*2)
        PC_IND_IDXL_2_DSP  = self::OFS_PC_IND_IDX_DSP +  6, // 2(pc, r0.l*2)   OR (2, pc, r0.l*2)
        PC_IND_IDXQ_2_DSP  = self::OFS_PC_IND_IDX_DSP +  7, // 2(pc, r0.q*2)   OR (2, pc, r0.q*2)

        // Program Counter Indirect with 32-bit scaled index and displacement
        PC_IND_IDXB_4_DSP  = self::OFS_PC_IND_IDX_DSP +  8, // -2(pc, r0.b*4)  OR (-2, pc, r0.b*4)
        PC_IND_IDXW_4_DSP  = self::OFS_PC_IND_IDX_DSP +  9, // -2(pc, r0.w*4)  OR (-2, pc, r0.w*4)
        PC_IND_IDXL_4_DSP  = self::OFS_PC_IND_IDX_DSP + 10, // -2(pc, r0.l*4)  OR (-2, pc, r0.l*4)
        PC_IND_IDXQ_4_DSP  = self::OFS_PC_IND_IDX_DSP + 11, // -2(pc, r0.q*4)  OR (-2, pc, r0.q*4)

        // Program Counter Indirect with 64-bit scaled index and displacement
        PC_IND_IDXB_8_DSP  = self::OFS_PC_IND_IDX_DSP + 12, // -2(pc, r0.b*8)  OR (-2, pc, r0.b*8)
        PC_IND_IDXW_8_DSP  = self::OFS_PC_IND_IDX_DSP + 13, // -2(pc, r0.w*8)  OR (-2, pc, r0.w*8)
        PC_IND_IDXL_8_DSP  = self::OFS_PC_IND_IDX_DSP + 14, // -2(pc, r0.l*8)  OR (-2, pc, r0.l*8)
        PC_IND_IDXQ_8_DSP  = self::OFS_PC_IND_IDX_DSP + 15  // -2(pc, r0.q*8)  OR (-2, pc, r0.q*8)
    ;
}

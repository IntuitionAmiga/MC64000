#ifndef MC64K_BYTECODE_ADDRESSING_HPP
#   define MC64K_BYTECODE_ADDRESSING_HPP

/**\
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

/**
 * MC64K::ByteCode::EffectiveAddress
 *
 * Enumeration of addressing modes
 *
 * @todo header generation from template definition shared with php assembler sources.
 */

namespace MC64K::ByteCode::EffectiveAddress {

/**
 * Group
 *
 * Enumerates the base offsets for each Effective Address Type
 */
enum Group {
    OFS_GPR_DIR          = 0,   // GPR Direct r<N>
    OFS_GPR_IND          = OFS_GPR_DIR          + 16,  // GPR Indirect (r<N>)
    OFS_GPR_IND_POST_INC = OFS_GPR_IND          + 16,  // GPR Indirect, Post Increment (r<N>)+
    OFS_GPR_IND_POST_DEC = OFS_GPR_IND_POST_INC + 16,  // GPR Indirect, Post Decrement (r<N>)-
    OFS_GPR_IND_PRE_INC  = OFS_GPR_IND_POST_DEC + 16,  // GPR Indirect, Pre Increment +(r<N>)
    OFS_GPR_IND_PRE_DEC  = OFS_GPR_IND_PRE_INC  + 16,  // GPR Indirect, Pre Decrement -(r<N>)
    OFS_GPR_IND_DSP      = OFS_GPR_IND_PRE_DEC  + 16,  // GPR Indirect with Displacement <d32>(r<N>) / (<d32>, r<N>)
    OFS_FPR_DIR          = OFS_GPR_IND_DSP      + 16, // FPR Direct fp<N>
    OFS_GPR_IDX          = OFS_FPR_DIR          + 16, // GPR Indexed (r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
    OFS_GPR_IDX_DSP      = OFS_GPR_IDX          + 16, // GPR Indexed with Displacement <d32>(r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
    OFS_OTHER            = OFS_GPR_IDX_DSP      + 16,
    OFS_OTHER_2          = OFS_OTHER            + 16,

    OFS_PC_IND_IDX       = OFS_OTHER_2          + 16, // PC Indirect, indexed  (pc, r<N>.<b|w|l|q> [ * <2|4|8>])
    OFS_PC_IND_IDX_DSP   = OFS_PC_IND_IDX       + 16, // PC Indirect, indeced with displacement  <d32>(pc, r<N>.<b|w|l|q> [ * <2|4|8>])
};

/**
 * RegisterDirect
 *
 * Enumeration of Register Direct EA modes
 */
enum RegisterDirect {
    // General Purpose Register Direct r<N>
    R0_DIR    = OFS_GPR_DIR + 0,
    R1_DIR    = OFS_GPR_DIR + 1,
    R2_DIR    = OFS_GPR_DIR + 2,
    R3_DIR    = OFS_GPR_DIR + 3,
    R4_DIR    = OFS_GPR_DIR + 4,
    R5_DIR    = OFS_GPR_DIR + 5,
    R6_DIR    = OFS_GPR_DIR + 6,
    R7_DIR    = OFS_GPR_DIR + 7,
    R8_DIR    = OFS_GPR_DIR + 8,
    R9_DIR    = OFS_GPR_DIR + 9,
    R10_DIR   = OFS_GPR_DIR + 10,
    R11_DIR   = OFS_GPR_DIR + 11,
    R12_DIR   = OFS_GPR_DIR + 12,
    R13_DIR   = OFS_GPR_DIR + 13,
    R14_DIR   = OFS_GPR_DIR + 14,
    R15_DIR   = OFS_GPR_DIR + 15,

    // FPU Register Direct fp<N>
    FP0_DIR    = OFS_FPR_DIR + 0,
    FP1_DIR    = OFS_FPR_DIR + 1,
    FP2_DIR    = OFS_FPR_DIR + 2,
    FP3_DIR    = OFS_FPR_DIR + 3,
    FP4_DIR    = OFS_FPR_DIR + 4,
    FP5_DIR    = OFS_FPR_DIR + 5,
    FP6_DIR    = OFS_FPR_DIR + 6,
    FP7_DIR    = OFS_FPR_DIR + 7,
    FP8_DIR    = OFS_FPR_DIR + 8,
    FP9_DIR    = OFS_FPR_DIR + 9,
    FP10_DIR   = OFS_FPR_DIR + 10,
    FP11_DIR   = OFS_FPR_DIR + 11,
    FP12_DIR   = OFS_FPR_DIR + 12,
    FP13_DIR   = OFS_FPR_DIR + 13,
    FP14_DIR   = OFS_FPR_DIR + 14,
    FP15_DIR   = OFS_FPR_DIR + 15
};

/**
 * RegisterIndirect
 *
 * Enumeration of Register Indirect EA modes
 */
enum RegisterIndirect {
    // Register Indirect (r<N>)
    R0_IND    = OFS_GPR_IND + 0,
    R1_IND    = OFS_GPR_IND + 1,
    R2_IND    = OFS_GPR_IND + 2,
    R3_IND    = OFS_GPR_IND + 3,
    R4_IND    = OFS_GPR_IND + 4,
    R5_IND    = OFS_GPR_IND + 5,
    R6_IND    = OFS_GPR_IND + 6,
    R7_IND    = OFS_GPR_IND + 7,
    R8_IND    = OFS_GPR_IND + 8,
    R9_IND    = OFS_GPR_IND + 9,
    R10_IND   = OFS_GPR_IND + 10,
    R11_IND   = OFS_GPR_IND + 11,
    R12_IND   = OFS_GPR_IND + 12,
    R13_IND   = OFS_GPR_IND + 13,
    R14_IND   = OFS_GPR_IND + 14,
    R15_IND   = OFS_GPR_IND + 15,

    // Register Indirect, Post Increment (r<N>)+
    R0_IND_POST_INC   = OFS_GPR_IND_POST_INC + 0,
    R1_IND_POST_INC   = OFS_GPR_IND_POST_INC + 1,
    R2_IND_POST_INC   = OFS_GPR_IND_POST_INC + 2,
    R3_IND_POST_INC   = OFS_GPR_IND_POST_INC + 3,
    R4_IND_POST_INC   = OFS_GPR_IND_POST_INC + 4,
    R5_IND_POST_INC   = OFS_GPR_IND_POST_INC + 5,
    R6_IND_POST_INC   = OFS_GPR_IND_POST_INC + 6,
    R7_IND_POST_INC   = OFS_GPR_IND_POST_INC + 7,
    R8_IND_POST_INC   = OFS_GPR_IND_POST_INC + 8,
    R9_IND_POST_INC   = OFS_GPR_IND_POST_INC + 9,
    R10_IND_POST_INC  = OFS_GPR_IND_POST_INC + 10,
    R11_IND_POST_INC  = OFS_GPR_IND_POST_INC + 11,
    R12_IND_POST_INC  = OFS_GPR_IND_POST_INC + 12,
    R13_IND_POST_INC  = OFS_GPR_IND_POST_INC + 13,
    R14_IND_POST_INC  = OFS_GPR_IND_POST_INC + 14,
    R15_IND_POST_INC  = OFS_GPR_IND_POST_INC + 15,

    // Register Indirect, Post Decrement (r<N>)-
    R0_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 0,
    R1_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 1,
    R2_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 2,
    R3_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 3,
    R4_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 4,
    R5_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 5,
    R6_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 6,
    R7_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 7,
    R8_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 8,
    R9_IND_POST_DEC   = OFS_GPR_IND_POST_DEC + 9,
    R10_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 10,
    R11_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 11,
    R12_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 12,
    R13_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 13,
    R14_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 14,
    R15_IND_POST_DEC  = OFS_GPR_IND_POST_DEC + 15,

    // Register Indirect, Pre Increment +(r<N>)
    R0_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 0,
    R1_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 1,
    R2_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 2,
    R3_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 3,
    R4_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 4,
    R5_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 5,
    R6_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 6,
    R7_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 7,
    R8_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 8,
    R9_IND_PRE_INC    = OFS_GPR_IND_PRE_INC + 9,
    R10_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 10,
    R11_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 11,
    R12_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 12,
    R13_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 13,
    R14_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 14,
    R15_IND_PRE_INC   = OFS_GPR_IND_PRE_INC + 15,

    // Register Indirect, Pre Decrement -(r<N>)
    R0_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 0,
    R1_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 1,
    R2_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 2,
    R3_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 3,
    R4_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 4,
    R5_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 5,
    R6_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 6,
    R7_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 7,
    R8_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 8,
    R9_IND_PRE_DEC    = OFS_GPR_IND_PRE_DEC + 9,
    R10_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 10,
    R11_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 11,
    R12_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 12,
    R13_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 13,
    R14_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 14,
    R15_IND_PRE_DEC   = OFS_GPR_IND_PRE_DEC + 15,

    // Register Indirect with displacement <d32>(r<N>) / (<d32>, r<N>)
    R0_IND_DSP    = OFS_GPR_IND_DSP + 0,
    R1_IND_DSP    = OFS_GPR_IND_DSP + 1,
    R2_IND_DSP    = OFS_GPR_IND_DSP + 2,
    R3_IND_DSP    = OFS_GPR_IND_DSP + 3,
    R4_IND_DSP    = OFS_GPR_IND_DSP + 4,
    R5_IND_DSP    = OFS_GPR_IND_DSP + 5,
    R6_IND_DSP    = OFS_GPR_IND_DSP + 6,
    R7_IND_DSP    = OFS_GPR_IND_DSP + 7,
    R8_IND_DSP    = OFS_GPR_IND_DSP + 8,
    R9_IND_DSP    = OFS_GPR_IND_DSP + 9,
    R10_IND_DSP   = OFS_GPR_IND_DSP + 10,
    R11_IND_DSP   = OFS_GPR_IND_DSP + 11,
    R12_IND_DSP   = OFS_GPR_IND_DSP + 12,
    R13_IND_DSP   = OFS_GPR_IND_DSP + 13,
    R14_IND_DSP   = OFS_GPR_IND_DSP + 14,
    R15_IND_DSP   = OFS_GPR_IND_DSP + 15
};

/**
 * RegisterIndirectIndexed
 *
 * Enumeration of Register Indirect with Index EA modes
 */
enum RegisterIndirectIndexed {
    // Register Indirect with 8-bit scaled index
    REG_IND_IDXB       = OFS_GPR_IDX +  0, // (r7, r0.b)
    REG_IND_IDXW       = OFS_GPR_IDX +  1, // (r7, r0.w)
    REG_IND_IDXL       = OFS_GPR_IDX +  2, // (r7, r0.l)
    REG_IND_IDXQ       = OFS_GPR_IDX +  3, // (r7, r0.q)

    // Register Indirect with 16-bit scaled index
    REG_IND_IDXB_2     = OFS_GPR_IDX +  4, // (r7, r0.b*2)
    REG_IND_IDXW_2     = OFS_GPR_IDX +  5, // (r7, r0.w*2)
    REG_IND_IDXL_2     = OFS_GPR_IDX +  6, // (r7, r0.l*2)
    REG_IND_IDXQ_2     = OFS_GPR_IDX +  7, // (r7, r0.q*2)

    // Register Indirect with 32-bit scaled index
    REG_IND_IDXB_4     = OFS_GPR_IDX +  8, // (r7, r0.b*4)
    REG_IND_IDXW_4     = OFS_GPR_IDX +  9, // (r7, r0.w*4)
    REG_IND_IDXL_4     = OFS_GPR_IDX + 10, // (r7, r0.l*4)
    REG_IND_IDXQ_4     = OFS_GPR_IDX + 11, // (r7, r0.q*4)

    // Register Indirect with 64-bit scaled index
    REG_IND_IDXB_8     = OFS_GPR_IDX + 12, // (r7, r0.b*8)
    REG_IND_IDXW_8     = OFS_GPR_IDX + 13, // (r7, r0.w*8)
    REG_IND_IDXL_8     = OFS_GPR_IDX + 14, // (r7, r0.l*8)
    REG_IND_IDXQ_8     = OFS_GPR_IDX + 15, // (r7, r0.q*8)

    // Register Indirect with 8-bit scaled index and displacement
    REG_IND_IDXB_DSP   = OFS_GPR_IDX_DSP +  0, // -2(r7, r0.b)    OR (-2, r7, r0.b)
    REG_IND_IDXW_DSP   = OFS_GPR_IDX_DSP +  1, // -2(r7, r0.w)    OR (-2, r7, r0.w)
    REG_IND_IDXL_DSP   = OFS_GPR_IDX_DSP +  2, // -2(r7, r0.l)    OR (-2, r7, r0.l)
    REG_IND_IDXQ_DSP   = OFS_GPR_IDX_DSP +  3, // -2(r7, r0.q)    OR (-2, r7, r0.q)

    // Register Indirect with 16-bit scaled index and displacement
    REG_IND_IDXB_2_DSP = OFS_GPR_IDX_DSP +  4, // 8(r7, r0.b*2)   OR (8, r7, r0.b*2)
    REG_IND_IDXW_2_DSP = OFS_GPR_IDX_DSP +  5, // 8(r7, r0.w*2)   OR (8, r7, r0.w*2)
    REG_IND_IDXL_2_DSP = OFS_GPR_IDX_DSP +  6, // 8(r7, r0.l*2)   OR (8, r7, r0.l*2)
    REG_IND_IDXQ_2_DSP = OFS_GPR_IDX_DSP +  7, // 8(r7, r0.q*2)   OR (8, r7, r0.q*2)

    // Register Indirect with 32-bit scaled index and displacement
    REG_IND_IDXB_4_DSP = OFS_GPR_IDX_DSP +  8, // -6(r7, r0.b*4)  OR (-2, r7, r0.b*4)
    REG_IND_IDXW_4_DSP = OFS_GPR_IDX_DSP +  9, // -6(r7, r0.w*4)  OR (-2, r7, r0.w*4)
    REG_IND_IDXL_4_DSP = OFS_GPR_IDX_DSP + 10, // -6(r7, r0.l*4)  OR (-2, r7, r0.l*4)
    REG_IND_IDXQ_4_DSP = OFS_GPR_IDX_DSP + 11, // -6(r7, r0.q*4)  OR (-2, r7, r0.q*4)

    // Register Indirect with 64-bit scaled index and displacement
    REG_IND_IDXB_8_DSP = OFS_GPR_IDX_DSP + 12, // 2(r7, r0.b*8)   OR (2, r7, r0.b*8)
    REG_IND_IDXW_8_DSP = OFS_GPR_IDX_DSP + 13, // 2(r7, r0.w*8)   OR (2, r7, r0.w*8)
    REG_IND_IDXL_8_DSP = OFS_GPR_IDX_DSP + 14, // 2(r7, r0.l*8)   OR (2, r7, r0.l*8)
    REG_IND_IDXQ_8_DSP = OFS_GPR_IDX_DSP + 15, // 2(r7, r0.q*8)   OR (2, r7, r0.q*8)

    // Program Counter Indirect with 8-bit scaled index
    PC_IND_IDXB        = OFS_PC_IND_IDX +  0, // (pc, r0.b)
    PC_IND_IDXW        = OFS_PC_IND_IDX +  1, // (pc, r0.w)
    PC_IND_IDXL        = OFS_PC_IND_IDX +  2, // (pc, r0.l)
    PC_IND_IDXQ        = OFS_PC_IND_IDX +  3, // (pc, r0.q)

    // Program Counter Indirect with 16-bit scaled index
    PC_IND_IDXB_2      = OFS_PC_IND_IDX +  4, // (pc, r0.b*2)
    PC_IND_IDXW_2      = OFS_PC_IND_IDX +  5, // (pc, r0.w*2)
    PC_IND_IDXL_2      = OFS_PC_IND_IDX +  6, // (pc, r0.l*2)
    PC_IND_IDXQ_2      = OFS_PC_IND_IDX +  7, // (pc, r0.q*2)

    // Program Counter Indirect with 32-bit scaled index
    PC_IND_IDXB_4      = OFS_PC_IND_IDX +  8, // (pc, r0.b*4)
    PC_IND_IDXW_4      = OFS_PC_IND_IDX +  9, // (pc, r0.w*4)
    PC_IND_IDXL_4      = OFS_PC_IND_IDX + 10, // (pc, r0.l*4)
    PC_IND_IDXQ_4      = OFS_PC_IND_IDX + 11, // (pc, r0.q*4)

    // Program Counter Indirect with 64-bit scaled index
    PC_IND_IDXB_8      = OFS_PC_IND_IDX + 12, // (pc, r0.b*8)
    PC_IND_IDXW_8      = OFS_PC_IND_IDX + 13, // (pc, r0.w*8)
    PC_IND_IDXL_8      = OFS_PC_IND_IDX + 14, // (pc, r0.l*8)
    PC_IND_IDXQ_8      = OFS_PC_IND_IDX + 15, // (pc, r0.q*8)

    // Program Counter Indirect with 8-bit scaled index and displacement
    PC_IND_IDXB_DSP    = OFS_PC_IND_IDX_DSP +  0, // 2(pc, r0.b)   OR (2, pc, r0.b)
    PC_IND_IDXW_DSP    = OFS_PC_IND_IDX_DSP +  1, // 2(pc, r0.w)   OR (2, pc, r0.w)
    PC_IND_IDXL_DSP    = OFS_PC_IND_IDX_DSP +  2, // 2(pc, r0.l)   OR (2, pc, r0.l)
    PC_IND_IDXQ_DSP    = OFS_PC_IND_IDX_DSP +  3, // 2(pc, r0.q)   OR (2, pc, r0.q)

    // Program Counter Indirect with 16-bit scaled index and displacement
    PC_IND_IDXB_2_DSP  = OFS_PC_IND_IDX_DSP +  4, // 2(pc, r0.b*2)   OR (2, pc, r0.b*2)
    PC_IND_IDXW_2_DSP  = OFS_PC_IND_IDX_DSP +  5, // 2(pc, r0.w*2)   OR (2, pc, r0.w*2)
    PC_IND_IDXL_2_DSP  = OFS_PC_IND_IDX_DSP +  6, // 2(pc, r0.l*2)   OR (2, pc, r0.l*2)
    PC_IND_IDXQ_2_DSP  = OFS_PC_IND_IDX_DSP +  7, // 2(pc, r0.q*2)   OR (2, pc, r0.q*2)

    // Program Counter Indirect with 32-bit scaled index and displacement
    PC_IND_IDXB_4_DSP  = OFS_PC_IND_IDX_DSP +  8, // -2(pc, r0.b*4)  OR (-2, pc, r0.b*4)
    PC_IND_IDXW_4_DSP  = OFS_PC_IND_IDX_DSP +  9, // -2(pc, r0.w*4)  OR (-2, pc, r0.w*4)
    PC_IND_IDXL_4_DSP  = OFS_PC_IND_IDX_DSP + 10, // -2(pc, r0.l*4)  OR (-2, pc, r0.l*4)
    PC_IND_IDXQ_4_DSP  = OFS_PC_IND_IDX_DSP + 11, // -2(pc, r0.q*4)  OR (-2, pc, r0.q*4)

    // Program Counter Indirect with 64-bit scaled index and displacement
    PC_IND_IDXB_8_DSP  = OFS_PC_IND_IDX_DSP + 12, // -2(pc, r0.b*8)  OR (-2, pc, r0.b*8)
    PC_IND_IDXW_8_DSP  = OFS_PC_IND_IDX_DSP + 13, // -2(pc, r0.w*8)  OR (-2, pc, r0.w*8)
    PC_IND_IDXL_8_DSP  = OFS_PC_IND_IDX_DSP + 14, // -2(pc, r0.l*8)  OR (-2, pc, r0.l*8)
    PC_IND_IDXQ_8_DSP  = OFS_PC_IND_IDX_DSP + 15  // -2(pc, r0.q*8)  OR (-2, pc, r0.q*8)
};

/**
 * Other
 *
 * Enumeration of sundry EA modes, in particular immediate EA modes.
 */
enum Other {
    INT_IMMEDIATE      = OFS_OTHER + 0,

    // Nybble constants

    // Immediate small integer encoded directly, no extension byte.
    INT_SMALL_0        = 0,
    INT_SMALL_1        = 1,
    INT_SMALL_2        = 2,
    INT_SMALL_3        = 3,
    INT_SMALL_4        = 4,
    INT_SMALL_5        = 5,
    INT_SMALL_6        = 6,
    INT_SMALL_7        = 7,
    INT_SMALL_8        = 8,

    // Immediate integer value encoded into extension bytes.
    INT_IMM_BYTE       = 9,
    INT_IMM_WORD       = 10,
    INT_IMM_LONG       = 11,
    INT_IMM_QUAD       = 12,

    // Immediate floating point value encoded into extension bytes.
    FLT_IMMEDIATE      = 13,

    FLT_IMM_SINGLE     = 13,
    FLT_IMM_DOUBLE     = 14,

    // Program counter indirect with displacement
    PC_IND_DSP         = 15,
};

enum Other2 {
    // For the case where the source operand is the same as the destination (without side effects)
    SAME_AS_DEST       = 0,

    // Reference to an imported symbol ID
    IMPORT_SYMBOL_ID   = 1,
    RESERVED_UP_TO     = 15
};

} // namespace
#endif

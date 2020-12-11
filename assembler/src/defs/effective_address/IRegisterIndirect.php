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
 * IRegisterIndirect
 *
 * Enumerates the bytecodes for trivial register indirect addressing modes
 */
interface IRegisterIndirect extends IByteCodeGroups {

    const
        // Register Indirect (r<N>)
        R0_IND    = self::OFS_GPR_IND + 0,
        R1_IND    = self::OFS_GPR_IND + 1,
        R2_IND    = self::OFS_GPR_IND + 2,
        R3_IND    = self::OFS_GPR_IND + 3,
        R4_IND    = self::OFS_GPR_IND + 4,
        R5_IND    = self::OFS_GPR_IND + 5,
        R6_IND    = self::OFS_GPR_IND + 6,
        R7_IND    = self::OFS_GPR_IND + 7,
        R8_IND    = self::OFS_GPR_IND + 8,
        R9_IND    = self::OFS_GPR_IND + 9,
        R10_IND   = self::OFS_GPR_IND + 10,
        R11_IND   = self::OFS_GPR_IND + 11,
        R12_IND   = self::OFS_GPR_IND + 12,
        R13_IND   = self::OFS_GPR_IND + 13,
        R14_IND   = self::OFS_GPR_IND + 14,
        R15_IND   = self::OFS_GPR_IND + 15,

        // Register Indirect, Post Increment (r<N>)+
        R0_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 0,
        R1_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 1,
        R2_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 2,
        R3_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 3,
        R4_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 4,
        R5_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 5,
        R6_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 6,
        R7_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 7,
        R8_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 8,
        R9_IND_POST_INC   = self::OFS_GPR_IND_POST_INC + 9,
        R10_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 10,
        R11_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 11,
        R12_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 12,
        R13_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 13,
        R14_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 14,
        R15_IND_POST_INC  = self::OFS_GPR_IND_POST_INC + 15,

        // Register Indirect, Post Decrement (r<N>)-
        R0_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 0,
        R1_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 1,
        R2_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 2,
        R3_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 3,
        R4_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 4,
        R5_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 5,
        R6_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 6,
        R7_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 7,
        R8_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 8,
        R9_IND_POST_DEC   = self::OFS_GPR_IND_POST_DEC + 9,
        R10_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 10,
        R11_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 11,
        R12_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 12,
        R13_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 13,
        R14_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 14,
        R15_IND_POST_DEC  = self::OFS_GPR_IND_POST_DEC + 15,

        // Register Indirect, Pre Increment +(r<N>)
        R0_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 0,
        R1_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 1,
        R2_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 2,
        R3_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 3,
        R4_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 4,
        R5_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 5,
        R6_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 6,
        R7_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 7,
        R8_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 8,
        R9_IND_PRE_INC    = self::OFS_GPR_IND_PRE_INC + 9,
        R10_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 10,
        R11_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 11,
        R12_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 12,
        R13_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 13,
        R14_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 14,
        R15_IND_PRE_INC   = self::OFS_GPR_IND_PRE_INC + 15,

        // Register Indirect, Pre Decrement -(r<N>)
        R0_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 0,
        R1_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 1,
        R2_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 2,
        R3_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 3,
        R4_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 4,
        R5_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 5,
        R6_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 6,
        R7_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 7,
        R8_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 8,
        R9_IND_PRE_DEC    = self::OFS_GPR_IND_PRE_DEC + 9,
        R10_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 10,
        R11_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 11,
        R12_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 12,
        R13_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 13,
        R14_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 14,
        R15_IND_PRE_DEC   = self::OFS_GPR_IND_PRE_DEC + 15,

        // Register Indirect with displacement <d32>(r<N>) / (<d32>, r<N>)
        R0_IND_DSP    = self::OFS_GPR_IND_DSP + 0,
        R1_IND_DSP    = self::OFS_GPR_IND_DSP + 1,
        R2_IND_DSP    = self::OFS_GPR_IND_DSP + 2,
        R3_IND_DSP    = self::OFS_GPR_IND_DSP + 3,
        R4_IND_DSP    = self::OFS_GPR_IND_DSP + 4,
        R5_IND_DSP    = self::OFS_GPR_IND_DSP + 5,
        R6_IND_DSP    = self::OFS_GPR_IND_DSP + 6,
        R7_IND_DSP    = self::OFS_GPR_IND_DSP + 7,
        R8_IND_DSP    = self::OFS_GPR_IND_DSP + 8,
        R9_IND_DSP    = self::OFS_GPR_IND_DSP + 9,
        R10_IND_DSP   = self::OFS_GPR_IND_DSP + 10,
        R11_IND_DSP   = self::OFS_GPR_IND_DSP + 11,
        R12_IND_DSP   = self::OFS_GPR_IND_DSP + 12,
        R13_IND_DSP   = self::OFS_GPR_IND_DSP + 13,
        R14_IND_DSP   = self::OFS_GPR_IND_DSP + 14,
        R15_IND_DSP   = self::OFS_GPR_IND_DSP + 15
    ;
}

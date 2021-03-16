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
 * IRegisterDirect
 *
 * Enumerates the bytecodes for register direct addressing modes.
 */
interface IRegisterDirect extends IByteCodeGroups {

    const
        // General Purpose Register Direct r<N>
        R0_DIR    = self::OFS_GPR_DIR + 0,
        R1_DIR    = self::OFS_GPR_DIR + 1,
        R2_DIR    = self::OFS_GPR_DIR + 2,
        R3_DIR    = self::OFS_GPR_DIR + 3,
        R4_DIR    = self::OFS_GPR_DIR + 4,
        R5_DIR    = self::OFS_GPR_DIR + 5,
        R6_DIR    = self::OFS_GPR_DIR + 6,
        R7_DIR    = self::OFS_GPR_DIR + 7,
        R8_DIR    = self::OFS_GPR_DIR + 8,
        R9_DIR    = self::OFS_GPR_DIR + 9,
        R10_DIR   = self::OFS_GPR_DIR + 10,
        R11_DIR   = self::OFS_GPR_DIR + 11,
        R12_DIR   = self::OFS_GPR_DIR + 12,
        R13_DIR   = self::OFS_GPR_DIR + 13,
        R14_DIR   = self::OFS_GPR_DIR + 14,
        R15_DIR   = self::OFS_GPR_DIR + 15,

        // FPU Register Direct fp<N>
        FP0_DIR    = self::OFS_FPR_DIR + 0,
        FP1_DIR    = self::OFS_FPR_DIR + 1,
        FP2_DIR    = self::OFS_FPR_DIR + 2,
        FP3_DIR    = self::OFS_FPR_DIR + 3,
        FP4_DIR    = self::OFS_FPR_DIR + 4,
        FP5_DIR    = self::OFS_FPR_DIR + 5,
        FP6_DIR    = self::OFS_FPR_DIR + 6,
        FP7_DIR    = self::OFS_FPR_DIR + 7,
        FP8_DIR    = self::OFS_FPR_DIR + 8,
        FP9_DIR    = self::OFS_FPR_DIR + 9,
        FP10_DIR   = self::OFS_FPR_DIR + 10,
        FP11_DIR   = self::OFS_FPR_DIR + 11,
        FP12_DIR   = self::OFS_FPR_DIR + 12,
        FP13_DIR   = self::OFS_FPR_DIR + 13,
        FP14_DIR   = self::OFS_FPR_DIR + 14,
        FP15_DIR   = self::OFS_FPR_DIR + 15
    ;
}

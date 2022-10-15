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
 * IByteCodeGroups
 *
 * Effective address modes are encoded into a single byte, followed by any number of additional bytes for operand data, for example four bytes
 * for a 32-bit displacement value.
 *
 * MC64K defines 16 General Purpose Registers (GPR) and 16 Floating Point Registers (FPR). To help keep instruction bytcode length down, where
 * the effective address uses a single register a bytecode value is reserved for it.
 */
interface IByteCodeGroups {
    const
        OFS_GPR_DIR          = 0,   // GPR Direct r<N>
        OFS_GPR_IND          = self::OFS_GPR_DIR + 16,           // GPR Indirect (r<N>)
        OFS_GPR_IND_POST_INC = self::OFS_GPR_IND + 16,           // GPR Indirect, Post Increment (r<N>)+
        OFS_GPR_IND_POST_DEC = self::OFS_GPR_IND_POST_INC + 16,  // GPR Indirect, Post Decrement (r<N>)-
        OFS_GPR_IND_PRE_INC  = self::OFS_GPR_IND_POST_DEC + 16,  // GPR Indirect, Pre Increment +(r<N>)
        OFS_GPR_IND_PRE_DEC  = self::OFS_GPR_IND_PRE_INC  + 16,  // GPR Indirect, Pre Decrement -(r<N>)
        OFS_GPR_IND_DSP      = self::OFS_GPR_IND_PRE_DEC  + 16,  // GPR Indirect with Displacement <d32>(r<N>) / (<d32>, r<N>)
        OFS_FPR_DIR          = self::OFS_GPR_IND_DSP + 16,       // FPR Direct fp<N>
        OFS_GPR_IDX          = self::OFS_FPR_DIR + 16,           // GPR Indexed (r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_GPR_IDX_DSP      = self::OFS_GPR_IDX + 16,           // GPR Indexed with Displacement <d32>(r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_OTHER            = self::OFS_GPR_IDX_DSP + 16,       // Sundry modes (space for 32 of them)

        // Re-enumerated these but we aren't supporting them for the time being.
        OFS_PC_IND_IDX       = self::OFS_OTHER + 16,             // PC Indirect, indexed  (pc, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_PC_IND_IDX_DSP   = self::OFS_PC_IND_IDX + 16         // PC Indirect, indeced with displacement  <d32>(pc, r<N>.<b|w|l|q> [ * <2|4|8>])
    ;
}

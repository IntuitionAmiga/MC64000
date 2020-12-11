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
        OFS_GPR_IND          = 16,  // GPR Indirect (r<N>)
        OFS_GPR_IND_POST_INC = 32,  // GPR Indirect, Post Increment (r<N>)+
        OFS_GPR_IND_POST_DEC = 48,  // GPR Indirect, Post Decrement (r<N>)-
        OFS_GPR_IND_PRE_INC  = 64,  // GPR Indirect, Pre Increment +(r<N>)
        OFS_GPR_IND_PRE_DEC  = 80,  // GPR Indirect, Pre Decrement -(r<N>)
        OFS_GPR_IND_DSP      = 96,  // GPR Indirect with Displacement <d32>(r<N>) / (<d32>, r<N>)
        OFS_FPR_DIR          = 112, // FPR Direct fp<N>
        OFS_GPR_IDX          = 128, // GPR Indexed (r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_GPR_IDX_DSP      = 144, // GPR Indexed with Displacement <d32>(r<N>, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_PC_IND_IDX       = 160, // PC Indirect, indexed  (pc, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_PC_IND_IDX_DSP   = 176, // PC Indirect, indeced with displacement  <d32>(pc, r<N>.<b|w|l|q> [ * <2|4|8>])
        OFS_OTHER            = 192  // All other modes
    ;
}

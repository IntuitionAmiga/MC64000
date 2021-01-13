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
 * ISameAsDestination
 *
 * Enumerates the set of destination operand EA modes that can be cloned for the source.
 */
interface ISameAsDestination {
    const LEGAL = [
        // All register direct modes are cloneable
        IRegisterDirect::R0_DIR,
        IRegisterDirect::R1_DIR,
        IRegisterDirect::R2_DIR,
        IRegisterDirect::R3_DIR,
        IRegisterDirect::R4_DIR,
        IRegisterDirect::R5_DIR,
        IRegisterDirect::R6_DIR,
        IRegisterDirect::R7_DIR,
        IRegisterDirect::R8_DIR,
        IRegisterDirect::R9_DIR,
        IRegisterDirect::R10_DIR,
        IRegisterDirect::R11_DIR,
        IRegisterDirect::R12_DIR,
        IRegisterDirect::R13_DIR,
        IRegisterDirect::R14_DIR,
        IRegisterDirect::R15_DIR,
        IRegisterDirect::FP0_DIR,
        IRegisterDirect::FP1_DIR,
        IRegisterDirect::FP2_DIR,
        IRegisterDirect::FP3_DIR,
        IRegisterDirect::FP4_DIR,
        IRegisterDirect::FP5_DIR,
        IRegisterDirect::FP6_DIR,
        IRegisterDirect::FP7_DIR,
        IRegisterDirect::FP8_DIR,
        IRegisterDirect::FP9_DIR,
        IRegisterDirect::FP10_DIR,
        IRegisterDirect::FP11_DIR,
        IRegisterDirect::FP12_DIR,
        IRegisterDirect::FP13_DIR,
        IRegisterDirect::FP14_DIR,
        IRegisterDirect::FP15_DIR,

        // All non-modifying indirect modes are legal
        IRegisterIndirect::R0_IND,
        IRegisterIndirect::R1_IND,
        IRegisterIndirect::R2_IND,
        IRegisterIndirect::R3_IND,
        IRegisterIndirect::R4_IND,
        IRegisterIndirect::R5_IND,
        IRegisterIndirect::R6_IND,
        IRegisterIndirect::R7_IND,
        IRegisterIndirect::R8_IND,
        IRegisterIndirect::R9_IND,
        IRegisterIndirect::R10_IND,
        IRegisterIndirect::R11_IND,
        IRegisterIndirect::R12_IND,
        IRegisterIndirect::R13_IND,
        IRegisterIndirect::R14_IND,
        IRegisterIndirect::R15_IND,
        IRegisterIndirect::R0_IND_DSP,
        IRegisterIndirect::R1_IND_DSP,
        IRegisterIndirect::R2_IND_DSP,
        IRegisterIndirect::R3_IND_DSP,
        IRegisterIndirect::R4_IND_DSP,
        IRegisterIndirect::R5_IND_DSP,
        IRegisterIndirect::R6_IND_DSP,
        IRegisterIndirect::R7_IND_DSP,
        IRegisterIndirect::R8_IND_DSP,
        IRegisterIndirect::R9_IND_DSP,
        IRegisterIndirect::R10_IND_DSP,
        IRegisterIndirect::R11_IND_DSP,
        IRegisterIndirect::R12_IND_DSP,
        IRegisterIndirect::R13_IND_DSP,
        IRegisterIndirect::R14_IND_DSP,
        IRegisterIndirect::R15_IND_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXB,
        IRegisterIndirectIndexed::REG_IND_IDXW,
        IRegisterIndirectIndexed::REG_IND_IDXL,
        IRegisterIndirectIndexed::REG_IND_IDXQ,
        IRegisterIndirectIndexed::REG_IND_IDXB_2,
        IRegisterIndirectIndexed::REG_IND_IDXW_2,
        IRegisterIndirectIndexed::REG_IND_IDXL_2,
        IRegisterIndirectIndexed::REG_IND_IDXQ_2,
        IRegisterIndirectIndexed::REG_IND_IDXB_4,
        IRegisterIndirectIndexed::REG_IND_IDXW_4,
        IRegisterIndirectIndexed::REG_IND_IDXL_4,
        IRegisterIndirectIndexed::REG_IND_IDXQ_4,
        IRegisterIndirectIndexed::REG_IND_IDXB_8,
        IRegisterIndirectIndexed::REG_IND_IDXW_8,
        IRegisterIndirectIndexed::REG_IND_IDXL_8,
        IRegisterIndirectIndexed::REG_IND_IDXQ_8,
        IRegisterIndirectIndexed::REG_IND_IDXB_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXW_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXL_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXQ_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXB_2_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXW_2_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXL_2_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXQ_2_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXB_4_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXW_4_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXL_4_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXQ_4_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXB_8_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXW_8_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXL_8_DSP,
        IRegisterIndirectIndexed::REG_IND_IDXQ_8_DSP,

        // Legal as "destination" for some contexts, e.g. compare and branch
        IOther::PC_IND_DSP,

        IRegisterIndirectIndexed::PC_IND_IDXB,
        IRegisterIndirectIndexed::PC_IND_IDXW,
        IRegisterIndirectIndexed::PC_IND_IDXL,
        IRegisterIndirectIndexed::PC_IND_IDXQ,
        IRegisterIndirectIndexed::PC_IND_IDXB_2,
        IRegisterIndirectIndexed::PC_IND_IDXW_2,
        IRegisterIndirectIndexed::PC_IND_IDXL_2,
        IRegisterIndirectIndexed::PC_IND_IDXQ_2,
        IRegisterIndirectIndexed::PC_IND_IDXB_4,
        IRegisterIndirectIndexed::PC_IND_IDXW_4,
        IRegisterIndirectIndexed::PC_IND_IDXL_4,
        IRegisterIndirectIndexed::PC_IND_IDXQ_4,
        IRegisterIndirectIndexed::PC_IND_IDXB_8,
        IRegisterIndirectIndexed::PC_IND_IDXW_8,
        IRegisterIndirectIndexed::PC_IND_IDXL_8,
        IRegisterIndirectIndexed::PC_IND_IDXQ_8,
        IRegisterIndirectIndexed::PC_IND_IDXB_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXW_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXL_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXQ_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXB_2_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXW_2_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXL_2_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXQ_2_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXB_4_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXW_4_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXL_4_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXQ_4_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXB_8_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXW_8_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXL_8_DSP,
        IRegisterIndirectIndexed::PC_IND_IDXQ_8_DSP,
    ];
}

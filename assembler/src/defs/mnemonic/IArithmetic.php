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

namespace ABadCafe\MC64K\Defs\Mnemonic;

/**
 * IArithmetic
 *
 * Enumerates arithmetic instruction opcodes.
 */
interface IArithmetic extends IByteCodeGroups {
    const
        // Fast path register to register
        R2R_EXTB_L  = self::OFS_ARITHMETIC +  0,
        R2R_EXTB_Q  = self::OFS_ARITHMETIC +  1,
        R2R_EXTW_L  = self::OFS_ARITHMETIC +  2,
        R2R_EXTW_Q  = self::OFS_ARITHMETIC +  3,
        R2R_EXTL_Q  = self::OFS_ARITHMETIC +  4,
        R2R_NEG_L   = self::OFS_ARITHMETIC +  5,
        R2R_NEG_Q   = self::OFS_ARITHMETIC +  6,
        R2R_FNEG_S  = self::OFS_ARITHMETIC +  7,
        R2R_FNEG_D  = self::OFS_ARITHMETIC +  8,
        R2R_FABS_S  = self::OFS_ARITHMETIC +  9,

        R2R_FABS_D  = self::OFS_ARITHMETIC + 10,
        R2R_ADD_L   = self::OFS_ARITHMETIC + 11,
        R2R_ADD_Q   = self::OFS_ARITHMETIC + 12,
        R2R_FADD_S  = self::OFS_ARITHMETIC + 13,
        R2R_FADD_D  = self::OFS_ARITHMETIC + 14,
        R2R_SUB_L   = self::OFS_ARITHMETIC + 15,
        R2R_SUB_Q   = self::OFS_ARITHMETIC + 16,
        R2R_FSUB_S  = self::OFS_ARITHMETIC + 17,
        R2R_FSUB_D  = self::OFS_ARITHMETIC + 18,
        R2R_MULS_L  = self::OFS_ARITHMETIC + 19,

        R2R_MULS_Q  = self::OFS_ARITHMETIC + 20,
        R2R_MULU_L  = self::OFS_ARITHMETIC + 21,
        R2R_MULU_Q  = self::OFS_ARITHMETIC + 22,
        R2R_FMUL_S  = self::OFS_ARITHMETIC + 23,
        R2R_FMUL_D  = self::OFS_ARITHMETIC + 24,
        R2R_FDIV_S  = self::OFS_ARITHMETIC + 25,
        R2R_FDIV_D  = self::OFS_ARITHMETIC + 26,
        R2R_FMOD_S  = self::OFS_ARITHMETIC + 27,
        R2R_FMOD_D  = self::OFS_ARITHMETIC + 28,
        R2R_FSQRT_S = self::OFS_ARITHMETIC + 29,

        R2R_FSQRT_D = self::OFS_ARITHMETIC + 30,
        EXTB_W      = self::OFS_ARITHMETIC + 31,
        EXTB_L      = self::OFS_ARITHMETIC + 32,
        EXTB_Q      = self::OFS_ARITHMETIC + 33,
        EXTW_L      = self::OFS_ARITHMETIC + 34,
        EXTW_Q      = self::OFS_ARITHMETIC + 35,
        EXTL_Q      = self::OFS_ARITHMETIC + 36,
        ASL_B       = self::OFS_ARITHMETIC + 37,
        ASL_W       = self::OFS_ARITHMETIC + 38,
        ASL_L       = self::OFS_ARITHMETIC + 39,

        ASL_Q       = self::OFS_ARITHMETIC + 40,
        ASR_B       = self::OFS_ARITHMETIC + 41,
        ASR_W       = self::OFS_ARITHMETIC + 42,
        ASR_L       = self::OFS_ARITHMETIC + 43,
        ASR_Q       = self::OFS_ARITHMETIC + 44,
        ADD_B       = self::OFS_ARITHMETIC + 45,
        ADD_W       = self::OFS_ARITHMETIC + 46,
        ADD_L       = self::OFS_ARITHMETIC + 47,
        ADD_Q       = self::OFS_ARITHMETIC + 48,
        FADD_S      = self::OFS_ARITHMETIC + 49,

        FADD_D      = self::OFS_ARITHMETIC + 50,
        SUB_B       = self::OFS_ARITHMETIC + 51,
        SUB_W       = self::OFS_ARITHMETIC + 52,
        SUB_L       = self::OFS_ARITHMETIC + 53,
        SUB_Q       = self::OFS_ARITHMETIC + 54,
        FSUB_S      = self::OFS_ARITHMETIC + 55,
        FSUB_D      = self::OFS_ARITHMETIC + 56,
        NEG_B       = self::OFS_ARITHMETIC + 57,
        NEG_W       = self::OFS_ARITHMETIC + 58,
        NEG_L       = self::OFS_ARITHMETIC + 59,

        NEG_Q       = self::OFS_ARITHMETIC + 60,
        FNEG_S      = self::OFS_ARITHMETIC + 61,
        FNEG_D      = self::OFS_ARITHMETIC + 62,
        MULS_B      = self::OFS_ARITHMETIC + 63,
        MULS_W      = self::OFS_ARITHMETIC + 64,
        MULS_L      = self::OFS_ARITHMETIC + 65,
        MULS_Q      = self::OFS_ARITHMETIC + 66,
        MULU_B      = self::OFS_ARITHMETIC + 67,
        MULU_W      = self::OFS_ARITHMETIC + 68,
        MULU_L      = self::OFS_ARITHMETIC + 69,

        MULU_Q      = self::OFS_ARITHMETIC + 70,
        FMUL_S      = self::OFS_ARITHMETIC + 71,
        FMUL_D      = self::OFS_ARITHMETIC + 72,
        DIVS_L      = self::OFS_ARITHMETIC + 73,
        DIVS_Q      = self::OFS_ARITHMETIC + 74,
        MODS_L      = self::OFS_ARITHMETIC + 75,
        MODS_Q      = self::OFS_ARITHMETIC + 76,
        DIVU_L      = self::OFS_ARITHMETIC + 77,
        DIVU_Q      = self::OFS_ARITHMETIC + 78,
        MODU_L      = self::OFS_ARITHMETIC + 79,

        MODU_Q      = self::OFS_ARITHMETIC + 80,
        FDIV_S      = self::OFS_ARITHMETIC + 81,
        FDIV_D      = self::OFS_ARITHMETIC + 82,
        FMOD_S      = self::OFS_ARITHMETIC + 83,
        FMOD_D      = self::OFS_ARITHMETIC + 84,
        FABS_S      = self::OFS_ARITHMETIC + 85,
        FABS_D      = self::OFS_ARITHMETIC + 86,
        FSQRT_S     = self::OFS_ARITHMETIC + 87,
        FSQRT_D     = self::OFS_ARITHMETIC + 88,
        FACOS_S     = self::OFS_ARITHMETIC + 89,

        FACOS_D     = self::OFS_ARITHMETIC + 90,
        FASIN_S     = self::OFS_ARITHMETIC + 91,
        FASIN_D     = self::OFS_ARITHMETIC + 92,
        FATAN_S     = self::OFS_ARITHMETIC + 93,
        FATAN_D     = self::OFS_ARITHMETIC + 94,
        FCOS_S      = self::OFS_ARITHMETIC + 95,
        FCOS_D      = self::OFS_ARITHMETIC + 96,
        FSIN_S      = self::OFS_ARITHMETIC + 97,
        FSIN_D      = self::OFS_ARITHMETIC + 98,
        FSINCOS_S   = self::OFS_ARITHMETIC + 99,

        FSINCOS_D   = self::OFS_ARITHMETIC + 100,
        FTAN_S      = self::OFS_ARITHMETIC + 101,
        FTAN_D      = self::OFS_ARITHMETIC + 102,
        FETOX_S     = self::OFS_ARITHMETIC + 103,
        FETOX_D     = self::OFS_ARITHMETIC + 104,
        FLOGN_S     = self::OFS_ARITHMETIC + 105,
        FLOGN_D     = self::OFS_ARITHMETIC + 106,
        FLOG2_S     = self::OFS_ARITHMETIC + 107,
        FLOG2_D     = self::OFS_ARITHMETIC + 108,
        FTWOTOX_S   = self::OFS_ARITHMETIC + 109,

        FTWOTOX_D   = self::OFS_ARITHMETIC + 110
    ;

}

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
        R2R_FMACC_S = self::OFS_ARITHMETIC + 31,
        R2R_FMACC_D = self::OFS_ARITHMETIC + 32,
        R2R_FMADD_S = self::OFS_ARITHMETIC + 33,
        R2R_FMADD_D = self::OFS_ARITHMETIC + 34,
        EXTB_W      = self::OFS_ARITHMETIC + 35,
        EXTB_L      = self::OFS_ARITHMETIC + 36,
        EXTB_Q      = self::OFS_ARITHMETIC + 37,
        EXTW_L      = self::OFS_ARITHMETIC + 38,
        EXTW_Q      = self::OFS_ARITHMETIC + 39,

        EXTL_Q      = self::OFS_ARITHMETIC + 40,
        ASL_B       = self::OFS_ARITHMETIC + 41,
        ASL_W       = self::OFS_ARITHMETIC + 42,
        ASL_L       = self::OFS_ARITHMETIC + 43,
        ASL_Q       = self::OFS_ARITHMETIC + 44,
        ASR_B       = self::OFS_ARITHMETIC + 45,
        ASR_W       = self::OFS_ARITHMETIC + 46,
        ASR_L       = self::OFS_ARITHMETIC + 47,
        ASR_Q       = self::OFS_ARITHMETIC + 48,
        ADD_B       = self::OFS_ARITHMETIC + 49,

        ADD_W       = self::OFS_ARITHMETIC + 50,
        ADD_L       = self::OFS_ARITHMETIC + 51,
        ADD_Q       = self::OFS_ARITHMETIC + 52,
        FADD_S      = self::OFS_ARITHMETIC + 53,
        FADD_D      = self::OFS_ARITHMETIC + 54,
        SUB_B       = self::OFS_ARITHMETIC + 55,
        SUB_W       = self::OFS_ARITHMETIC + 56,
        SUB_L       = self::OFS_ARITHMETIC + 57,
        SUB_Q       = self::OFS_ARITHMETIC + 58,
        FSUB_S      = self::OFS_ARITHMETIC + 59,

        FSUB_D      = self::OFS_ARITHMETIC + 60,
        NEG_B       = self::OFS_ARITHMETIC + 61,
        NEG_W       = self::OFS_ARITHMETIC + 62,
        NEG_L       = self::OFS_ARITHMETIC + 63,
        NEG_Q       = self::OFS_ARITHMETIC + 64,
        FNEG_S      = self::OFS_ARITHMETIC + 65,
        FNEG_D      = self::OFS_ARITHMETIC + 66,
        MULS_B      = self::OFS_ARITHMETIC + 67,
        MULS_W      = self::OFS_ARITHMETIC + 68,
        MULS_L      = self::OFS_ARITHMETIC + 69,

        MULS_Q      = self::OFS_ARITHMETIC + 70,
        MULU_B      = self::OFS_ARITHMETIC + 71,
        MULU_W      = self::OFS_ARITHMETIC + 72,
        MULU_L      = self::OFS_ARITHMETIC + 73,
        MULU_Q      = self::OFS_ARITHMETIC + 74,
        FMUL_S      = self::OFS_ARITHMETIC + 75,
        FMUL_D      = self::OFS_ARITHMETIC + 76,
        DIVS_L      = self::OFS_ARITHMETIC + 77,
        DIVS_Q      = self::OFS_ARITHMETIC + 78,
        MODS_L      = self::OFS_ARITHMETIC + 79,

        MODS_Q      = self::OFS_ARITHMETIC + 80,
        DIVU_L      = self::OFS_ARITHMETIC + 81,
        DIVU_Q      = self::OFS_ARITHMETIC + 82,
        MODU_L      = self::OFS_ARITHMETIC + 83,
        MODU_Q      = self::OFS_ARITHMETIC + 84,
        FDIV_S      = self::OFS_ARITHMETIC + 85,
        FDIV_D      = self::OFS_ARITHMETIC + 86,
        FMOD_S      = self::OFS_ARITHMETIC + 87,
        FMOD_D      = self::OFS_ARITHMETIC + 88,
        FABS_S      = self::OFS_ARITHMETIC + 89,

        FABS_D      = self::OFS_ARITHMETIC + 90,
        FSQRT_S     = self::OFS_ARITHMETIC + 91,
        FSQRT_D     = self::OFS_ARITHMETIC + 92,
        FACOS_S     = self::OFS_ARITHMETIC + 93,
        FACOS_D     = self::OFS_ARITHMETIC + 94,
        FASIN_S     = self::OFS_ARITHMETIC + 95,
        FASIN_D     = self::OFS_ARITHMETIC + 96,
        FATAN_S     = self::OFS_ARITHMETIC + 97,
        FATAN_D     = self::OFS_ARITHMETIC + 98,
        FCOS_S      = self::OFS_ARITHMETIC + 99,

        FCOS_D      = self::OFS_ARITHMETIC + 100,
        FSIN_S      = self::OFS_ARITHMETIC + 101,
        FSIN_D      = self::OFS_ARITHMETIC + 102,
        FSINCOS_S   = self::OFS_ARITHMETIC + 103,
        FSINCOS_D   = self::OFS_ARITHMETIC + 104,
        FTAN_S      = self::OFS_ARITHMETIC + 105,
        FTAN_D      = self::OFS_ARITHMETIC + 106,
        FETOX_S     = self::OFS_ARITHMETIC + 107,
        FETOX_D     = self::OFS_ARITHMETIC + 108,
        FLOGN_S     = self::OFS_ARITHMETIC + 109,

        FLOGN_D     = self::OFS_ARITHMETIC + 110,
        FLOG2_S     = self::OFS_ARITHMETIC + 111,
        FLOG2_D     = self::OFS_ARITHMETIC + 112,
        FTWOTOX_S   = self::OFS_ARITHMETIC + 113,
        FTWOTOX_D   = self::OFS_ARITHMETIC + 114
    ;

}

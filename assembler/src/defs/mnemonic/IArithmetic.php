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
 * ILogical
 *
 * Enumerates arithmetic instructions
 */
interface IArithmetic extends IByteCodeGroups {
    const
        EXTB_W    = self::OFS_ARITHMETIC +  0, // 0xA0
        EXTB_L    = self::OFS_ARITHMETIC +  1,
        EXTB_Q    = self::OFS_ARITHMETIC +  2,
        EXTW_L    = self::OFS_ARITHMETIC +  3,
        EXTW_Q    = self::OFS_ARITHMETIC +  4,
        EXTL_Q    = self::OFS_ARITHMETIC +  5,
        ASL_B     = self::OFS_ARITHMETIC +  6,
        ASL_W     = self::OFS_ARITHMETIC +  7,
        ASL_L     = self::OFS_ARITHMETIC +  8,
        ASL_Q     = self::OFS_ARITHMETIC +  9,
        ASR_B     = self::OFS_ARITHMETIC + 10,
        ASR_W     = self::OFS_ARITHMETIC + 11,
        ASR_L     = self::OFS_ARITHMETIC + 12,
        ASR_Q     = self::OFS_ARITHMETIC + 13,
        ADD_B     = self::OFS_ARITHMETIC + 14,
        ADD_W     = self::OFS_ARITHMETIC + 15,

        ADD_L     = self::OFS_ARITHMETIC + 16, // 0xB0
        ADD_Q     = self::OFS_ARITHMETIC + 17,
        FADD_S    = self::OFS_ARITHMETIC + 18,
        FADD_D    = self::OFS_ARITHMETIC + 19,
        SUB_B     = self::OFS_ARITHMETIC + 20,
        SUB_W     = self::OFS_ARITHMETIC + 21,
        SUB_L     = self::OFS_ARITHMETIC + 22,
        SUB_Q     = self::OFS_ARITHMETIC + 23,
        FSUB_S    = self::OFS_ARITHMETIC + 24,
        FSUB_D    = self::OFS_ARITHMETIC + 25,
        NEG_B     = self::OFS_ARITHMETIC + 26,
        NEG_W     = self::OFS_ARITHMETIC + 27,
        NEG_L     = self::OFS_ARITHMETIC + 28,
        NEG_Q     = self::OFS_ARITHMETIC + 29,
        FNEG_S    = self::OFS_ARITHMETIC + 30,
        FNEG_D    = self::OFS_ARITHMETIC + 31,

        MULS_B    = self::OFS_ARITHMETIC + 32, // 0xC0
        MULS_W    = self::OFS_ARITHMETIC + 33,
        MULS_L    = self::OFS_ARITHMETIC + 34,
        MULS_Q    = self::OFS_ARITHMETIC + 35,
        MULU_B    = self::OFS_ARITHMETIC + 36,
        MULU_W    = self::OFS_ARITHMETIC + 37,
        MULU_L    = self::OFS_ARITHMETIC + 38,
        MULU_Q    = self::OFS_ARITHMETIC + 39,
        FMUL_S    = self::OFS_ARITHMETIC + 40,
        FMUL_D    = self::OFS_ARITHMETIC + 41,
        DIVS_B    = self::OFS_ARITHMETIC + 42,
        DIVS_W    = self::OFS_ARITHMETIC + 43,
        DIVS_L    = self::OFS_ARITHMETIC + 44,
        DIVS_Q    = self::OFS_ARITHMETIC + 45,
        DIVU_B    = self::OFS_ARITHMETIC + 46,
        DIVU_W    = self::OFS_ARITHMETIC + 47,

        DIVU_L    = self::OFS_ARITHMETIC + 48, // 0xD0
        DIVU_Q    = self::OFS_ARITHMETIC + 49,
        FDIV_S    = self::OFS_ARITHMETIC + 50,
        FDIV_D    = self::OFS_ARITHMETIC + 51,
        FMOD_S    = self::OFS_ARITHMETIC + 52,
        FMOD_D    = self::OFS_ARITHMETIC + 53,
        FABS_S    = self::OFS_ARITHMETIC + 54,
        FABS_D    = self::OFS_ARITHMETIC + 55,
        FSQRT_S   = self::OFS_ARITHMETIC + 56,
        FSQRT_D   = self::OFS_ARITHMETIC + 57,
        FACOS_S   = self::OFS_ARITHMETIC + 58,
        FACOS_D   = self::OFS_ARITHMETIC + 59,
        FASIN_S   = self::OFS_ARITHMETIC + 60,
        FASIN_D   = self::OFS_ARITHMETIC + 61,
        FATAN_S   = self::OFS_ARITHMETIC + 62,
        FATAN_D   = self::OFS_ARITHMETIC + 63,

        FCOS_S    = self::OFS_ARITHMETIC + 64, // 0xE0
        FCOS_D    = self::OFS_ARITHMETIC + 65,
        FSIN_S    = self::OFS_ARITHMETIC + 66,
        FSIN_D    = self::OFS_ARITHMETIC + 67,
        FSINCOS_S = self::OFS_ARITHMETIC + 68,
        FSINCOS_D = self::OFS_ARITHMETIC + 69,
        FTAN_S    = self::OFS_ARITHMETIC + 70,
        FTAN_D    = self::OFS_ARITHMETIC + 71,
        FETOX_S   = self::OFS_ARITHMETIC + 72,
        FETOX_D   = self::OFS_ARITHMETIC + 73,
        FLOGN_S   = self::OFS_ARITHMETIC + 74,
        FLOGN_D   = self::OFS_ARITHMETIC + 75,
        FLOG2_S   = self::OFS_ARITHMETIC + 76,
        FLOG2_D   = self::OFS_ARITHMETIC + 77,
        FTWOTOX_S = self::OFS_ARITHMETIC + 78,
        FTWOTOX_D = self::OFS_ARITHMETIC + 79,

        FGETEXP_S = self::OFS_ARITHMETIC + 80, // 0xF0
        FGETEXP_D = self::OFS_ARITHMETIC + 81,
        FGETMAN_S = self::OFS_ARITHMETIC + 82,
        FGETMAN_D = self::OFS_ARITHMETIC + 83

    ;

}

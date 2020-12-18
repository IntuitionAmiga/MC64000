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
 * IControl
 *
 * Enumerates flow control instruction opcodes.
 */
interface IControl extends IByteCodeGroups {
    const
        HCF    = self::OFS_CONTROL +  0, // 0x00
        BRA_B  = self::OFS_CONTROL +  1,
        BRA    = self::OFS_CONTROL +  2,
        BSR_B  = self::OFS_CONTROL +  3,
        BSR    = self::OFS_CONTROL +  4,
        JMP    = self::OFS_CONTROL +  5,
        JSR    = self::OFS_CONTROL +  6,
        RTS    = self::OFS_CONTROL +  7,
        BIZ_B  = self::OFS_CONTROL +  8,
        BIZ_W  = self::OFS_CONTROL +  9,
        BIZ_L  = self::OFS_CONTROL + 10,
        BIZ_Q  = self::OFS_CONTROL + 11,
        FBIZ_S = self::OFS_CONTROL + 12,
        FBIZ_D = self::OFS_CONTROL + 13,
        BNZ_B  = self::OFS_CONTROL + 14,
        BNZ_W  = self::OFS_CONTROL + 15,

        BNZ_L  = self::OFS_CONTROL + 16, // 0x10
        BNZ_Q  = self::OFS_CONTROL + 17,
        FBNZ_S = self::OFS_CONTROL + 18,
        FBNZ_D = self::OFS_CONTROL + 19,
        BMI_B  = self::OFS_CONTROL + 20,
        BMI_W  = self::OFS_CONTROL + 21,
        BMI_L  = self::OFS_CONTROL + 22,
        BMI_Q  = self::OFS_CONTROL + 23,
        FBMI_S = self::OFS_CONTROL + 24,
        FBMI_D = self::OFS_CONTROL + 25,
        BPL_B  = self::OFS_CONTROL + 26,
        BPL_W  = self::OFS_CONTROL + 27,
        BPL_L  = self::OFS_CONTROL + 28,
        BPL_Q  = self::OFS_CONTROL + 29,
        FPBL_S = self::OFS_CONTROL + 30,
        FBPL_D = self::OFS_CONTROL + 31,

        BLT_B  = self::OFS_CONTROL + 32, // 0x20
        BLT_W  = self::OFS_CONTROL + 33,
        BLT_L  = self::OFS_CONTROL + 34,
        BLT_Q  = self::OFS_CONTROL + 35,
        FBLT_S = self::OFS_CONTROL + 36,
        FBLT_D = self::OFS_CONTROL + 37,
        BLE_B  = self::OFS_CONTROL + 38,
        BLE_W  = self::OFS_CONTROL + 39,
        BLE_L  = self::OFS_CONTROL + 40,
        BLE_Q  = self::OFS_CONTROL + 41,
        FBLE_S = self::OFS_CONTROL + 42,
        FBLE_D = self::OFS_CONTROL + 43,
        BEQ_B  = self::OFS_CONTROL + 44,
        BEQ_W  = self::OFS_CONTROL + 45,
        BEQ_L  = self::OFS_CONTROL + 46,
        BEQ_Q  = self::OFS_CONTROL + 47,

        FBEQ_S = self::OFS_CONTROL + 48, // 0x30
        FBEQ_D = self::OFS_CONTROL + 49,
        BGE_B  = self::OFS_CONTROL + 50,
        BGE_W  = self::OFS_CONTROL + 51,
        BGE_L  = self::OFS_CONTROL + 52,
        BGE_Q  = self::OFS_CONTROL + 53,
        FBGE_S = self::OFS_CONTROL + 54,
        FBGE_D = self::OFS_CONTROL + 55,
        BGT_B  = self::OFS_CONTROL + 56,
        BGT_W  = self::OFS_CONTROL + 57,
        BGT_L  = self::OFS_CONTROL + 58,
        BGT_Q  = self::OFS_CONTROL + 59,
        FBGT_S = self::OFS_CONTROL + 60,
        FBGT_D = self::OFS_CONTROL + 61,
        BNE_B  = self::OFS_CONTROL + 62,
        BNE_W  = self::OFS_CONTROL + 63,

        BNE_L  = self::OFS_CONTROL + 64, // 0x40
        BNE_Q  = self::OFS_CONTROL + 65,
        FBNE_S = self::OFS_CONTROL + 66,
        FBNE_D = self::OFS_CONTROL + 67,
        BBS_B  = self::OFS_CONTROL + 68,
        BBS_W  = self::OFS_CONTROL + 69,
        BBS_L  = self::OFS_CONTROL + 70,
        BBS_Q  = self::OFS_CONTROL + 71,
        BBC_B  = self::OFS_CONTROL + 72,
        BBC_W  = self::OFS_CONTROL + 73,
        BBC_L  = self::OFS_CONTROL + 74,
        BBC_Q  = self::OFS_CONTROL + 75,
        DBNZ   = self::OFS_CONTROL + 76
    ;

}

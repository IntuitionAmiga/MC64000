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
 * IDataMove
 *
 * Enumerates data movement instructions
 */
interface IDataMove extends IByteCodeGroups {
    const
        MOVE_B   = self::OFS_DATA_MOVE +  0, // 0x4D
        MOVE_W   = self::OFS_DATA_MOVE +  1,
        MOVE_L   = self::OFS_DATA_MOVE +  2,

        MOVE_Q   = self::OFS_DATA_MOVE +  3, // 0x50
        SAVEM    = self::OFS_DATA_MOVE +  4,
        LOADM    = self::OFS_DATA_MOVE +  5,
        FMOVEB_S = self::OFS_DATA_MOVE +  6,
        FMOVEB_D = self::OFS_DATA_MOVE +  7,
        FMOVEW_S = self::OFS_DATA_MOVE +  8,
        FMOVEW_D = self::OFS_DATA_MOVE +  9,
        FMOVEL_S = self::OFS_DATA_MOVE + 10,
        FMOVEL_D = self::OFS_DATA_MOVE + 11,
        FMOVEQ_S = self::OFS_DATA_MOVE + 12,
        FMOVEQ_D = self::OFS_DATA_MOVE + 13,
        FMOVES_L = self::OFS_DATA_MOVE + 14,
        FMOVES_Q = self::OFS_DATA_MOVE + 15,
        FMOVES_D = self::OFS_DATA_MOVE + 16,
        FMOVED_L = self::OFS_DATA_MOVE + 17,
        FMOVED_Q = self::OFS_DATA_MOVE + 18,

        FMOVED_S = self::OFS_DATA_MOVE + 19, // 0x60
        FSAVEM   = self::OFS_DATA_MOVE + 20,
        FLOADM   = self::OFS_DATA_MOVE + 21,
        CLR_B    = self::OFS_DATA_MOVE + 22,
        CLR_W    = self::OFS_DATA_MOVE + 23,
        CLR_L    = self::OFS_DATA_MOVE + 24,
        CLR_Q    = self::OFS_DATA_MOVE + 25,
        EXG      = self::OFS_DATA_MOVE + 26,
        FEXG     = self::OFS_DATA_MOVE + 27,
        SWAP     = self::OFS_DATA_MOVE + 28,
        SWAP_L   = self::OFS_DATA_MOVE + 29,
        SWAP_Q   = self::OFS_DATA_MOVE + 30,
        LINK     = self::OFS_DATA_MOVE + 31,
        UNLK     = self::OFS_DATA_MOVE + 32,
        LEA      = self::OFS_DATA_MOVE + 33,
        PEA      = self::OFS_DATA_MOVE + 34 // 0x6F
    ;

}

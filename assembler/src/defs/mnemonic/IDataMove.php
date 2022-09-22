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
 * Enumerates data movement instruction opcodes.
 */
interface IDataMove extends IByteCodeGroups {
    const
        // Fast path register-to-register versions
        R2R_MOVE_L   = self::OFS_DATA_MOVE +  0,
        R2R_MOVE_Q   = self::OFS_DATA_MOVE +  1,
        R2R_FMOVE_S  = self::OFS_DATA_MOVE +  2,
        R2R_FMOVE_D  = self::OFS_DATA_MOVE +  3,
        R2R_CLR_L    = self::OFS_DATA_MOVE +  4,
        R2R_CLR_Q    = self::OFS_DATA_MOVE +  5,

        // Register-to-register by default
        R2R_EXG      = self::OFS_DATA_MOVE +  6,
        R2R_FEXG     = self::OFS_DATA_MOVE +  7,
        R2R_SWAP     = self::OFS_DATA_MOVE +  8,
        R2R_SWAP_L   = self::OFS_DATA_MOVE +  9,
        R2R_SWAP_Q   = self::OFS_DATA_MOVE + 10,

        // Generalised <ea> versions
        MOVE_B       = self::OFS_DATA_MOVE + 11,
        MOVE_W       = self::OFS_DATA_MOVE + 12,
        MOVE_L       = self::OFS_DATA_MOVE + 13,
        MOVE_Q       = self::OFS_DATA_MOVE + 14,
        SAVEM        = self::OFS_DATA_MOVE + 15,
        LOADM        = self::OFS_DATA_MOVE + 16,
        FMOVEB_S     = self::OFS_DATA_MOVE + 17,
        FMOVEB_D     = self::OFS_DATA_MOVE + 18,
        FMOVEW_S     = self::OFS_DATA_MOVE + 19,
        FMOVEW_D     = self::OFS_DATA_MOVE + 20,
        FMOVEL_S     = self::OFS_DATA_MOVE + 21,
        FMOVEL_D     = self::OFS_DATA_MOVE + 22,
        FMOVEQ_S     = self::OFS_DATA_MOVE + 23,
        FMOVEQ_D     = self::OFS_DATA_MOVE + 24,
        FMOVES_L     = self::OFS_DATA_MOVE + 25,
        FMOVES_Q     = self::OFS_DATA_MOVE + 26,
        FMOVES_D     = self::OFS_DATA_MOVE + 27,
        FMOVED_L     = self::OFS_DATA_MOVE + 28,
        FMOVED_Q     = self::OFS_DATA_MOVE + 29,
        FMOVED_S     = self::OFS_DATA_MOVE + 30,
        FMOVE_S      = self::OFS_DATA_MOVE + 31,
        FMOVE_D      = self::OFS_DATA_MOVE + 32,
        FINFO_S      = self::OFS_DATA_MOVE + 33,
        FINFO_D      = self::OFS_DATA_MOVE + 34,
        CLR_B        = self::OFS_DATA_MOVE + 35,
        CLR_W        = self::OFS_DATA_MOVE + 36,
        CLR_L        = self::OFS_DATA_MOVE + 37,
        CLR_Q        = self::OFS_DATA_MOVE + 38,
        LINK         = self::OFS_DATA_MOVE + 39,
        UNLK         = self::OFS_DATA_MOVE + 40,
        LEA          = self::OFS_DATA_MOVE + 41,
        PEA          = self::OFS_DATA_MOVE + 42,
        SCC          = self::OFS_DATA_MOVE + 43 // set conditional
    ;

}

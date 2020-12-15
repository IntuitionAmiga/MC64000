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
 * IOther
 *
 * Enumerates the bytecodes for the remaining sundry addressing modes
 */
interface IOther extends IByteCodeGroups {
    const
        INT_IMMEDIATE      = self::OFS_OTHER + 0,

        // Immediate small integer encoded directly, no extension byte.
        INT_SMALL_0        = self::OFS_OTHER + 0,
        INT_SMALL_1        = self::OFS_OTHER + 1,
        INT_SMALL_2        = self::OFS_OTHER + 2,
        INT_SMALL_3        = self::OFS_OTHER + 3,
        INT_SMALL_4        = self::OFS_OTHER + 4,
        INT_SMALL_5        = self::OFS_OTHER + 5,
        INT_SMALL_6        = self::OFS_OTHER + 6,
        INT_SMALL_7        = self::OFS_OTHER + 7,
        INT_SMALL_8        = self::OFS_OTHER + 8,

        // Immediate integer value encoded into extension bytes.
        INT_IMM_BYTE       = self::OFS_OTHER +  9,
        INT_IMM_WORD       = self::OFS_OTHER + 10,
        INT_IMM_LONG       = self::OFS_OTHER + 11,
        INT_IMM_QUAD       = self::OFS_OTHER + 12,

        // Immediate floating point value encoded into extension bytes.
        FLT_IMMEDIATE      = self::OFS_OTHER + 13,

        FLT_IMM_SINGLE     = self::OFS_OTHER + 13,
        FLT_IMM_DOUBLE     = self::OFS_OTHER + 14,

        // Program counter indirect with displacement
        PC_IND_DSP         = self::OFS_OTHER + 15,

        SAME_AS_DEST       = self::OFS_OTHER + 16

    ;
}

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
 * Enumerates bitwise logic instruction opcodes.
 */
interface ILogical extends IByteCodeGroups {
    const
        // Fast path register-to-register versions
        R2R_AND_L  = self::OFS_LOGICAL +  0,
        R2R_AND_Q  = self::OFS_LOGICAL +  1,
        R2R_OR_L   = self::OFS_LOGICAL +  2,
        R2R_OR_Q   = self::OFS_LOGICAL +  3,
        R2R_EOR_L  = self::OFS_LOGICAL +  4,
        R2R_EOR_Q  = self::OFS_LOGICAL +  5,
        R2R_NOT_L  = self::OFS_LOGICAL +  6,
        R2R_NOT_Q  = self::OFS_LOGICAL +  7,
        R2R_LSL_L  = self::OFS_LOGICAL +  8,
        R2R_LSL_Q  = self::OFS_LOGICAL +  9,
        R2R_LSR_L  = self::OFS_LOGICAL + 10,
        R2R_LSR_Q  = self::OFS_LOGICAL + 11,

        AND_B      = self::OFS_LOGICAL + 12,
        AND_W      = self::OFS_LOGICAL + 13,
        AND_L      = self::OFS_LOGICAL + 14,
        AND_Q      = self::OFS_LOGICAL + 15,

        OR_B       = self::OFS_LOGICAL + 16,
        OR_W       = self::OFS_LOGICAL + 17,
        OR_L       = self::OFS_LOGICAL + 18,
        OR_Q       = self::OFS_LOGICAL + 19,

        EOR_B      = self::OFS_LOGICAL + 20,
        EOR_W      = self::OFS_LOGICAL + 21,
        EOR_L      = self::OFS_LOGICAL + 22,
        EOR_Q      = self::OFS_LOGICAL + 23,

        NOT_B      = self::OFS_LOGICAL + 24,
        NOT_W      = self::OFS_LOGICAL + 25,
        NOT_L      = self::OFS_LOGICAL + 26,
        NOT_Q      = self::OFS_LOGICAL + 27,

        LSL_B      = self::OFS_LOGICAL + 28,
        LSL_W      = self::OFS_LOGICAL + 29,
        LSL_L      = self::OFS_LOGICAL + 30,
        LSL_Q      = self::OFS_LOGICAL + 31,

        LSR_B      = self::OFS_LOGICAL + 32,
        LSR_W      = self::OFS_LOGICAL + 33,
        LSR_L      = self::OFS_LOGICAL + 34,
        LSR_Q      = self::OFS_LOGICAL + 35,

        ROL_B      = self::OFS_LOGICAL + 36,
        ROL_W      = self::OFS_LOGICAL + 37,
        ROL_L      = self::OFS_LOGICAL + 38,
        ROL_Q      = self::OFS_LOGICAL + 39,

        ROR_B      = self::OFS_LOGICAL + 40,
        ROR_W      = self::OFS_LOGICAL + 41,
        ROR_L      = self::OFS_LOGICAL + 42,
        ROR_Q      = self::OFS_LOGICAL + 43,

        BCLR_B     = self::OFS_LOGICAL + 44,
        BCLR_W     = self::OFS_LOGICAL + 45,
        BCLR_L     = self::OFS_LOGICAL + 46,
        BCLR_Q     = self::OFS_LOGICAL + 47,

        BSET_B     = self::OFS_LOGICAL + 48,
        BSET_W     = self::OFS_LOGICAL + 49,
        BSET_L     = self::OFS_LOGICAL + 50,
        BSET_Q     = self::OFS_LOGICAL + 51,

        BFFFO      = self::OFS_LOGICAL + 52,
        BFCNT      = self::OFS_LOGICAL + 53
    ;

}

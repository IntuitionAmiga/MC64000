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
        AND_B    = self::OFS_LOGICAL +  0, // 0x70
        AND_W    = self::OFS_LOGICAL +  1,
        AND_L    = self::OFS_LOGICAL +  2,
        AND_Q    = self::OFS_LOGICAL +  3,
        OR_B     = self::OFS_LOGICAL +  4,
        OR_W     = self::OFS_LOGICAL +  5,
        OR_L     = self::OFS_LOGICAL +  6,
        OR_Q     = self::OFS_LOGICAL +  7,
        EOR_B    = self::OFS_LOGICAL +  8,
        EOR_W    = self::OFS_LOGICAL +  9,
        EOR_L    = self::OFS_LOGICAL + 10,
        EOR_Q    = self::OFS_LOGICAL + 11,
        NOT_B    = self::OFS_LOGICAL + 12,
        NOT_W    = self::OFS_LOGICAL + 13,
        NOT_L    = self::OFS_LOGICAL + 14,
        NOT_Q    = self::OFS_LOGICAL + 15,

        LSL_B    = self::OFS_LOGICAL + 16, // 0x80
        LSL_W    = self::OFS_LOGICAL + 17,
        LSL_L    = self::OFS_LOGICAL + 18,
        LSL_Q    = self::OFS_LOGICAL + 19,
        LSR_B    = self::OFS_LOGICAL + 20,
        LSR_W    = self::OFS_LOGICAL + 21,
        LSR_L    = self::OFS_LOGICAL + 22,
        LSR_Q    = self::OFS_LOGICAL + 23,
        ROL_B    = self::OFS_LOGICAL + 24,
        ROL_W    = self::OFS_LOGICAL + 25,
        ROL_L    = self::OFS_LOGICAL + 26,
        ROL_Q    = self::OFS_LOGICAL + 27,
        ROR_B    = self::OFS_LOGICAL + 28,
        ROR_W    = self::OFS_LOGICAL + 29,
        ROR_L    = self::OFS_LOGICAL + 30,
        ROR_Q    = self::OFS_LOGICAL + 31,

        BCLR_B   = self::OFS_LOGICAL + 32, // 0x90
        BCLR_W   = self::OFS_LOGICAL + 33,
        BCLR_L   = self::OFS_LOGICAL + 34,
        BCLR_Q   = self::OFS_LOGICAL + 35,
        BSET_B   = self::OFS_LOGICAL + 36,
        BSET_W   = self::OFS_LOGICAL + 37,
        BSET_L   = self::OFS_LOGICAL + 38,
        BSET_Q   = self::OFS_LOGICAL + 39,
        BFCLR    = self::OFS_LOGICAL + 40,
        BFSET    = self::OFS_LOGICAL + 41,
        BFINS    = self::OFS_LOGICAL + 42,
        BFEXTS   = self::OFS_LOGICAL + 43,
        BFEXTU   = self::OFS_LOGICAL + 44,
        BFFFO    = self::OFS_LOGICAL + 45,
        BFCNT    = self::OFS_LOGICAL + 46,
        BFXXX    = self::OFS_LOGICAL + 47
    ;

}

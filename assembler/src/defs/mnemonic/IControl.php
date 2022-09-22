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
        STOP   = self::OFS_CONTROL +  0, // Execution will halt on encountering 0x00
        HOST   = self::OFS_CONTROL +  1,
        BRA_B  = self::OFS_CONTROL +  2,
        BRA    = self::OFS_CONTROL +  3,
        BSR_B  = self::OFS_CONTROL +  4,
        BSR    = self::OFS_CONTROL +  5,
        JMP    = self::OFS_CONTROL +  6,
        JSR    = self::OFS_CONTROL +  7,
        RTS    = self::OFS_CONTROL +  8,

        // Branch on condition: monadic. Compares <ea> to zero.
        BMC	   = self::OFS_CONTROL +  9,

        // Branch on condition: dyadic. Compares <ea:src> to <ea:dst>
        BDC    = self::OFS_CONTROL + 10,

        // Decrement and branch if not zero
        DBNZ   = self::OFS_CONTROL + 11
    ;
}

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
 * IByteCodeGroups
 *
 * Enumerates the group offsets for different opcode classes.
 */
interface IByteCodeGroups {
    const
        OFS_CONTROL    = 0,
        OFS_DATA_MOVE  = self::OFS_CONTROL    + 15,
        OFS_LOGICAL    = self::OFS_DATA_MOVE  + 45,
        OFS_ARITHMETIC = self::OFS_LOGICAL    + 54,
        OFS_OTHER      = self::OFS_ARITHMETIC + 115,
        OFS_MAX        = 255
    ;
}

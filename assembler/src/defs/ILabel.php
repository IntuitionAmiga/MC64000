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

namespace ABadCafe\MC64K\Defs;

/**
 * ILabel
 *
 * Defines constants pertaining to label definitions.
 */
interface ILabel extends IIdentifier {
    const
        // Label is defined as readable
        IE_READ           = 'r',

        // Label is defined as writable
        IE_WRITE          = 'w',

        // Label is defined as callable
        IE_CALL           = 'x',

        // Match for label IO qualification. Expects any of rwx after space
        IE_SUFFIX_MATCH   = '(?:\s+([' . self::IE_READ . self::IE_WRITE . self::IE_CALL . ']+)){0,1}',
        SUFFIX_CHAR       = ':',
        LOCAL_PREFIX_CHAR = '.',
        LOCAL_MATCH       = '/^\.' . self::BASIC_MATCH . '\:/',
        GLOBAL_MATCH      = '/^'   . self::BASIC_MATCH . '\:/'
    ;
}

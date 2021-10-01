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
 * IInteger
 *
 * Matching for integer literals
 */
interface IInteger {

    /**
     * Straight enumeration of supported bases for integer literals
     */
    const
        BASE_16 = 16,
        BASE_10 = 10,
        BASE_8  = 8,
        BASE_2  = 2
    ;

    /**
     * Regular expression fragments to match integers expressed in each base.
     */
    const
        MATCH_BASE_16 = '(0x|\$)([0-9A-Fa-f]+)',
        MATCH_BASE_10 = '([\+\-]{0,1}\d+)',
        MATCH_BASE_8  = '(0|@)([0-7]+)',
        MATCH_BASE_2  = '(0b|\%)([01]+)'
    ;

    /**
     * Map of the prefix sigil for supported integer literals in non-Base10 formats.
     */
    const MAP_SIGIL_BASE = [
        '0x' => self::BASE_16,
        '$'  => self::BASE_16,
        '0'  => self::BASE_8,
        '@'  => self::BASE_8,
        '0b' => self::BASE_2,
        '%'  => self::BASE_2
    ];

    /**
     * Map of the maximum expected string lengths for unpadded integers in each base, for each machine word size.
     */
    const MAP_BASE_LENGTH = [
        self::BASE_16 => [
            IIntLimits::BYTE => 2,
            IIntLimits::WORD => 4,
            IIntLimits::LONG => 8,
            IIntLimits::QUAD => 16
        ],

        self::BASE_10 => [
            IIntLimits::BYTE => 3,
            IIntLimits::WORD => 5,
            IIntLimits::LONG => 10,
            IIntLimits::QUAD => 20
        ],

        self::BASE_8 => [
            IIntLimits::BYTE => 3,
            IIntLimits::WORD => 6,
            IIntLimits::LONG => 11,
            IIntLimits::QUAD => 22
        ],

        self::BASE_2 => [
            IIntLimits::BYTE => 8,
            IIntLimits::WORD => 16,
            IIntLimits::LONG => 32,
            IIntLimits::QUAD => 64
        ],
    ];

    const SUB_MATCH = '(?:' .
        self::MATCH_BASE_8  . '|' . // Have to match octal first, otherwise will be misinterpreted
        self::MATCH_BASE_10 . '|' .
        self::MATCH_BASE_16 . '|' .
        self::MATCH_BASE_2  . ')';

    const
        IDX_BASE_SIGIL = 0,
        IDX_VALUE_DATA = 1
    ;

    const LITERAL_PREFIX = '#';
}

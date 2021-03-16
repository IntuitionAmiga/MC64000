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
 * IIntLimits
 *
 * Defines constants and limits associated with integer values.
 */
interface IIntLimits {

    const
        BYTE         = 1,
        WORD         = 2,
        LONG         = 4,
        QUAD         = 8,
        MIN_SIGNED   = 0,
        MAX_SIGNED   = 1,
        MAX_UNSIGNED = 2,
        BIN_FORMAT   = 3
    ;

    const WORD_SIZES = [
        self::BYTE => [
            self::MIN_SIGNED   => -128,
            self::MIN_SIGNED   => 127,
            self::MAX_UNSIGNED => 0xFF,
            self::BIN_FORMAT   => 'C'
        ],
        self::WORD => [
            self::MIN_SIGNED   => -32768,
            self::MIN_SIGNED   => 32767,
            self::MAX_UNSIGNED => 0xFFFF,
            self::BIN_FORMAT   => 'v'
        ],
        self::LONG => [
            self::MIN_SIGNED   => -2147483648,
            self::MIN_SIGNED   => 2147483647,
            self::MAX_UNSIGNED => 0xFFFFFFFF,
            self::BIN_FORMAT   => 'V'
        ],
        self::QUAD => [
            self::MIN_SIGNED   => PHP_INT_MIN,
            self::MIN_SIGNED   => PHP_INT_MAX,
            self::MAX_UNSIGNED => -1, // this is a hack, we mean all bits 1,
            self::BIN_FORMAT   => 'P'
        ]
    ];

}

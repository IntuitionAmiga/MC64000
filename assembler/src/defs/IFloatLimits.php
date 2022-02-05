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
 * IFloatLimits
 *
 * Defines constants and limits associated with IEEE754 values.
 */
interface IFloatLimits {

    const
        SINGLE       = 4,
        DOUBLE       = 8,
        MIN_ABS      = 0,
        MAX_ABS      = 1
    ;

    const
        // Limits for normalised numbers. Denormalised numbers can go smaller but here be dragons.
        SINGLE_MAX_ABS    = 3.40282e38,
        SINGLE_MIN_ABS    = 1.17549e-38,
        DOUBLE_MAX_ABS    = PHP_FLOAT_MAX,
        DOUBLE_MIN_ABS    = PHP_FLOAT_MIN
    ;

    /**
     * Map of number properties per word type
     */
    const WORD_SIZES = [
        self::SINGLE => [
            self::MIN_ABS => self::SINGLE_MIN_ABS,
            self::MAX_ABS => self::SINGLE_MAX_ABS
        ],
        self::DOUBLE => [
            self::MIN_ABS => self::DOUBLE_MIN_ABS,
            self::MAX_ABS => self::DOUBLE_MAX_ABS
        ]
    ];
}

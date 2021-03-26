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
 * IIdentifier
 *
 * Defines constants pertaining basic identifier matches:
 *
 *     1. Length must be between 1-256
 *     2. First character must be a letter or an underscore
 *     3. Subsequent characters can be a letter, number or underscore
 */
interface IIdentifier {
    const
        /**
         * @const int MIN_LENGTH
         */
        MIN_LENGTH = 1,

        /**
         * @const int MIN_LENGTH
         */
        MAX_LENGTH = 256,

        /**
         * @const string BASIC_MATCH
         */
        BASIC_MATCH = '[A-Za-z_][A-Za-z0-9_]{0,' . (self::MAX_LENGTH - 1) . '}'
    ;
}

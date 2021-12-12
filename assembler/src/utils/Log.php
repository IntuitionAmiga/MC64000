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

namespace ABadCafe\MC64K\Utils;

use function \fprintf;

/**
 * Basic Log Facility
 */
class Log {

    private static int $iLine  = 0;

    /**
     * Variadic template string message
     *
     * @param string $sFormat
     * @param int|float|string  $aVarArgs
     */
    public static function printf(string $sFormat, ...$aVarArgs): void {
        fprintf(
            STDERR, ('#%d ' . $sFormat . "\n"), self::$iLine++,
            ...$aVarArgs
        );
    }

    /**
     * Simple direct message
     *
     * @param string $sMessage
     */
    public static function write(string $sMessage): void {
        fprintf(
            STDERR, "#%d %s\n", self::$iLine++, $sMessage
        );
    }
}

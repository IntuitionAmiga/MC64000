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

namespace ABadCafe\MC64K\Parser\Utils;

/**
 * Hex
 *
 * Utilities for dealing with hexadecimal values
 */
class Hex {

    /**
     * Convert a hex string to a signed 8-bit value
     *
     * @param  string $sHex
     * @return int
     */
    public static function stringToInt8(string $sHex) : int {
        $iValue = (int)base_convert(substr($sHex, -2), 16, 10);
        return $iValue & 0x80 ? $iValue -= 1<<8 : $iValue;
    }

    /**
     * Convert a hex string to a signed 16-bit value
     *
     * @param  string $sHex
     * @return int
     */
    public static function stringToInt16(string $sHex) : int {
        $iValue = (int)base_convert(substr($sHex, -4), 16, 10);
        return $iValue & 0x8000 ? $iValue -= 1<<16 : $iValue;
    }

    /**
     * Convert a hex string to a signed 32-bit value
     *
     * @param  string $sHex
     * @return int
     */
    public static function stringToInt32(string $sHex) : int {
        $iValue = (int)base_convert(substr($sHex, -8), 16, 10);
        return $iValue & 0x80000000 ? $iValue -= 1<<32 : $iValue;
    }
}

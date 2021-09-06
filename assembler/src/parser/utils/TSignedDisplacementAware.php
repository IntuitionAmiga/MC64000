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
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs\IIntLimits;

use function \substr, \strlen;

/**
 * TSignedDisplacementAware
 *
 * Utilities for parsing 32-bit signed displacement components of effective addresses and branches.
 */
trait TSignedDisplacementAware {

    /**
     * @param  string $sDisplacement
     * @param  bool   $bHex
     * @return int
     * @throws \RangeException
     */
    private function parseDisplacement(string $sDisplacement, bool $bHex): int {
        if ($bHex) {
            return $this->parseHexDisplacement($sDisplacement);
        } else {
            return $this->parseDecimalDisplacement($sDisplacement);
        }
    }

    /**
     * @param  string $sDisplacement
     * @return int
     * @throws \RangeException
     */
    private function parseHexDisplacement(string $sDisplacement): int {
        $sDisplacement = substr($sDisplacement, 2);
        if (strlen($sDisplacement) <= 8) {
            return Parser\Utils\Hex::stringToInt32($sDisplacement);
        }
        throw new \RangeException('Displacement ' . $sDisplacement . ' is too large');
    }

    /**
     * @param  string $sDisplacement
     * @return int
     * @throws \RangeException
     */
    private function parseDecimalDisplacement(string $sDisplacement): int {
        $iDisplacement = (int)$sDisplacement;
        // Test against limits for signed 32-bit integer
        if ($iDisplacement >= IIntLimits::LONG_MIN_SIGNED && $iDisplacement <= IIntLimits::LONG_MAX_SIGNED) {
            return $iDisplacement;
        }
        throw new \RangeException('Displacement ' . $sDisplacement . ' is too large');
    }
}

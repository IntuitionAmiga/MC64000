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
use ABadCafe\MC64K\Defs;

use function \is_finite;

/**
 * FloatingPoint
 *
 * Utility class for parsing floating point strings for different word sizes.
 */
class FloatingPoint implements Defs\IFloatLimits {

    const SUB_MATCH = '((?:[-+]{0,1}\d+)\.\d*(?:[eE][-+]{0,1}\d+){0,1})([sSdD]{0,1})';

    const
        MATCHED_VALUE = 1,
        MATCHED_FSIZE = 2
    ;

    const MIN_MATCH = '/^' . self::SUB_MATCH . '$/';

    /**
     * Parses a floating point value for the specified word size. The double precision equivalent value is returned but
     * will only have single precision and range for singles.
     *
     * @param  string $sInput            - the literal string to parse
     * @param  int    $iWordSize         - the target word size (4 or 8)
     * @param  string $sPrefix           - Optional literal prefix, e.g. #
     * @return float
     * @throws \OutOfRangeException      - for invalid word sizes.
     * @throws \InvalidArgumentException - for malformed input strings.
     * @throws \RangeException           - for well-formed numbers outside the range of the word size.
     */
    public static function parseLiteral(string $sInput, int $iWordSize, string $sPrefix=''): float {
        if (!isset(self::WORD_SIZES[$iWordSize])) {
            throw new \OutOfRangeException('Invalid word size '. $iWordSize);
        }
        $aMatches = self::match($sInput, $sPrefix);
        if (empty($aMatches)) {
            throw new \InvalidArgumentException('Invalid literal format ' . $sInput);
        }

        return self::parseMatch($aMatches, $iWordSize);
    }

    /**
     * Perform the basic match to isolate the string representation and the implied base.
     * Split out like this so that clients can test the match and optionally process the result later.
     * If the string parses, an array of two strings for the base indicator and numeric value are returned,
     * otherwise null.
     *
     * @param  string $sInput
     * @param  string $sPrefix
     * @return string[]|null
     */
    public static function match(string $sInput, string $sPrefix): ?array {
        $aMatches = [];
        if (!preg_match('/^' . $sPrefix . self::SUB_MATCH . '$/', $sInput, $aMatches)) {
            return null;
        }
        return $aMatches;
    }

    /**
     * Parses the result of a previous match() call.
     *
     * @param  string[] $aMatches
     * @param  int      $iWordSize
     * @return float
     */
    public static function parseMatch(array $aMatches, int $iWordSize): float {
        $fValue = doubleval($aMatches[self::MATCHED_VALUE]);
        if (false === is_finite($fValue)) {
            throw new \RangeException('Could not encode ' . $aMatches[self::MATCHED_VALUE]);
        }
        if (
            self::SINGLE == $iWordSize ||
            's' === $aMatches[self::MATCHED_FSIZE] ||
            'S' === $aMatches[self::MATCHED_FSIZE]
        ) {
            $fAbs = abs($fValue);
            if (
                $fAbs !== 0.0 && (
                    $fAbs > self::SINGLE_MAX_ABS ||
                    $fAbs < self::SINGLE_MIN_ABS
                )
            ) {
                throw new \RangeException('Cannot encode single precision ' . $aMatches[self::MATCHED_VALUE]);
            }
        }
        return $fValue;
    }

}

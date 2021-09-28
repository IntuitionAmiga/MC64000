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

use function \trim, \base_convert, \substr, \strlen, \bcadd, \bcmul, \bcdiv, \bcmod, \bccomp;

/**
 * Integer
 */
class Integer implements Defs\IInteger, Defs\IIntLimits {

    const MIN_MATCH = '/^' . self::SUB_MATCH . '$/';

    /**
     * Parses a literal integer value in any one of the supported bases, for the specified word size. The
     * signed equivalent is returned, based on the word size, e.g. a 1-byte input string of "255" is
     * returned as -1, where as the same input requested as a 2-byte word will be returned as 255.
     */
    public static function parseLiteral(string $sInput, int $iWordSize): int {
        if (!isset(self::WORD_SIZES[$iWordSize])) {
            throw new \OutOfRangeException('Invalid word size '. $iWordSize);
        }
        $aMatches = self::match($sInput);
        $iBase    = self::MAP_SIGIL_BASE[$aMatches[self::IDX_BASE_SIGIL]] ?? self::BASE_10;
        $sValue   = $aMatches[self::IDX_VALUE_DATA];

        self::assertLength($sValue, $iBase, $iWordSize);

        // Quads require careful handling. All smaller sizes can be handled the same way.
        if ($iWordSize === self::QUAD) {

        } else {
            return self::rangeLimitSmall(
                self::baseConvertSmall(
                    $sValue,
                    $iBase,
                    $iWordSize
                ),
                $iWordSize
            );
        }
        return 0;
    }

    /**
     * For 8-32 bit values, perform a string to integer conversion, complete with any required base conversion.
     *
     * @param  string $sValue
     * @param  int    $iBase
     * @param  int    $iWordSize
     * @return int
     */
    private static function baseConvertSmall(string $sValue, int $iBase, int $iWordSize): int {
        // Easy
        if (self::BASE_10 === $iBase) {
            return (int)$sValue;
        }
        // Convert the base. The result is always unsigned, so deal with that.
        $iUnsigned = (int)base_convert($sValue, $iBase, self::BASE_10);
        $iSignBit  = 1 << (($iWordSize << 3) - 1);
        if ($iUnsigned & $iSignBit) {
            return $iUnsigned - ($iSignBit << 1);
        }
        return $iUnsigned;
    }

    /**
     * For 8-32 bit values, ensure the output is in the range MIN_SIGNED to MAX_SIGNED or
     * throw an exception if that cannot be achieved.
     *
     * @param  int $iIntermediate
     * @param  int $iWordSize
     * @return int
     * @throws \RangeException
     */
    private static function rangeLimitSmall(int $iIntermediate, int $iWordSize): int {
        if (
            $iIntermediate < self::WORD_SIZES[$iWordSize][self::MIN_SIGNED] ||
            $iIntermediate > self::WORD_SIZES[$iWordSize][self::MAX_UNSIGNED]
        ) {
            throw new \RangeException("Value " . $iIntermediate. " is not in range");
        }

        if ($iIntermediate > self::WORD_SIZES[$iWordSize][self::MAX_SIGNED]) {
            $iIntermediate -= self::WORD_SIZES[$iWordSize][self::MAX_UNSIGNED] + 1;
        }
        return $iIntermediate;
    }

    /**
     * For string representation, ensure the character length is within the expected size for the
     * base and word size. This is just to early out on more expensive checks later.
     *
     * @param  string $sValue
     * @param  int $iBase
     * @param  int $iWordSize
     * @throws \LengthException
     */
    private static function assertLength(string $sValue, int $iBase, int $iWordSize) {
        $sTemp = $iBase === self::BASE_10 ? ltrim($sValue, '-+') : $sValue;
        if (strlen($sTemp) > self::MAP_BASE_LENGTH[$iBase][$iWordSize]) {
            throw new \LengthException(
                'Too many digits in Base-' . $iBase .
                ' value ' . $sValue .
                ' for word size ' . $iWordSize
            );
        }
    }

    /**
     * Perform the basic match to isolate the string representation and the implied base.
     */
    private static function match(string $sInput): array {
        $aMatches = [];
        if (!preg_match(self::MIN_MATCH, $sInput, $aMatches)) {
            throw new \InvalidArgumentException('Invalid literal integer format ' . $sInput);
        }

        // We are only interested in the last two parts of the match.
        return array_slice($aMatches, -2, 2);
    }


    /**
     * Replaces base_convert() with code suitable for arbitrarily large numbers by processing the string representation
     * directly.
     *
     * @param  string $sInput
     * @param  int    $iFromBase
     * @return string
     */
    public static function fromBase(string $sInput, int $iFromBase): string {
        $sInput = trim($sInput);
        if ($iFromBase != self::BASE_10) {
            $iLength   = strlen($sInput);
            $sOutput   = '0';
            $sFromBase = (string)$iFromBase;
            for ($i = 0; $i < $iLength; ++$i) {
                $sDigit  = base_convert($sInput[$i], $iFromBase, self::BASE_10);
                $sOutput = bcadd(bcmul($sOutput, $sFromBase), $sDigit);
            }
            return $sOutput;
        }
        return $sInput;
    }
}

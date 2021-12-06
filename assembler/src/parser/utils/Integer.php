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

use function \preg_match, \array_slice, \ltrim, \trim, \base_convert, \substr, \strlen, \bcadd, \bcsub, \bcmul, \bcdiv, \bcmod, \bccomp;

/**
 * Integer
 *
 * Utility class for parsing integer strings in different base/format for different word sizes.
 *
 *
 */
class Integer implements Defs\IInteger, Defs\IIntLimits {

    const MIN_MATCH = '/^' . self::SUB_MATCH . '$/';

    private const
        BC_MIN_QUAD_SIGNED   = '-9223372036854775808',
        BC_MAX_QUAD_SIGNED   = '9223372036854775807',
        BC_MAX_QUAD_UNSIGNED = '18446744073709551615',
        BC_2_TO_64           = '18446744073709551616',

        CHAR_TO_NYBBLE = [
            '0' => 0, '1' => 1, '2' => 2, '3' => 3,
            '4' => 4, '5' => 5, '6' => 6, '7' => 7,
            '8' => 8, '9' => 9,
            'a' => 10, 'A' => 10, 'b' => 11, 'B' => 11,
            'c' => 12, 'C' => 12, 'd' => 13, 'D' => 13,
            'e' => 14, 'E' => 14, 'f' => 15, 'F' => 15,
        ]
    ;

    /**
     * Parses a literal integer value in any one of the supported bases, for the specified word size. The
     * signed equivalent is returned, based on the word size, e.g. a 1-byte input string of "255" is
     * returned as -1, where as the same input requested as a 2-byte word will be returned as 255.
     *
     * @param  string $sInput            - the literal string to parse
     * @param  int    $iWordSize         - the target word size (1, 2, 4 or 8)
     * @param  string $sPrefix           - Optional literal prefix, e.g. #
     * @return int
     * @throws \OutOfRangeException      - for invalid word sizes.
     * @throws \InvalidArgumentException - for malformed input strings.
     * @throws \RangeException           - for well-formed numbers outside the range of the word size.
     */
    public static function parseLiteral(string $sInput, int $iWordSize, string $sPrefix=''): int {
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

        // We are only interested in the last two parts of the match.
        return array_slice($aMatches, -2, 2);
    }

    /**
     * Parses the result of a previous match() call.
     *
     * @param  string[] $aMatches
     * @param  int      $iWordSize
     * @return int
     */
    public static function parseMatch(array $aMatches, int $iWordSize): int {

        $iBase    = self::MAP_SIGIL_BASE[$aMatches[self::IDX_BASE_SIGIL]] ?? self::BASE_10;
        $sValue   = $aMatches[self::IDX_VALUE_DATA];

        self::assertLength($sValue, $iBase, $iWordSize);

        // Quads require careful handling. All smaller sizes can be handled the same way.
        if ($iWordSize === self::QUAD) {
            switch ($iBase) {
                case self::BASE_2:  return self::binToQuad($sValue);
                case self::BASE_8:  return self::octToQuad($sValue);
                case self::BASE_10: return self::decToQuad($sValue);
                case self::BASE_16: return self::hexToQuad($sValue);
            }
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
        // Convert the base to decimal. The result is always unsigned, so deal with that.
        $iUnsigned = (int)base_convert($sValue, $iBase, self::BASE_10);
        $iSignBit  = 1 << (($iWordSize << 3) - 1);
        if ($iUnsigned & $iSignBit) {
            return $iUnsigned - ($iSignBit << 1);
        }
        return $iUnsigned;
    }

    /**
     * For 8-32 bit values, ensure the output is in the range MIN_SIGNED to MAX_SIGNED or
     * throw an exception if that cannot be achieved. Input values are allowed between
     * MIN_SIGNED and MAX_UNSIGNED. Where the value is between MAX_SIGNED and MAX_UNSIGNED
     * MAX_UNSIGNED is deducted to derive the appropriate modular value.
     *
     * @param  int $iValue
     * @param  int $iWordSize
     * @return int
     * @throws \RangeException
     */
    private static function rangeLimitSmall(int $iValue, int $iWordSize): int {
        if (
            $iValue < self::WORD_SIZES[$iWordSize][self::MIN_SIGNED] ||
            $iValue > self::WORD_SIZES[$iWordSize][self::MAX_UNSIGNED]
        ) {
            throw new \RangeException("Value " . $iValue. " is not in range");
        }

        // Modular clamp
        if ($iValue > self::WORD_SIZES[$iWordSize][self::MAX_SIGNED]) {
            $iValue -= self::WORD_SIZES[$iWordSize][self::MAX_UNSIGNED] + 1;
        }
        return $iValue;
    }

    /**
     * For 64-bit values, ensure the output is in the range MIN_SIGNED to MAX_SIGNED or
     * throw an exception if that cannot be achieved. Input values are allowed between
     * MIN_SIGNED and MAX_UNSIGNED. Where the value is between MAX_SIGNED and MAX_UNSIGNED
     * MAX_UNSIGNED is deducted to derive the appropriate modular value.
     *
     * Since 64-bit PHP has no explicit understanding of unsigned arithmetic, the necessary
     * range comparisons are performed using bcmath, before conversion.
     *
     * @param  string $sValue
     * @return int
     * @throws \RangeException
     */
    private static function decToQuad(string $sValue): int {
        if (
            bccomp($sValue, self::BC_MIN_QUAD_SIGNED) < 0 ||
            bccomp($sValue, self::BC_MAX_QUAD_UNSIGNED) > 0
        ) {
            throw new \RangeException("Value " . $sValue. " is not in range");
        }

        // Modular clamp
        if (bccomp($sValue, self::BC_MAX_QUAD_SIGNED) > 0) {
            $sValue = bcsub($sValue, self::BC_2_TO_64);
        }
        return (int)$sValue;
    }

    /**
     * Converts a 64-bit binary string to an integer.
     *
     * @param  string $sValue
     * @return int
     */
    private static function binToQuad(string $sValue): int {
        $iLen   = strlen($sValue)-1;
        $iValue = 0;
        for ($i = 0; $i < $iLen; ++$i) {
            $iValue |= (int)$sValue[$i];
            $iValue <<= 1;
        }
        $iValue |= (int)$sValue[$i];
        return $iValue;
    }

    /**
     * Converts a 64-bit octal string to an integer. When the string length is at the maximum length,
     * the first digit must be either 0 or 1 as any larger value would require more than 64 bits. In this
     * case RangeException is thrown.
     *
     * @param  string $sValue
     * @return int
     * @throws \RangeException
     */
    private static function octToQuad(string $sValue): int {
        $iLen = strlen($sValue);
        if ((int)$sValue[0] > 1 && self::MAP_BASE_LENGTH[self::BASE_8][self::QUAD] == $iLen) {
            throw new \RangeException("Value " . $sValue. " is not in range");
        }
        --$iLen;
        $iValue = 0;
        for ($i = 0; $i < $iLen; ++$i) {
            $iValue |= (int)$sValue[$i];
            $iValue <<= 3;
        }
        $iValue |= (int)$sValue[$i];
        return $iValue;
    }

    /**
     * Converts a 64-bit hexadecimal string to an integer.
     *
     * @param  string $sValue
     * @return int
     */
    private static function hexToQuad(string $sValue): int {
        $iLen   = strlen($sValue)-1;
        $iValue = 0;
        for ($i = 0; $i < $iLen; ++$i) {
            $iValue |= self::CHAR_TO_NYBBLE[$sValue[$i]];
            $iValue <<= 4;
        }
        $iValue |= self::CHAR_TO_NYBBLE[$sValue[$i]];
        return $iValue;
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
    private static function assertLength(string $sValue, int $iBase, int $iWordSize): void {
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

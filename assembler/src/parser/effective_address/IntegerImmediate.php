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

namespace ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\EffectiveAddress;
use ABadCafe\MC64K\Parser;

/**
 * IntegerImmediate
 *
 * Effective address parser for integer immediates.
 */
class IntegerImmediate implements IParser, EffectiveAddress\IOther {

    use TOperationSizeAware;

    const
        MATCH         = '/^#' . self::D32 . '$/',
        MIN_INT_SMALL = 0,
        MAX_INT_SMALL = 8,
        MATCHED_VALUE = 1,
        MATCHED_HEX   = 2
    ;

    const INT_RANGES = [
        self::INT_IMM_BYTE => [-128, 127, 'c'],
        self::INT_IMM_WORD => [-32768, 32767, 'v'],
        self::INT_IMM_LONG => [-2147483648, 2147483647, 'V'],
        self::INT_IMM_QUAD => [PHP_INT_MIN, PHP_INT_MAX, 'P']
    ];

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        if (preg_match(self::MATCH, $sSource, $aMatches)) {
            $sImmediate = $aMatches[self::MATCHED_VALUE];
            if (isset($aMatches[self::MATCHED_HEX])) {
                return $this->encodeHexadecimalIntegerImmediate(substr($sImmediate, 2));
            } else {
                return $this->encodeDecimalIntegerImmediate((int)$sImmediate);
            }
        }
        return null;
    }

    /**
     * Encodes a signed integer using the smallest possible output effective address mode.
     *
     * @param  int    $iImmediate
     * @return string bytecode
     * @throws \RangeException
     */
    private function encodeDecimalIntegerImmediate(int $iImmediate) : string {
        if ($iImmediate >= self::MIN_INT_SMALL && $iImmediate <= self::MAX_INT_SMALL) {
            return chr(self::INT_SMALL_0 + $iImmediate);
        } else {
            foreach (self::INT_RANGES as $iMode => $aRange) {
                if ($iImmediate >= $aRange[0] && $iImmediate <= $aRange[1]) {
                    return chr($iMode) . pack($aRange[2], $iImmediate);
                }
            }
        }
        throw new \RangeException('Could not encode ' . (string)$iImmediate);
    }

    /**
     * Hex immediates are parsed a bit specially:
     *
     * 0xFF will be encoded as -1, as a byte
     * 0x0FF will be encoded as 255, as a word
     * 0xFFFF will be encoded as -1, as a byte
     */
    private function encodeHexadecimalIntegerImmediate(string $sImmediate) : string {
        $iLength = strlen($sImmediate);

        if ($iLength < 1 || $iLength > 16) {
            throw new \RangeException('Could not encode ' . $sImmediate);
        }
        if ($iLength <= 2) {
            return $this->encodeDecimalIntegerImmediate(Parser\Utils\Hex::stringToInt8($sImmediate));
        }
        if ($iLength <= 4) {
            return $this->encodeDecimalIntegerImmediate(Parser\Utils\Hex::stringToInt16($sImmediate));
        }
        if ($iLength <= 8) {
            return $this->encodeDecimalIntegerImmediate(Parser\Utils\Hex::stringToInt32($sImmediate));
        }
        if ($iLength <= 15) {
            $sImmediate = str_pad($sImmediate, 16, '0', STR_PAD_LEFT);
        }

        $iLower = Parser\Utils\Hex::stringToInt32(substr($sImmediate, 8, 8));
        $iUpper = Parser\Utils\Hex::stringToInt32(substr($sImmediate, 0, 8));

        // The upper half may not be significant, depending on what the lower half is
        if ($iLower < 0) {
            if ($iUpper === -1) {
                // Upper is just sign extension of the lower half
                return $this->encodeDecimalIntegerImmediate($iLower);
            }
        } else {
            if ($iUpper === 0) {
                // Upper is just zero pad of the lower half
                return $this->encodeDecimalIntegerImmediate($iLower);
            }
        }
        return chr(self::INT_IMM_QUAD) . pack('V2', $iLower, $iUpper);
    }
}

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

namespace ABadCafe\MC64K\Parser\EffectiveAddress;

use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs\EffectiveAddress;

class Immediate implements MC64K\IParser, IMatch, EffectiveAddress\IOther {

    const MATCHES = [
        'parseInteger' => '/^#' . self::D32 . '$/',
        'parseFloat' => '/^#(([-+]{0,1}\d+)\.\d*([eE][-+]{0,1}\d+){0,1})$/'
    ];

    const
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

    public function parse(string $sSource) : ?string {
        foreach (self::MATCHES as $sCallback => $sRegex) {
            if (preg_match($sRegex, $sSource, $aMatches)) {
                return $this->{$sCallback}($aMatches);
            }
        }
        return null;
    }

    /**
     * Extract the integer match and determine the densest immediate mode it can fit into.
     *
     * @param string[] $aMatches - regex output
     * @return string - bytecode for complete EA / immediate
     */
    private function parseInteger(array $aMatches) : string {
        $sImmediate = $aMatches[self::MATCHED_VALUE];
        if (isset($aMatches[self::MATCHED_HEX])) {
            return $this->encodeHexadecimalIntegerImmediate(substr($sImmediate, 2));
        } else {
            return $this->encodeDecimalIntegerImmediate((int)$sImmediate);
        }
    }

    /**
     * Parses a signed decimal value
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

    private function encodeHexadecimalIntegerImmediate(string $sImmediate) : string {

        switch (strlen($sImmediate)) {
            case 1:
            case 2:
                $iValue = (int)base_convert($sImmediate, 16, 10);
                if ($iValue & 0x80) {
                    $iValue -= 1<<8;
                }
                return $this->encodeDecimalIntegerImmediate($iValue);
            case 3:
            case 4:
                $iValue = (int)base_convert($sImmediate, 16, 10);
                if ($iValue & 0x8000) {
                    $iValue -= 1<<16;
                }
                return $this->encodeDecimalIntegerImmediate($iValue);
            case 5:
            case 6:
            case 7:
            case 8:
                $iValue = (int)base_convert($sImmediate, 16, 10);
                if ($iValue & 0x8000000) {
                    $iValue -= 1<<32;
                }
                return $this->encodeDecimalIntegerImmediate($iValue);
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                // TODO - See if the value is something in the range of a 32-bit first.
                // Otherwise do our own direct conversion as there are issues dealing with
                // the interpretation of sign.
            default:
                throw new \RangeException('Could not encode ' . $sImmediate);

        }
    }

    private function parseFloat(array $aMatches) : string {

    }
}

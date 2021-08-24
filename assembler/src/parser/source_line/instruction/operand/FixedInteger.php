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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Parser;

use function \preg_match, \substr, \pack, \strlen, \str_pad;

/**
 * FixedInteger
 *
 * Operand parser for fixed integer values.
 */
class FixedInteger implements MC64K\IParser, Defs\IIntLimits {

    const
        MATCH_NUMERIC = '/^#' . Parser\EffectiveAddress\IParser::D32 . '$/',
        MATCHED_VALUE = 1,
        MATCHED_HEX   = 2
    ;

    private int $iSize;

    public function __construct(int $iSize) {
        if (!isset(self::WORD_SIZES[$iSize])) {
            throw new \UnexpectedValueException();
        }
        $this->iSize = $iSize;
    }

    /**
     * @inheritDoc
     *
     * If the resolved displacement value is zero (branch to next instruction)
     * the code will be folded out.
     *
     * @throws CodeFoldException
     */
    public function parse(string $sSource) : ?string {
        if (preg_match(self::MATCH_NUMERIC, $sSource, $aMatches)) {
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
     * @param  int $iImmediate
     * @return string
     */
    private function encodeDecimalIntegerImmediate(int $iImmediate) : string {
        return pack(self::WORD_SIZES[$this->iSize][self::BIN_FORMAT], $iImmediate);
    }

    /**
     * @param  string $sImmediate
     * @return string
     */
    private function encodeHexadecimalIntegerImmediate(string $sImmediate) : string {
        $iHexLength = strlen($sImmediate);
        if ($iHexLength > ($this->iSize * 2)) {
            throw new \RangeException('Could not encode ' . $sImmediate, ', too large for expected operand size ' . $this->iSize);
        }
        $iImmediate = 0;
        switch ($this->iSize) {
            case self::BYTE:
                $iImmediate = Parser\Utils\Hex::stringToInt8($sImmediate);
                break;
            case self::WORD:
                $iImmediate = Parser\Utils\Hex::stringToInt16($sImmediate);
                break;
            case self::LONG:
                $iImmediate = Parser\Utils\Hex::stringToInt32($sImmediate);
                break;
            case self::QUAD:
                if ($iHexLength <= 15) {
                    $sImmediate = str_pad($sImmediate, 16, '0', STR_PAD_LEFT);
                }
                $iLower = Parser\Utils\Hex::stringToInt32(substr($sImmediate, 8, 8));
                $iUpper = Parser\Utils\Hex::stringToInt32(substr($sImmediate, 0, 8));

                // Gratuitous hack to extract the integer representation that was present because base_convert() returns
                // a numeric string of the unsigned value.
                return pack('V2', $iLower, $iUpper);
        }
        return $this->encodeDecimalIntegerImmediate($iImmediate);
    }
}

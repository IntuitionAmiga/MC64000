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
use ABadCafe\MC64K\Defs\IIntLimits;
use ABadCafe\MC64K\Parser;

use function \preg_match, \substr, \chr, \pack, \strlen, \str_pad, \unpack;

/**
 * IntegerImmediate
 *
 * Effective address parser for integer immediates.
 */
class IntegerImmediate implements IParser, EffectiveAddress\IOther {

    use TOperationSizeAware;

    const PREFIX = '#';

    const

        MATCH         = '/^#' . self::D32 . '$/',
        MIN_INT_SMALL = 0,
        MAX_INT_SMALL = 8,
        MATCHED_VALUE = 1,
        MATCHED_HEX   = 2
    ;

    const INT_RANGES = [
        self::INT_IMM_BYTE => [IIntLimits::BYTE_MIN_SIGNED, IIntLimits::BYTE_MAX_SIGNED, IIntLimits::BYTE_BIN_FORMAT],
        self::INT_IMM_WORD => [IIntLimits::WORD_MIN_SIGNED, IIntLimits::WORD_MAX_SIGNED, IIntLimits::WORD_BIN_FORMAT],
        self::INT_IMM_LONG => [IIntLimits::LONG_MIN_SIGNED, IIntLimits::LONG_MAX_SIGNED, IIntLimits::LONG_BIN_FORMAT],
        self::INT_IMM_QUAD => [IIntLimits::QUAD_MIN_SIGNED, IIntLimits::QUAD_MAX_SIGNED, IIntLimits::QUAD_BIN_FORMAT]
    ];

    /**
     * @var int|null $iLastParsed
     */
    private ?int $iLastParsed = null;

    /**
     * @return int|null
     */
    public function getLastParsed(): ?int {
        return $this->iLastParsed;
    }

    /**
     * @inheritDoc
     */
    public function hasSideEffects(): bool {
        return false;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {
        $this->iLastParsed = null;
        $aMatches = Parser\Utils\Integer::match($sSource, self::PREFIX);
        if (!empty($aMatches)) {
            $iSize = $this->iOperationSize ?? IIntLimits::QUAD;
            $this->iLastParsed = Parser\Utils\Integer::parseMatch($aMatches, $iSize);
            return $this->encodeValue($this->iLastParsed);
        }
        return null;
    }

    /**
     * Encode an integer value into the smallest possible immediate mode bytecode.
     *
     * @param  int $iValue
     * @return string
     * @throws \RangeException
     */
    private function encodeValue(int $iValue): string {
        if ($iValue >= self::MIN_INT_SMALL && $iValue <= self::MAX_INT_SMALL) {
            return chr(self::INT_SMALL_0 + $iValue);
        } else {
            foreach (self::INT_RANGES as $iMode => $aRange) {
                if ($iValue >= $aRange[0] && $iValue <= $aRange[1]) {
                    return chr($iMode) . pack($aRange[2], $iValue);
                }
            }
        }
        $this->iLastParsed = null;
        throw new \RangeException('Could not encode ' . (string)$iValue);
    }
}

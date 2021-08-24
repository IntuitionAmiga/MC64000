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

use function \preg_match, \doubleval, \is_nan, \abs, \chr, \pack;

/**
 * FloatImmediate
 *
 * Effective address parser for floating point immediates.
 */
class FloatImmediate implements IParser, EffectiveAddress\IOther {

    use TOperationSizeAware;

    const
        MATCH         = '/^#((?:[-+]{0,1}\d+)\.\d*(?:[eE][-+]{0,1}\d+){0,1})([sSdD]{0,1})$/',
        MATCHED_VALUE = 1,
        MATCHED_FSIZE = 2,
        MAX_SINGLE    = 3.4e38,
        MIN_SINGLE    = 1.18e-38
    ;

    /**
     * @var float|null $iLastParsed
     */
    private ?float $fLastParsed = null;

    /**
     * @return float|null
     */
    public function getLastParsed() : ?float {
        return $this->fLastParsed;
    }

    /**
     * @inheritDoc
     */
    public function hasSideEffects() : bool {
        return false;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        $this->fLastParsed = null;
        if (preg_match(self::MATCH, $sSource, $aMatches)) {
            $fValue = doubleval($aMatches[self::MATCHED_VALUE]);
            if (is_nan($fValue)) {
                throw new \RangeException('Could not encode ' . $aMatches[self::MATCHED_VALUE]);
            }

            $this->fLastParsed = $fValue;

            // If the operation size or immidate requests a single, we need to validate it's in range.
            if (
                $this->iOperationSize == 4             ||
                's' === $aMatches[self::MATCHED_FSIZE] ||
                'S' === $aMatches[self::MATCHED_FSIZE]
            ) {
                $fAbs = abs($fValue);
                if (
                    $fAbs !== 0.0 && (
                        $fAbs > self::MAX_SINGLE ||
                        $fAbs < self::MIN_SINGLE
                    )
                ) {
                    throw new \RangeException('Cannot encode single precision ' . $aMatches[self::MATCHED_VALUE]);
                }
                return chr(self::FLT_IMM_SINGLE) . pack('g', $fValue);
            } else {
                return chr(self::FLT_IMM_DOUBLE) . pack('e', $fValue);
            }
        }
        return null;
    }
}

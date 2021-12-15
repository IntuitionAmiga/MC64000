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
use ABadCafe\MC64K\Defs\IFloatLimits;
use ABadCafe\MC64K\Parser;

use function \preg_match, \doubleval, \is_nan, \abs, \chr, \pack;

/**
 * FloatImmediate
 *
 * Effective address parser for floating point immediates.
 */
class FloatImmediate implements IParser, EffectiveAddress\IOther {

    use TOperationSizeAware;

    const PREFIX = '#';

    /**
     * @var float|null $iLastParsed
     */
    private ?float $fLastParsed = null;

    /**
     * @return float|null
     */
    public function getLastParsed(): ?float {
        return $this->fLastParsed;
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
        $this->fLastParsed = null;
        $aMatches = Parser\Utils\FloatingPoint::match($sSource, self::PREFIX);
        if (!empty($aMatches)) {
            $iSize = $this->iOperationSize ?? IFloatLimits::DOUBLE;
            $this->fLastParsed = Parser\Utils\FloatingPoint::parseMatch($aMatches, $iSize);
            if (IFloatLimits::SINGLE === $iSize) {
                return chr(self::FLT_IMM_SINGLE) . pack('g', $this->fLastParsed);
            } else {
                return chr(self::FLT_IMM_DOUBLE) . pack('e', $this->fLastParsed);
            }
        }
        return null;
    }
}

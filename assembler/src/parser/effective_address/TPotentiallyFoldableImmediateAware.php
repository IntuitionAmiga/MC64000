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

/**
 * TPotentiallyFoldableImmediateAware
 *
 * Minmimal guts for Parser implementations that need to know an operation size, primarily immediate modes.
 */
trait TPotentiallyFoldableImmediateAware {

    /**
     * @param  IParser $oParser
     * @return IPotentiallyFoldableImmediateParser
     * @throws \InvalidArgumentException
     */
    private function castParserImmediateAware(IParser $oParser): IPotentiallyFoldableImmediateParser {
        if ($oParser instanceof IPotentiallyFoldableImmediateParser) {
            return $oParser;
        }
        throw new \InvalidArgumentException();
    }

    /**
     * @param  IParser $oParser
     * @return int|null
     * @throws \InvalidArgumentException
     */
    private function getIntegerImmediate(IParser $oParser): ?int {
        $oImmParser = $this->castParserImmediateAware($oParser);
        return $oImmParser->wasImmediate() ? (int)$oImmParser->getImmediate() : null;
    }

    /**
     * @param  IParser $oParser
     * @return float|null
     * @throws \InvalidArgumentException
     */
    private function getFloatImmediate(IParser $oParser): ?float {
        $oImmParser = $this->castParserImmediateAware($oParser);
        return $oImmParser->wasImmediate() ? (float)$oImmParser->getImmediate() : null;
    }
}

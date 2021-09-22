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

namespace ABadCafe\MC64K\Parser\SourceLine\Directive\Processor;
use ABadCafe\MC64K\Parser\SourceLine\Directive;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;

use function \preg_match, \rtrim, \chr, \str_repeat;

/**
 * Flag
 *
 * Allows the locally scoped override of assembler option flags.
 */
class Align implements Directive\IProcessor {

    const
        EXTRACT_MATCH = '/^\s+@(?:align){0,1}\s+(\d+)\s*,\s*(\d+)/',
        KEYWORDS      = [
            'align'
        ]
    ;

    /**
     * @inheritDoc
     */
    public function getMatchingKeywords(): array {
        return self::KEYWORDS;
    }

    /**
     * @inheritDoc
     */
    public function process(string $sSource): void {
        preg_match(self::EXTRACT_MATCH, rtrim($sSource), $aMatches);
        if (!empty($aMatches[2])) {
            $iAlignSize = (int)$aMatches[2];
            $oBuffer    = State\Coordinator::get()->getOutput();
            $iModulo    = $oBuffer->getCurrentOffset() % $iAlignSize;
            if ($iModulo > 0) {
                $sPadChar = chr((int)$aMatches[1]);
                $sPad     = str_repeat($sPadChar, $iAlignSize - $iModulo);
                $oBuffer->appendStatement($sPad);
            }
        }
    }
}


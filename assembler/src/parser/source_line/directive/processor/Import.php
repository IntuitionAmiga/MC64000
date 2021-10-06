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

use function \preg_match, \rtrim;

/**
 * Import
 *
 * Directive that indicates symbols that are to be imported, i.e. provided by the host.
 */
class Import implements Directive\IProcessor {

    private const
        EXTRACT_MATCH = '/^\s+@import\s+(' . Defs\ILabel::BASIC_MATCH .  ')' . Defs\ILabel::IE_SUFFIX_MATCH . '/',
        KEYWORDS      = [
            'import'
        ],
        IE_DEFAULT    = Defs\ILabel::IE_READ . Defs\ILabel::IE_WRITE
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
        if (!empty($aMatches[1])) {
            State\Coordinator::get()
                ->getLabelLocation()
                ->registerImport($aMatches[1], $aMatches[2] ?? self::IE_DEFAULT);
        }
    }
}


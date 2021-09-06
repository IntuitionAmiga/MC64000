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
 * Flag
 *
 * Allows the locally scoped override of assembler option flags.
 */
class Flag implements Directive\IProcessor {

    const
        EXTRACT_MATCH = '/^\s+@(en|dis)(?:able){0,1}\s+([a-zA-Z0-9_]+)/',
        KEYWORDS      = [
            'en',
            'dis',
            'enable',
            'disable'
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
        if (!empty($aMatches[1]) && !empty($aMatches[2])) {
            switch ($aMatches[1]) {
                case 'en':
                    State\Coordinator::get()->getOptions()->enable($aMatches[2]);
                    break;
                case 'dis':
                    State\Coordinator::get()->getOptions()->disable($aMatches[2]);
                default:
                    break;
            }
        }
    }
}


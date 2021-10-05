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
 * Define
 *
 * Allows locally scoped defining of literal substitutions:
 *
 *    @def     M_PI   #3.1415926535898
 *    @define  five   5
 */
class Define implements Directive\IProcessor {

    const
        EXTRACT_MATCH = '/^\s+@(def|define|equ)\s+([a-zA-Z_]{1}[a-zA-Z0-9_]*)\s+(.*)/',
        KEYWORDS      = [
            'def',
            'define',
            'equ'
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
        if (!empty($aMatches[2]) && !empty($aMatches[3])) {
            State\Coordinator::get()
                ->getDefinitionSet()
                ->add($aMatches[2], $aMatches[3]);
            if ('equ' === $aMatches[1]) {
                State\Coordinator::get()
                    ->getGlobalDefinitionSet()
                    ->add($aMatches[2], $aMatches[3]);
            }
        }
    }
}


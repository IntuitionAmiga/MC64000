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
use ABadCafe\MC64K\Utils\Log;

use function \preg_match, \rtrim;

/**
 * Stacksize
 *
 * Hints the required stack size for the entire binary. This is an advisory.
 *
 *    @stacksize 128
 */
class StackSize implements Directive\IProcessor {

    private const
        EXTRACT_MATCH = '/^\s+@stacksize\s+(\d+)\s*$/',
        KEYWORDS      = [
            'stacksize'
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
        if (preg_match(self::EXTRACT_MATCH, rtrim($sSource), $aMatches)) {
            $iStackSize = (int)$aMatches[1];
            $oOptions   = State\Coordinator::get()
                ->getGlobalOptions();
            $iOldStackSize = $oOptions->get(Defs\Project\IOptions::APP_STACK_SIZE, -1);
            if ($iStackSize > $iOldStackSize) {
                Log::printf(
                    "Increasing stack size from %d to %d",
                    $iOldStackSize,
                    $iStackSize
                );
                $oOptions->set(Defs\Project\IOptions::APP_STACK_SIZE, $iStackSize);
            }
        }
    }
}


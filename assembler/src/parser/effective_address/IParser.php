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
use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;

/**
 * IParser
 *
 * Extension if of the vanilla Parser, specifically for EffectiveAddress processing.
 */
interface IParser extends MC64K\IParser {

    const
        // Matches a displacement value, either a signed decimal or 0x/$ prefixed hexadecimal literal
        //D32 = '([\+\-]{0,1}\d+|(0x|\$)[0-9A-Fa-f]+)',
        D32 = Defs\IInteger::MATCH_OPTIMISTIC,

        // Matches an address register qualification (rN or aN)
        RA  = '([ra]\d+|sp)',

        // Matches any integer register qualification (rN, aN or dN | sp)
        RDA = '([rda]\d+|sp)'
    ;

    /**
     * Some EA modes may need awareness of the operation size being performed (in bytes).
     *
     * @param  int $iSize
     * @return self
     */
    public function setOperationSize(int $iSize): self;

    /**
     * Returns true if the EA mode has side effects.
     */
    public function hasSideEffects(): bool;
}

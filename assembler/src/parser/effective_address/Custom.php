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

/**
 * Custom
 *
 * Effective Address Parser adapter for other IParser implementations for instructions that have
 * specalised, inline data.
 */
class Custom implements IParser {

    protected MC64K\IParser $oParser;

    /**
     * @inheritDoc
     */
    public function hasSideEffects() : bool {
        if ($this->oParser instanceof IParser) {
            return $this->oParser->hasSideEffects();
        }
        return false;
    }

    /**
     * @inheritDoc
     */
    public function setOperationSize(int $iSize) : self {
        if ($this->oParser instanceof IParser) {
            $this->oParser->setOperationSize($iSize);
        }
        return $this;
    }

    /**
     * Constructor
     *
     * @param MC64K\IParser $oParser
     */
    public function __construct(MC64K\IParser $oParser) {
        $this->oParser    = $oParser;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        return $this->oParser->parse($sSource);
    }
}

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
 * AllFloatWriteable
 *
 * Meta Parser for float EA modes that can be written (i.e. suitable for destination).
 */
class AllFloatWriteable implements IParser {

    /**
     * @var IParser[] $aParsers - set of float writeable parsers
     */
    private array $aParsers = [];

    /**
     * Constructor
     */
    public function __construct() {

        // Initial best guess order of frequency.
        $this->aParsers = [
            new GPRDirect,
            new GPRIndirect,
            new GPRIndirectDisplacement,
            new GPRIndirectIndexed,
            new GPRIndirectIndexedDisplacement,
        ];
    }

    /**
     * @inheritDoc
     */
    public function setOperationSize(int $iSize) : self {
        foreach ($this->aParsers as $oParser) {
            $this->oParser->setOperationSize($iSize);
        }
        return $this;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        foreach ($this->aParsers as $oParser) {
            $sParsed = $oParser->parse($sSource);
            if (null !== $sParsed) {
                return $sParsed;
            }
        }
        return null;
    }
}

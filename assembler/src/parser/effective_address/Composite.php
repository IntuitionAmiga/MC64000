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
 * Composite
 *
 * Meta Parser
 */
abstract class Composite implements IParser {

    /**
     * @var IParser[] $aParsers - set of integer readable parsers
     */
    protected array $aParsers = [];

    /**
     * @var IParser|null $oParsedBy - the last IParser that succeeded in a call to parse()
     */
    protected ?IParser $oParsedBy = null;

    /**
     * @inheritDoc
     */
    public function setOperationSize(int $iSize): self {
        foreach ($this->aParsers as $oParser) {
            $oParser->setOperationSize($iSize);
        }
        return $this;
    }

    /**
     * @inheritDoc
     */
    public function hasSideEffects(): bool {
        return $this->oParsedBy ? $this->oParsedBy->hasSideEffects() : true;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {
        $this->oParsedBy = null;
        foreach ($this->aParsers as $oParser) {
            $sParsed = $oParser->parse($sSource);
            if (null !== $sParsed) {
                $this->oParsedBy = $oParser;
                return $sParsed;
            }
        }
        return null;
    }
}

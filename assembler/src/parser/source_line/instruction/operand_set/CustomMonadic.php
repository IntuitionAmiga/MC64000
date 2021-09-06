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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\OperandSet;
use ABadCafe\MC64K\Parser\EffectiveAddress;

/**
 * CustomMonadic
 *
 * For custom single operand operations.
 */
class CustomMonadic extends Monadic {

    /** @var int[] $aOpcodes */
    private array $aOpcodes = [];

    /** @var string[] $aPrefix */
    private array $aPrefix  = [];

    /**
     * Constructor
     *
     * @param int[]                    $aOpcodes
     * @param EffectiveAddress\IParser $oSrcParser
     * @param string[]                 $aPrefix
     */
    public function __construct(array $aOpcodes, EffectiveAddress\IParser $oSrcParser, array $aPrefix = []) {
        $this->aOpcodes   = $aOpcodes;
        $this->aPrefix    = $aPrefix;
        $this->oSrcParser = $oSrcParser;
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return $this->aOpcodes;
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $sBytecode = parent::parse($iOpcode, $aOperands, $aSizes);
        if (isset($this->aPrefix[$iOpcode])) {
            return $this->aPrefix[$iOpcode] . $sBytecode;
        }
        return $sBytecode;
    }
}

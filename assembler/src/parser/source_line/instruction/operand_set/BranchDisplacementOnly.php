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
use ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K\Parser\SourceLine\Instruction;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;

/**
 * BranchDisplacementOnly
 *
 * For those operations that have a displacement only
 */
class BranchDisplacementOnly implements Instruction\IOperandSetParser {

    use TBranching;

    const OPCODES = [
        IControl::BRA_B,
        IControl::BRA,
        IControl::BSR_B,
        IControl::BSR,
    ];

    const SHORT = [
        IControl::BRA_B => 1,
        IControl::BSR_B => 1,
    ];

    public function __construct() {
        $this->oTgtParser = new Operand\BranchDisplacement();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return self::OPCODES;
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $iCount = count($aOperands);
        if (1 !== $iCount) {
            throw new \LengthException(__CLASS__ . ' expects a single operand, got ' . $iCount);
        }

        $bShort = isset(self::SHORT[$iOpcode]);
        $sLabel = $aOperands[0];
        $oState = State\Common::get();

        $oState->setCurrentStatementLength(
            Defs\IOpcodeLimits::SIZE +
            ($bShort ?
                Defs\IBranchLimits::SHORT_DISPLACEMENT_SIZE :
                Defs\IBranchLimits::DISPLACEMENT_SIZE)
        );

        $sBytecode = $this->oTgtParser->parse($sLabel);

        if ($this->oTgtParser->wasUnresolved()) {
            $oState->addUnresolvedLabel($sLabel);
        }

        if ($bShort) {
            $this->checkShortBranchDisplacement();
            $sBytecode = $sBytecode[0];
        } else {
            $this->checkBranchDisplacement($sBytecode);
        }

        return $sBytecode;
    }
}

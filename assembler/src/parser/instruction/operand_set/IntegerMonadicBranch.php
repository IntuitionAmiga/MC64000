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

namespace ABadCafe\MC64K\Parser\Instruction\OperandSet;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Parser\Instruction\CodeFoldException;

/**
 * IntegerMonadicBranch
 *
 * For all vanilla integer compare and branch
 */
class IntegerMonadicBranch extends Monadic {

    use TBranching;

    const
        OPERAND_TARGET    = 1,
        MIN_OPERAND_COUNT = 2
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::BIZ_B,
        IControl::BIZ_W,
        IControl::BIZ_L,
        IControl::BIZ_Q,
        IControl::BNZ_B,
        IControl::BNZ_W,
        IControl::BNZ_L,
        IControl::BNZ_Q,
        IControl::BMI_B,
        IControl::BMI_W,
        IControl::BMI_L,
        IControl::BMI_Q,
        IControl::BPL_B,
        IControl::BPL_W,
        IControl::BPL_L,
        IControl::BPL_Q,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new Parser\EffectiveAddress\AllIntegerReadable();
        $this->oTgtParser = new Parser\Instruction\Operand\BranchDisplacement();
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
    public function parse(array $aOperands, array $aSizes = []) : string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

        $sDisplacement = $this->oTgtParser->parse($aOperands[self::OPERAND_TARGET]);
        $iSrcIndex     = $this->getSourceOperandIndex();
        $sSrcBytecode  = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid comparison operand'
            );
        }

        $sBytecode = $sSrcBytecode . $sDisplacement;
        $this->checkBranchDisplacement($sBytecode);

        if ($this->oSrcParser->wasImmediate()) {
            throw new CodeFoldException('Compile time constant comparison : TODO - fold out');
        }
        return $sBytecode;
    }

}

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
 * FloatDyadicBranch
 *
 * For all vanilla float compare and branch
 */
class FloatDyadicBranch extends Dyadic {

    use TBranching;

    const
        OPERAND_TARGET    = 2,
        MIN_OPERAND_COUNT = 3
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::FBLT_S,
        IControl::FBLT_D,
        IControl::FBLE_S,
        IControl::FBLE_D,
        IControl::FBEQ_S,
        IControl::FBEQ_D,
        IControl::FBGE_S,
        IControl::FBGE_D,
        IControl::FBGT_S,
        IControl::FBGT_D,
        IControl::FBNE_S,
        IControl::FBNE_D,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new Parser\EffectiveAddress\AllFloatReadable();
        $this->oDstParser = new Parser\EffectiveAddress\AllFloatReadable();
        $this->oTgtParser = new Parser\Instruction\Operand\BranchDisplacement();
        parent::__construct();
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

        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

        $sDisplacement = $this->oTgtParser->parse($aOperands[self::OPERAND_TARGET]);
        $iDstIndex     = $this->getDestinationOperandIndex();
        $sDstBytecode  = $this->oDstParser
            ->setOperationSize($aSizes[$iDstIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iDstIndex]);
        if (null === $sDstBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iDstIndex] . ' not a valid comparison operand'
            );
        }

        $iSrcIndex    = $this->getSourceOperandIndex();
        $sSrcBytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid comparison operand'
            );
        }

        $sBytecode = $sDstBytecode . $sSrcBytecode . $sDisplacement;
        $this->checkBranchDisplacement($sBytecode);

        if ($this->oDstParser->wasImmediate() && $this->oSrcParser->wasImmediate()) {
            throw new CodeFoldException(
                'Compile time constant comparison - TODO fold out src:' . $this->oSrcParser->getImmediate() .
                ' dst:' . $this->oDstParser->getImmediate()
            );
        }

        if ($this->canOptimiseSourceOperand($sSrcBytecode, $sDstBytecode)) {
            throw new CodeFoldException('Runtime invariant comparison - TODO - fold out');
        }
        return $sBytecode;
    }


}

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
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\UnhandledCodeFoldException;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;

/**
 * IntegerDyadicBranch
 *
 * For all vanilla integer compare and branch
 */
class IntegerDyadicBranch extends Dyadic {

    use TBranching;

    const
        OPERAND_TARGET    = 2,
        MIN_OPERAND_COUNT = 3
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::BLT_B => 'foldImmediateIsLessThan',
        IControl::BLT_W => 'foldImmediateIsLessThan',
        IControl::BLT_L => 'foldImmediateIsLessThan',
        IControl::BLT_Q => 'foldImmediateIsLessThan',
        IControl::BLE_B => 'foldImmediateIsLessThanOrEqual',
        IControl::BLE_W => 'foldImmediateIsLessThanOrEqual',
        IControl::BLE_L => 'foldImmediateIsLessThanOrEqual',
        IControl::BLE_Q => 'foldImmediateIsLessThanOrEqual',
        IControl::BEQ_B => 'foldImmediateIsEqual',
        IControl::BEQ_W => 'foldImmediateIsEqual',
        IControl::BEQ_L => 'foldImmediateIsEqual',
        IControl::BEQ_Q => 'foldImmediateIsEqual',
        IControl::BGE_B => 'foldImmediateIsGreaterOrEqual',
        IControl::BGE_W => 'foldImmediateIsGreaterOrEqual',
        IControl::BGE_L => 'foldImmediateIsGreaterOrEqual',
        IControl::BGE_Q => 'foldImmediateIsGreaterOrEqual',
        IControl::BGT_B => 'foldImmediateIsGreaterThan',
        IControl::BGT_W => 'foldImmediateIsGreaterThan',
        IControl::BGT_L => 'foldImmediateIsGreaterThan',
        IControl::BGT_Q => 'foldImmediateIsGreaterThan',
        IControl::BNE_B => 'foldImmediateIsNotEqual',
        IControl::BNE_W => 'foldImmediateIsNotEqual',
        IControl::BNE_L => 'foldImmediateIsNotEqual',
        IControl::BNE_Q => 'foldImmediateIsNotEqual',
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser = new EffectiveAddress\AllIntegerReadable();
        $this->oTgtParser = new Operand\BranchDisplacement();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return array_keys(self::OPCODES);
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

        // Check for foldable immediates
        if ($this->oDstParser->wasImmediate() && $this->oSrcParser->wasImmediate()) {
            $cCallback = [$this, self::OPCODES[$iOpcode]];
            throw new CodeFoldException(
                'Compile time constant comparison',
                $cCallback(
                    $this->oSrcParser->getImmediate(),
                    $this->oDstParser->getImmediate(),
                    $this->oTgtParser->getLastDisplacement(),
                    strlen($sBytecode)
                )
            );
        }

        // Check for foldable EA. These are where the source EA is exactly the same as the destination
        if ($this->canOptimiseSourceOperand($sSrcBytecode, $sDstBytecode)) {
            $cCallback = [$this, self::OPCODES[$iOpcode]];
            throw new CodeFoldException(
                'Runtime invariant comparison',
                $cCallback(
                    1, // doesn't matter
                    1, // doesn't matter
                    $this->oTgtParser->getLastDisplacement(),
                    strlen($sBytecode)
                )
            );
        }

        return $sBytecode;
    }
}

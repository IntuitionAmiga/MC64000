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
        MIN_OPERAND_COUNT = 2,
        FIXED_LENGTH      = 4
    ;

    /**
     * Map of opcode keys to test functions for resolving immediate branches taken or not.
     */
    const OPCODES = [
        IControl::BIZ_B => 'foldIsZero',
        IControl::BIZ_W => 'foldIsZero',
        IControl::BIZ_L => 'foldIsZero',
        IControl::BIZ_Q => 'foldIsZero',
        IControl::BNZ_B => 'foldIsNotZero',
        IControl::BNZ_W => 'foldIsNotZero',
        IControl::BNZ_L => 'foldIsNotZero',
        IControl::BNZ_Q => 'foldIsNotZero',
        IControl::BMI_B => 'foldIsMinus',
        IControl::BMI_W => 'foldIsMinus',
        IControl::BMI_L => 'foldIsMinus',
        IControl::BMI_Q => 'foldIsMinus',
        IControl::BPL_B => 'foldIsPlus',
        IControl::BPL_W => 'foldIsPlus',
        IControl::BPL_L => 'foldIsPlus',
        IControl::BPL_Q => 'foldIsPlus',
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
        return array_keys(self::OPCODES);
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
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
            $cCallback = [$this, self::OPCODES[$iOpcode]];
            throw new CodeFoldException(
                'Compile time constant comparison ',
                $cCallback(
                    $this->oSrcParser->getImmediate(),
                    $this->oTgtParser->getLastDisplacement(),
                    strlen($sBytecode)
                )
            );
        }
        return $sBytecode;
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsZero(int $iImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsNotZero(int $iImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsNotEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsMinus(int $iImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsLessThan($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsPlus(int $iImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsGreaterOrEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

}

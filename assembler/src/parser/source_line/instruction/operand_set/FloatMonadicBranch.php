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
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;

/**
 * FloatMonadicBranch
 *
 * For all vanilla float compare and branch
 */
class FloatMonadicBranch extends Monadic {

    use TBranching;

    const
        OPERAND_TARGET    = 1,
        MIN_OPERAND_COUNT = 2,
        FIXED_LENGTH      = 4
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::FBIZ_S => 'foldIsZero',
        IControl::FBIZ_D => 'foldIsZero',
        IControl::FBNZ_S => 'foldIsNotZero',
        IControl::FBNZ_D => 'foldIsNotZero',
        IControl::FBMI_S => 'foldIsMinus',
        IControl::FBMI_D => 'foldIsMinus',
        IControl::FBPL_S => 'foldIsPlus',
        IControl::FBPL_D => 'foldIsPlus',
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllFloatReadable();
        $this->oTgtParser = new Operand\BranchDisplacement();
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
    private function foldIsZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsNotZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsNotEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsMinus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsLessThan($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    private function foldIsPlus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsGreaterOrEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }
}

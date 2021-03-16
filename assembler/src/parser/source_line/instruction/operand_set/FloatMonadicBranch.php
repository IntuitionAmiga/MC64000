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
 * For single operand vanilla float compare and branch operations.
 */
class FloatMonadicBranch extends MonadicBranch {

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
     * Trampoline to TBranching code folders
     */
    protected function foldIsZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsNotZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsNotEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsMinus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsLessThan($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsPlus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $this->foldImmediateIsGreaterOrEqual($fImmediate, 0.0, $iDisplacement, $iOriginalSize);
    }
}

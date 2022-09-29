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
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Defs\Mnemonic\ICondition;


use function \array_keys;

/**
 * IntegerMonadicBranch
 *
 * For all vanilla single integer integer test and branch operations.
 */
class IntegerMonadicBranch extends MonadicBranch {

    /**
     * Map of opcode keys to test functions for resolving immediate branches taken or not.
     */
    const OPCODES = [
        IControl::BMC << 8 | ICondition::IEQ_B => 'foldIsZero',
        IControl::BMC << 8 | ICondition::IEQ_W => 'foldIsZero',
        IControl::BMC << 8 | ICondition::IEQ_L => 'foldIsZero',
        IControl::BMC << 8 | ICondition::IEQ_Q => 'foldIsZero',
        IControl::BMC << 8 | ICondition::INE_B => 'foldIsNotZero',
        IControl::BMC << 8 | ICondition::INE_W => 'foldIsNotZero',
        IControl::BMC << 8 | ICondition::INE_L => 'foldIsNotZero',
        IControl::BMC << 8 | ICondition::INE_Q => 'foldIsNotZero',
        IControl::BMC << 8 | ICondition::ILT_B => 'foldIsMinus',
        IControl::BMC << 8 | ICondition::ILT_W => 'foldIsMinus',
        IControl::BMC << 8 | ICondition::ILT_L => 'foldIsMinus',
        IControl::BMC << 8 | ICondition::ILT_Q => 'foldIsMinus',
        IControl::BMC << 8 | ICondition::IGT_B => 'foldIsPlus',
        IControl::BMC << 8 | ICondition::IGT_W => 'foldIsPlus',
        IControl::BMC << 8 | ICondition::IGT_L => 'foldIsPlus',
        IControl::BMC << 8 | ICondition::IGT_Q => 'foldIsPlus',
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oTgtParser = new Operand\BranchDisplacement();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return array_keys(self::OPCODES);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsZero(int $iImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $this->foldImmediateIsEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsNotZero(int $iImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $this->foldImmediateIsNotEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsMinus(int $iImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $this->foldImmediateIsLessThan($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * Trampoline to TBranching code folders
     */
    protected function foldIsPlus(int $iImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $this->foldImmediateIsGreaterOrEqual($iImmediate, 0, $iDisplacement, $iOriginalSize);
    }

    /**
     * @inheritDoc
     */
    protected function getFoldFunctions(): array {
        return self::OPCODES;
    }

}

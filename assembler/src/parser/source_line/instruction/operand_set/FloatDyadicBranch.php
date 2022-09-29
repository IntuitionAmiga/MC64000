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
use ABadCafe\MC64K\Defs\Mnemonic\ICondition;
use ABadCafe\MC64K\Defs;

use function \array_keys;

/**
 * FloatDyadicBranch
 *
 * For all vanilla float compare and branch operations.
 */
class FloatDyadicBranch extends DyadicBranch {

    const CB_DEFAULT = 1.0;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::BDC << 8 | ICondition::FLT_S => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::FLT_D => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::FLE_S => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::FLE_D => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::FEQ_S => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::FEQ_D => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::FGE_S => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::FGE_D => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::FGT_S => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::FGT_D => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::FNE_S => 'foldImmediateIsNotEqual',
        IControl::BDC << 8 | ICondition::FNE_D => 'foldImmediateIsNotEqual',
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllFloatReadable();
        $this->oDstParser = new EffectiveAddress\AllFloatReadable();
        $this->oTgtParser = new Operand\BranchDisplacement();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return array_keys(self::OPCODES);
    }

    /**
     * @inheritDoc
     */
    protected function getFoldFunctions(): array {
        return self::OPCODES;
    }

    protected function getCallbackDefault(): float {
        return self::CB_DEFAULT;
    }
}

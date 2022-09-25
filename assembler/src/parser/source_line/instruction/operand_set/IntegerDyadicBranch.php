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

use function \array_keys;

/**
 * IntegerDyadicBranch
 *
 * For all vanilla integer compare and branch operations.
 */
class IntegerDyadicBranch extends DyadicBranch {

    const CB_DEFAULT = 1;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::BDC << 8 | ICondition::ILT_B => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::ILT_W => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::ILT_L => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::ILT_Q => 'foldImmediateIsLessThan',
        IControl::BDC << 8 | ICondition::ILE_B => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::ILE_W => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::ILE_L => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::ILE_Q => 'foldImmediateIsLessOrEqual',
        IControl::BDC << 8 | ICondition::IEQ_B => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::IEQ_W => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::IEQ_L => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::IEQ_Q => 'foldImmediateIsEqual',
        IControl::BDC << 8 | ICondition::IGE_B => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::IGE_W => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::IGE_L => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::IGE_Q => 'foldImmediateIsGreaterOrEqual',
        IControl::BDC << 8 | ICondition::IGT_B => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::IGT_W => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::IGT_L => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::IGT_Q => 'foldImmediateIsGreaterThan',
        IControl::BDC << 8 | ICondition::INE_B => 'foldImmediateIsNotEqual',
        IControl::BDC << 8 | ICondition::INE_W => 'foldImmediateIsNotEqual',
        IControl::BDC << 8 | ICondition::INE_L => 'foldImmediateIsNotEqual',
        IControl::BDC << 8 | ICondition::INE_Q => 'foldImmediateIsNotEqual',
        IControl::BDC << 8 | ICondition::BPS_B => 'foldImmediateBitSetByte',
        IControl::BDC << 8 | ICondition::BPS_W => 'foldImmediateBitSetWord',
        IControl::BDC << 8 | ICondition::BPS_L => 'foldImmediateBitSetLong',
        IControl::BDC << 8 | ICondition::BPS_Q => 'foldImmediateBitSetQuad',
        IControl::BDC << 8 | ICondition::BPC_B => 'foldImmediateBitClearByte',
        IControl::BDC << 8 | ICondition::BPC_W => 'foldImmediateBitClearWord',
        IControl::BDC << 8 | ICondition::BPC_L => 'foldImmediateBitClearLong',
        IControl::BDC << 8 | ICondition::BPC_Q => 'foldImmediateBitClearQuad',
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
    public function getOpcodes(): array {
        return array_keys(self::OPCODES);
    }

    /**
     * For bbs.b with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitSetByte($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return (1<<($mSrcImmediate & 7)) & $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
    }

    /**
     * For bbs.w with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitSetWord($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return (1<<($mSrcImmediate & 15)) & $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For bbs.l with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitSetLong($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return (1<<($mSrcImmediate & 31)) & $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For bbs.q with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitSetQuad($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return (1<<($mSrcImmediate & 63)) & $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For bbc.b with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitClearByte($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return 0 === ((1<<($mSrcImmediate & 7)) & $mDstImmediate) ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For bbc.w with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitClearWord($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return 0 === ((1<<($mSrcImmediate & 15)) & $mDstImmediate) ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For bbc.l with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitClearLong($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return 0 === ((1<<($mSrcImmediate & 31)) & $mDstImmediate) ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
    }

    /**
     * For bbc.q with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateBitClearQuad($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return 0 === ((1<<($mSrcImmediate & 63)) & $mDstImmediate) ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
    }

    /**
     * @inheritDoc
     */
    protected function getFoldFunctions(): array {
        return self::OPCODES;
    }

    protected function getCallbackDefault(): int {
        return self::CB_DEFAULT;
    }
}

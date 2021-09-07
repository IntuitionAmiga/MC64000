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
        IControl::BLT_B => 'foldImmediateIsLessThan',
        IControl::BLT_W => 'foldImmediateIsLessThan',
        IControl::BLT_L => 'foldImmediateIsLessThan',
        IControl::BLT_Q => 'foldImmediateIsLessThan',
        IControl::BLE_B => 'foldImmediateIsLessOrEqual',
        IControl::BLE_W => 'foldImmediateIsLessOrEqual',
        IControl::BLE_L => 'foldImmediateIsLessOrEqual',
        IControl::BLE_Q => 'foldImmediateIsLessOrEqual',
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
        IControl::BBS_B => 'foldImmediateBitSetByte',
        IControl::BBS_W => 'foldImmediateBitSetWord',
        IControl::BBS_L => 'foldImmediateBitSetLong',
        IControl::BBS_Q => 'foldImmediateBitSetQuad',
        IControl::BBC_B => 'foldImmediateBitClearByte',
        IControl::BBC_W => 'foldImmediateBitClearWord',
        IControl::BBC_L => 'foldImmediateBitClearLong',
        IControl::BBC_Q => 'foldImmediateBitClearQuad',
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
}

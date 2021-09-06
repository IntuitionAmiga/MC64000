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
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Defs\IBranchLimits;
use ABadCafe\MC64K\Parser;

use function \chr, \strlen, \pack;

/**
 * TBranching
 *
 * Trait that provides a set of common behavioural requirements for set parsers that branch.
 */
trait TBranching {

    protected Operand\BranchDisplacement $oTgtParser;

    protected bool $bAllowBranchToSelf = false;

    /**
     * @param  string $sSource
     * @param  bool   $bOperandSideEffects
     * @return string
     * @throws CodeFoldException|UnhandledCodeFoldException
     */
    protected function parseBranchDisplacement(string $sSource, bool $bOperandSideEffects): string {
        try {
            return $this->oTgtParser->parse($sSource);
        } catch (CodeFoldException $oFold) {
            if ($bOperandSideEffects) {
                throw new UnhandledCodeFoldException("Cannot fold zero branch as one or both operands have side effects");
            }
            throw $oFold;
        }
    }

    /**
     * @throws \RangeException|\UnexpectedValueException
     */
    protected function checkShortBranchDisplacement(): void {
        $iDisplacement = $this->oTgtParser->getLastDisplacement();
        if (0 === $iDisplacement) {
            throw new \UnexpectedValueException('Short branch cannot be unresolved');
        }
        if (
            $iDisplacement < IFixedBranch::RANGES[IControl::BRA_B][IFixedBranch::MAX_REVERSE] ||
            $iDisplacement > IFixedBranch::RANGES[IControl::BRA_B][IFixedBranch::MAX_FORWARD]
        ) {
            throw new \RangeException('Short branch specified but target ' . $iDisplacement . ' is out of range');
        }
        $this->checkBranchDisplacement(chr(0), false);
    }

    /**
     * Checks the last parsed branch displacement against the current bytecode to make sure that a backwards branch
     * target doesn't land on or within the current instruction, which is not recoverable. Exceptions to this are
     * conditional branches to self where the operand(s) being tested have side effects that may result in a change
     * of outcome from one iteration to the next.
     *
     * @param string $sInstructionBytecode
     * @throws \UnexpectedValueException
     */
    protected function checkBranchDisplacement(string $sInstructionBytecode, bool $bOperandSideEffects): void {
        $iDisplacement = $this->oTgtParser->getLastDisplacement();
        if ($iDisplacement < 0) {
            $iInstructionLength = strlen($sInstructionBytecode) + 1;
            $iDisplacement      = -$iDisplacement;
            if (
                $iDisplacement < $iInstructionLength ||
                ($iDisplacement == $iInstructionLength && false === ($this->bAllowBranchToSelf || $bOperandSideEffects))
            ) {
                throw new \UnexpectedValueException(
                    'Invalid branch target -' . $iDisplacement .
                    '; must be further than -' . $iInstructionLength .
                    ' to avoid infinite loop/corrupted PC'
                );
            }
        }
    }

    /**
     * For BLT instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsLessThan($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate < $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For BLE instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsLessOrEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate <= $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For BEQ instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate == $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For BGE instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsGreaterOrEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate >= $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For BGT instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsGreaterThan($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate > $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * For BEQ instructions with a pair of immediate operands, evaluates the outcome and generates
     * the appropriate optimised bytecode (empty if branch is not taken, BRA if it is).
     *
     * @param  int|float $mSrcImmediate
     * @param  int|float $mDstImmediate
     * @param  int       $iDisplacement
     * @param  int       $iOriginalSize
     * @return string
     */
    protected function foldImmediateIsNotEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize): string {
        return $mSrcImmediate != $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize): '';
    }

    /**
     * Generates the appropriate bytecode for a fixed branch based on the original displacement and unoptimised
     * instruction size. For positive branches, the displacement is unchanged. For negative branches we must adjust by
     * the difference in size from the original instruction to the fixed branch.
     *
     * @param  int    $iDisplacement
     * @param  int    $iOriginalSize
     * @return string
     * @throws \UnexpectedValueException
     */
    protected function encodeFixedBranch(int $iDisplacement, int $iOriginalSize): string {
        // Negative branches require target adjustment because the fixed encoded branch instruction is almost guaranteed
        // to be shorter than the conditional instruction it will replace.
        if ($iDisplacement < 0) {
            foreach (IFixedBranch::RANGES as $iOpcode => $aRange) {
                $iProposedDisplacement = $iDisplacement + $iOriginalSize - $aRange[IFixedBranch::ENCODE_SIZE];
                if ($iProposedDisplacement >= $aRange[IFixedBranch::MAX_REVERSE]) {
                    return $this->generateFixedBranchBytecode(
                        $iOpcode,
                        $iProposedDisplacement,
                        $aRange[IFixedBranch::DATA_FORMAT]
                    );
                }
            }
            throw new \UnexpectedValueException('Unable to encode fixed branch for #' . $iProposedDisplacement);
        }

        if (IBranchLimits::UNRESOLVED_DISPLACEMENT === $iDisplacement) {
            $iOpcode = IControl::BRA;
            $aRange  = IFixedBranch::RANGES[$iOpcode];
            return $this->generateFixedBranchBytecode(
                IControl::BRA,
                $iDisplacement,
                IFixedBranch::RANGES[IControl::BRA][IFixedBranch::DATA_FORMAT]
            );
        }
        foreach (IFixedBranch::RANGES as $iOpcode => $aRange) {
            if ($iDisplacement <= $aRange[IFixedBranch::MAX_FORWARD]) {
                return $this->generateFixedBranchBytecode(
                    $iOpcode,
                    $iDisplacement,
                    $aRange[IFixedBranch::DATA_FORMAT]
                );
            }
        }
        throw new \UnexpectedValueException('Unable to encode fixed branch for #' . $iDisplacement);
    }

    /**
     * @param  int    $iOpcode
     * @param  int    $iDisplacement
     * @param  string $sFormat
     * @return string
     */
    private function generateFixedBranchBytecode(int $iOpcode, int $iDisplacement, string $sFormat): string {
        return chr($iOpcode) . pack($sFormat, $iDisplacement);
    }
}

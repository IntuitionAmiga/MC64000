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
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Parser\Instruction\Operand\BranchDisplacement;

/**
 * TBranching
 */
trait TBranching {

    private Parser\Instruction\Operand\BranchDisplacement $oTgtParser;

    /**
     * Checks the last parsed branch displacement agains the current bytecode to make sure that
     * a backwards branch target doesn't land on or within the current instruction, which is not
     * recoverable.
     *
     * @param string $sInstructionBytecode
     * @throws \UnexpectedValueException
     */
    private function checkBranchDisplacement(string $sInstructionBytecode) : void {
        $iDisplacement = $this->oTgtParser->getLastDisplacement();
        if ($iDisplacement < 0) {
            $iInstructionLength = strlen($sInstructionBytecode) + 1;
            $iDisplacement      = -$iDisplacement;
            if ($iDisplacement <= $iInstructionLength) {
                throw new \UnexpectedValueException(
                    'Invalid target -' . $iDisplacement .
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
    private function foldImmediateIsLessThan($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate < $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function foldImmediateIsLessThanOrEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate <= $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function foldImmediateIsEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate == $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function foldImmediateIsGreaterOrEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate >= $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function foldImmediateIsGreaterThan($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate > $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function foldImmediateIsNotEqual($mSrcImmediate, $mDstImmediate, int $iDisplacement, int $iOriginalSize) : string {
        return $mSrcImmediate != $mDstImmediate ? $this->encodeFixedBranch($iDisplacement, $iOriginalSize) : '';
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
    private function encodeFixedBranch(int $iDisplacement, int $iOriginalSize) : string {
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

        if (BranchDisplacement::UNRESOLVED === $iDisplacement) {
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
    private function generateFixedBranchBytecode(int $iOpcode, int $iDisplacement, string $sFormat) : string {
        return chr($iOpcode) . pack($sFormat, $iDisplacement);
    }
}

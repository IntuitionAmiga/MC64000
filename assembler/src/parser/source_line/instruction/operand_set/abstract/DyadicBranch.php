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
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;

/**
 * DyadicBranch
 *
 * Common behaviours for double operand with branch displacement.
 */
abstract class DyadicBranch extends Dyadic {

    use TBranching;

    const
        OPERAND_TARGET    = 2,
        MIN_OPERAND_COUNT = 3
    ;

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {

        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

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

        $oState = State\Coordinator::get();
        $oState->setCurrentStatementLength(
            Defs\IOpcodeLimits::SIZE +
            Defs\IBranchLimits::DISPLACEMENT_SIZE +
            strlen($sDstBytecode . $sSrcBytecode)
        );

        $sBranchTarget = $aOperands[self::OPERAND_TARGET];
        $sDisplacement = $this->parseBranchDisplacement(
            $sBranchTarget,
            $this->oDstParser->hasSideEffects() || $this->oSrcParser->hasSideEffects()
        );

        $sBytecode = $sDstBytecode . $sSrcBytecode . $sDisplacement;
        $this->checkBranchDisplacement($sBytecode);

        // Check for foldable immediate values
        if ($this->oDstParser->wasImmediate() && $this->oSrcParser->wasImmediate()) {
            $sFoldFunc = static::OPCODES[$iOpcode];
            $cCallback = [$this, $sFoldFunc];
            $sFolded   =  $cCallback(
                $this->oSrcParser->getImmediate(),
                $this->oDstParser->getImmediate(),
                $this->oTgtParser->getLastDisplacement(),
                strlen($sBytecode)
            );

            // If the folded code is not empty, make sure we handle any unresolved branch targets
            if ($sFolded && $this->oTgtParser->wasUnresolved()) {
                $oState
                    ->setCurrentStatementLength(strlen($sFolded))
                    ->addUnresolvedLabel($sBranchTarget);
            }

            throw new CodeFoldException(
                'SrcEA #' . $this->oSrcParser->getImmediate() . ', ' .
                'DstEA #' . $this->oDstParser->getImmediate() .
                ' using ' . $sFoldFunc,
                $sFolded
            );
        }

        // Check for foldable EA. These are where the source EA is exactly the same as the destination.
        if ($this->canOptimiseSourceOperand($sSrcBytecode, $sDstBytecode)) {
            $sFoldFunc = static::OPCODES[$iOpcode];
            $cCallback = [$this, $sFoldFunc];

            $sFolded   = $cCallback(
                static::CB_DEFAULT, // doesn't matter
                static::CB_DEFAULT, // doesn't matter
                $this->oTgtParser->getLastDisplacement(),
                strlen($sBytecode)
            );

            // If the folded code is not empty, make sure we handle any unresolved branch targets.
            if ($sFolded && $this->oTgtParser->wasUnresolved()) {
                $oState
                    ->setCurrentStatementLength(strlen($sFolded))
                    ->addUnresolvedLabel($sBranchTarget);
            }

            throw new CodeFoldException(
                'SrcEA '  . $aOperands[$iSrcIndex] . ', ' .
                'DstEA '  . $aOperands[$iDstIndex] .
                ' using ' . $sFoldFunc,
                $sFolded
            );
        }

        // Make sure any unresolved label references are recorded.
        if ($this->oTgtParser->wasUnresolved()) {
            $oState->addUnresolvedLabel($sBranchTarget);
        }

        return $sBytecode;
    }


}
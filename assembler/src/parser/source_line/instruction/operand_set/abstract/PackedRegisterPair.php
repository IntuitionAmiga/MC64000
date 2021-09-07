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
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;

use function \ord, \chr;

/**
 * PackedRegisterPair
 *
 * For register to register operations where both registers are packed into a single byte.
 */
abstract class PackedRegisterPair extends Dyadic {
    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);
        $oState = State\Coordinator::get()
            ->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE);
        $iDstIndex    = $this->getDestinationOperandIndex();
        $sDstBytecode = $this->oDstParser
            ->setOperationSize($aSizes[$iDstIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iDstIndex]);
        if (null === $sDstBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iDstIndex] . ' not a valid destination operand'
            );
        }

        $iSrcIndex    = $this->getSourceOperandIndex();
        $sSrcBytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid source operand'
            );
        }

        $iDstReg = ord($sDstBytecode) & 0x0F;
        $iSrcReg = ord($sSrcBytecode) & 0x0F;
        if ($iDstReg === $iSrcReg && $this->foldIfOperandsSame($iOpcode)) {
            throw new CodeFoldException(
                'Src Reg ' . $aOperands[$iSrcIndex] . ', ' .
                'Dst Reg ' . $aOperands[$iDstIndex] . ' using foldIfOperandsSame'
            );
        }
        $iByte   = $iSrcReg << 4 | $iDstReg;
        return chr($iByte);
    }

    /**
     * Depending on the operation, where source and destination are the same, it may be better to fold out the code.
     *
     * @param  int  $iOpcode
     * @return bool
     */
    protected abstract function foldIfOperandsSame(int $iOpcode): bool;
}

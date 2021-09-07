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
use ABadCafe\MC64K\Parser\SourceLine\Instruction;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;

use function \strlen, \count, \get_class;

/**
 * Monadic
 *
 * Base for all vanilla monadic operations.
 */
abstract class Monadic implements Instruction\IOperandSetParser {

    const
        DEF_OPERAND_SRC   = 0,
        MIN_OPERAND_COUNT = 1
    ;

    protected EffectiveAddress\IParser $oSrcParser;

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);
        $oState = State\Coordinator::get()
            ->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE);
        $iSrcIndex    = $this->getSourceOperandIndex();
        $sSrcBytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid operand');
        }
        $oState->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE + strlen($sSrcBytecode));
        return $sSrcBytecode;
    }

    /**
     * Returns the expected index in the operands array for the source operand
     *
     * @return int
     */
    protected function getSourceOperandIndex(): int {
        return self::DEF_OPERAND_SRC;
    }

    /**
     * Sanity checks the input array length
     *
     * @param  string[] $aOperands
     * @param  int      $iMinimumCount
     * @throws \LengthException
     */
    protected function assertMinimumOperandCount(array $aOperands, int $iMinimumCount): void {
        $iCount = count($aOperands);
        if ($iMinimumCount > $iCount) {
            throw new \LengthException(
                get_class($this) . ' expects at least ' .
                $iMinimumCount . ' operands, got ' . $iCount
            );
        }
    }
}

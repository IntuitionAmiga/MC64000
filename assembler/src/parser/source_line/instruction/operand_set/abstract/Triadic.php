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
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;

use function \array_combine, \strlen, \ord, \chr;

/**
 * Triadic
 *
 * Base for all vanilla destination @ source1, source2 -> destination operations.
 */
abstract class Triadic extends Dyadic {

    const
        DEF_OPERAND_SRC   = 1,
        DEF_OPERAND_SRC_2 = 0,
        DEF_OPERAND_DST   = 2,
        MIN_OPERAND_COUNT = 3
    ;

    protected EffectiveAddress\IParser $oSrc2Parser;

    protected ?string $sSrc2Bytecode;

    /**
     * Base constructor
     */
    public function __construct() {
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {

        $iInstructionSize = $this->getInitialInstructionSize();

        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);
        $oState = State\Coordinator::get();
        $oState->setCurrentStatementLength($iInstructionSize);

        $this->sSrcBytecode  = null;
        $this->sSrc2Bytecode = null;

        // Get the actual destination first
        $iDstIndex           = $this->getDestinationOperandIndex();
        $this->sDstBytecode  = $this->oDstParser
            ->setOperationSize($aSizes[$iDstIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iDstIndex]);
        if (null === $this->sDstBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iDstIndex] . ' not a valid destination operand'
            );
        }

        $iInstructionSize += strlen($this->sDstBytecode);
        $oState->setCurrentStatementLength($iInstructionSize);

        $iSrc1Index = $this->getSource1OperandIndex();
        $this->sSrcBytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrc1Index] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrc1Index]);
        if (null === $this->sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrc1Index] . ' not a valid source 1 operand'
            );
        }

        $oState->setCurrentStatementLength(
            Defs\IOpcodeLimits::SIZE_SUB +
            strlen($this->sDstBytecode) +
            strlen($this->sSrcBytecode)
        );

        $iSrc2Index    = $this->getSource2OperandIndex();
        $sSrc2Bytecode = $this->oSrc2Parser
            ->setOperationSize($aSizes[$iSrc2Index] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrc2Index]);
        if (null === $sSrc2Bytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrc2Index] . ' not a valid source 2 operand'
            );
        }

        $this->sSrc2Bytecode = $this->optimiseSourceOperandBytecode(
            $sSrc2Bytecode,
            $this->sSrcBytecode
        );

        $iInstructionSize += strlen($this->sSrc2Bytecode);
        $oState->setCurrentStatementLength($iInstructionSize);

        // TODO - code fold optimisations to either set or clear the destination
        // when the condition is compile time detectable

        return $this->sDstBytecode . $this->sSrcBytecode . $this->sSrc2Bytecode;
    }

    /**
     * Returns the expected index in the operands array for the source operand
     *
     * @return int
     */
    protected function getSource1OperandIndex(): int {
        return self::DEF_OPERAND_SRC;
    }

    /**
     * Returns the expected index in the operands array for the destination operand
     *
     * @return int
     */
    protected function getSource2OperandIndex(): int {
        return self::DEF_OPERAND_SRC_2;
    }

    /**
     * Returns the expected index in the operands array for the destination operand
     *
     * @return int
     */
    protected function getDestinationOperandIndex(): int {
        return self::DEF_OPERAND_DST;
    }

    protected abstract function getInitialInstructionSize(): int;
}

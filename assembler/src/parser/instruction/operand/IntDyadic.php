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

namespace ABadCafe\MC64K\Parser\Instruction\Operand;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Parser\Instruction;
use ABadCafe\MC64K\Defs\Mnemonic;

/**
 * IntDyadic
 *
 * For all vanilla integer destimatiom @ source -> destination operations
 */
class IntDyadic implements Instruction\IOperandParser {

    const
        OPERAND_COUNT = 2,
        OPERAND_SRC   = 0,
        OPERAND_DST   = 1
    ;

    private EffectiveAddress\IParser $oSrcParser;
    private EffectiveAddress\IParser $oDstParser;

    /***/
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser = new EffectiveAddress\AllIntegerWriteable();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return [
            Mnemonic\IArithmetic::ADD_B,
            Mnemonic\IArithmetic::ADD_W,
            Mnemonic\IArithmetic::ADD_L,
        ];
    }

    /**
     * @inheritDoc
     */
    public function parse(array $aOperands, int $iOperationSize) : string {
        $iCount = count($aOperands);
        if (self::OPERAND_COUNT !== $iCount) {
            throw new \LengthException(__CLASS__ . ' expects exactly ' . self::OPERAND_COUNT . ' operands, got ' . $iCount);
        }

        $sDst = $this->oDstParser
            ->setOperationSize($iOperationSize)
            ->parse($aOperands[self::OPERAND_DST]);
        if (null === $sDst) {
            throw new \UnexpectedValueException($aOperands[self::OPERAND_DST] . ' not a valid destination operand');
        }

        $sSrc = $this->oSrcParser
            ->setOperationSize($iOperationSize)
            ->parse($aOperands[self::OPERAND_SRC]);
        if (null === $sSrc) {
            throw new \UnexpededValueException($aOperands[self::OPERAND_SRC] . ' not a valid source operand');
        }

        // TODO - work out here if the source is a short-circuitable duplicate of the destination

        return $sDst . $sSrc;
    }
}

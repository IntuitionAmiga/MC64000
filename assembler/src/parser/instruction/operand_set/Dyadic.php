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
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs;

/**
 * Dyadic
 *
 * Base for all vanilla destination @ source -> destination operations
 */
abstract class Dyadic extends Monadic {

    const
        DEF_OPERAND_DST   = 1,
        MIN_OPERAND_COUNT = 2
    ;

    protected Parser\EffectiveAddress\IParser $oDstParser;

    /**
     * @var int[] $aSameAsDestination
     */
    protected static array $aSameAsDestination = [];

    /**
     * Base constructor
     */
    public function __construct() {
        if (empty(self::$aSameAsDestination)) {
            self::$aSameAsDestination = array_combine(
                Defs\EffectiveAddress\ISameAsDestination::LEGAL,
                Defs\EffectiveAddress\ISameAsDestination::LEGAL
            );
        }
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

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
                $aOperands[$iSrcIndex] . ' not a valid source operand');
        }

        return $sDstBytecode . $this->optimiseSourceOperandBytecode($sSrcBytecode, $sDstBytecode);
    }

    /**
     * Returns the expected index in the operands array for the destination operand
     *
     * @return int
     */
    protected function getDestinationOperandIndex() : int {
        return self::DEF_OPERAND_DST;
    }

    /**
     * Checks to see if the source operand can be optimised to "same as destination"
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return bool
     */
    protected function canOptimiseSourceOperand(string $sSrcBytecode, string $sDstBytecode) : bool {
        // If the source operand bytecode is the same as the destination and the destination mode
        // is in the set defined by ISameAsDestination, we can use the special "same as destination" EA mode
        return $sSrcBytecode === $sDstBytecode && isset(self::$aSameAsDestination[ord($sDstBytecode[0])]);
    }

    /**
     * Checks to see if the source operand bytecode can be replaced with the "same as destination" EA mode.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string bytecode
     */
    protected function optimiseSourceOperandBytecode(string $sSrcBytecode, string $sDstBytecode) : string {
        if ($this->canOptimiseSourceOperand($sSrcBytecode, $sDstBytecode)) {
            $sSrcBytecode = chr(Defs\EffectiveAddress\IOther::SAME_AS_DEST);
        }
        return $sSrcBytecode;
    }
}

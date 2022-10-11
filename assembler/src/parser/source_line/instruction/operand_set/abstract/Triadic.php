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
        DEF_OPERAND_SRC_2 = 1,
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
        // TODO
        throw new \Exception(__METHOD__ . "() TODO");
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

}

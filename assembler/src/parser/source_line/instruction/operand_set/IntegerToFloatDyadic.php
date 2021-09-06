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
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;

/**
 * IntegerToFloatDyadic
 *
 * For all vanilla integer to float conversion operations.
 */
class IntegerToFloatDyadic extends Dyadic {

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IDataMove::FMOVEB_S,
        IDataMove::FMOVEB_D,
        IDataMove::FMOVEW_S,
        IDataMove::FMOVEW_D,
        IDataMove::FMOVEL_S,
        IDataMove::FMOVEL_D,
        IDataMove::FMOVEQ_S,
        IDataMove::FMOVEQ_D,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser = new EffectiveAddress\AllFloatWriteable();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return self::OPCODES;
    }
}

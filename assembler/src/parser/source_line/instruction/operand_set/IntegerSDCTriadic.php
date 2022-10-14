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
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;

use function \array_keys, \strlen;

/**
 * IntegerSDCTriadic
 *
 * Used for implemeting the Integer Dyadic Compare and Set
 */
class IntegerSDCTriadic extends Triadic {

    //use EffectiveAddress\TPotentiallyFoldableImmediateAware;

    const
        NO_OP    = 0,
        CLEAR_8  = 1,
        CLEAR_16 = 2,
        CLEAR_32 = 3,
        CLEAR_64 = 4,
        FOLD     = 5,
        THROW    = 6
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to, mapped to folding
     * cases for given immediate source operands
     */
    const OPCODES = [
        // Set if <ea(s)> < <ea(d)>      // Triadic operations
        IDataMove::SLO_B   => [], // unsigned: Lower
        IDataMove::SLO_W   => [],
        IDataMove::SLO_L   => [],
        IDataMove::SLO_Q   => [],

        IDataMove::SLT_B   => [], // signed: Less Than
        IDataMove::SLT_W   => [],
        IDataMove::SLT_L   => [],
        IDataMove::SLT_Q   => [],

        // Set if <ea(s)> <= <ea(d)>
        IDataMove::SLS_B   => [], // unsigned: Lower or Same
        IDataMove::SLS_W   => [],
        IDataMove::SLS_L   => [],
        IDataMove::SLS_Q   => [],

        IDataMove::SLE_B   => [], // signed: Less or Equal
        IDataMove::SLE_W   => [],
        IDataMove::SLE_L   => [],
        IDataMove::SLE_Q   => [],

        // Set if <ea(s)> == <ea(d)>
        IDataMove::SEQ_B   => [],
        IDataMove::SEQ_W   => [],
        IDataMove::SEQ_L   => [],
        IDataMove::SEQ_Q   => [],

        // Set if <ea(s)> >= <ea(d)>
        IDataMove::SHS_B   => [], // unsigned: Higher or Same
        IDataMove::SHS_W   => [],
        IDataMove::SHS_L   => [],
        IDataMove::SHS_Q   => [],

        IDataMove::SGE_B   => [], // signed: Greater or Equal
        IDataMove::SGE_W   => [],
        IDataMove::SGE_L   => [],
        IDataMove::SGE_Q   => [],

        // Set if <ea(s)> > <ea(d)>
        IDataMove::SHI_B   => [], // unsigned: Higher
        IDataMove::SHI_W   => [],
        IDataMove::SHI_L   => [],
        IDataMove::SHI_Q   => [],

        IDataMove::SGT_B   => [], // signed: Greater Than
        IDataMove::SGT_W   => [],
        IDataMove::SGT_L   => [],
        IDataMove::SGT_Q   => [],

        // Set if <ea(s)> != <ea(d)>
        IDataMove::SNE_B   => [],
        IDataMove::SNE_W   => [],
        IDataMove::SNE_L   => [],
        IDataMove::SNE_Q   => [],

        // Set if bit set
        IDataMove::SBS_B   => [],
        IDataMove::SBS_W   => [],
        IDataMove::SBS_L   => [],
        IDataMove::SBS_Q   => [],

        // Set if bit clear
        IDataMove::SBC_B   => [],
        IDataMove::SBC_W   => [],
        IDataMove::SBC_L   => [],
        IDataMove::SBC_Q   => [],

    ];


    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser  = new EffectiveAddress\AllIntegerReadable();
        $this->oSrc2Parser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser  = new EffectiveAddress\AllIntegerWriteable();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return array_keys(self::OPCODES);
    }

    /**
     * @inheritDoc
     */
    protected function getInitialInstructionSize(int $iOpcode): int {
        return Defs\IOpcodeLimits::SIZE_SUB;
    }
}

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
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;

/**
 * IntegerDyadic
 *
 * For all vanilla integer destination @ source -> destination operations
 */
class IntegerDyadic extends Dyadic {

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IDataMove::MOVE_B,
        IDataMove::MOVE_W,
        IDataMove::MOVE_L,
        IDataMove::MOVE_Q,
        IDataMove::LEA,
        ILogical::AND_B,
        ILogical::AND_W,
        ILogical::AND_L,
        ILogical::AND_Q,
        ILogical::OR_B,
        ILogical::OR_W,
        ILogical::OR_L,
        ILogical::OR_Q,
        ILogical::EOR_B,
        ILogical::EOR_W,
        ILogical::EOR_L,
        ILogical::EOR_Q,
        ILogical::NOT_B,
        ILogical::NOT_W,
        ILogical::NOT_L,
        ILogical::NOT_Q,
        ILogical::LSL_B,
        ILogical::LSL_W,
        ILogical::LSL_L,
        ILogical::LSL_Q,
        ILogical::LSR_B,
        ILogical::LSR_W,
        ILogical::LSR_L,
        ILogical::LSR_Q,
        ILogical::ROL_B,
        ILogical::ROL_W,
        ILogical::ROL_L,
        ILogical::ROL_Q,
        ILogical::ROR_B,
        ILogical::ROR_W,
        ILogical::ROR_L,
        ILogical::ROR_Q,
        ILogical::BCLR_B,
        ILogical::BCLR_W,
        ILogical::BCLR_L,
        ILogical::BCLR_Q,
        ILogical::BSET_B,
        ILogical::BSET_W,
        ILogical::BSET_L,
        ILogical::BSET_Q,
        IArithmetic::EXTB_W,
        IArithmetic::EXTB_L,
        IArithmetic::EXTB_Q,
        IArithmetic::EXTW_L,
        IArithmetic::EXTW_Q,
        IArithmetic::EXTL_Q,
        IArithmetic::ASL_B,
        IArithmetic::ASL_W,
        IArithmetic::ASL_L,
        IArithmetic::ASL_Q,
        IArithmetic::ASR_B,
        IArithmetic::ASR_W,
        IArithmetic::ASR_L,
        IArithmetic::ASR_Q,
        IArithmetic::ADD_B,
        IArithmetic::ADD_W,
        IArithmetic::ADD_L,
        IArithmetic::ADD_Q,
        IArithmetic::SUB_B,
        IArithmetic::SUB_W,
        IArithmetic::SUB_L,
        IArithmetic::SUB_Q,
        IArithmetic::NEG_B,
        IArithmetic::NEG_W,
        IArithmetic::NEG_L,
        IArithmetic::NEG_Q,
        IArithmetic::MULS_B,
        IArithmetic::MULS_W,
        IArithmetic::MULS_L,
        IArithmetic::MULS_Q,
        IArithmetic::MULU_B,
        IArithmetic::MULU_W,
        IArithmetic::MULU_L,
        IArithmetic::MULU_Q,
        IArithmetic::DIVS_B,
        IArithmetic::DIVS_W,
        IArithmetic::DIVS_L,
        IArithmetic::DIVS_Q,
        IArithmetic::DIVU_B,
        IArithmetic::DIVU_W,
        IArithmetic::DIVU_L,
        IArithmetic::DIVU_Q,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser = new EffectiveAddress\AllIntegerWriteable();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return self::OPCODES;
    }
}

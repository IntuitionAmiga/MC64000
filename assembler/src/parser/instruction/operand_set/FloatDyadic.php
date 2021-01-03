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
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;

/**
 * FloatDyadic
 *
 * For all vanilla float destination @ source -> destination operations
 */
class FloatDyadic extends Dyadic {

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IDataMove::FMOVES_D,
        IDataMove::FMOVED_S,
        IArithmetic::FADD_S,
        IArithmetic::FADD_D,
        IArithmetic::FSUB_S,
        IArithmetic::FSUB_D,
        IArithmetic::FNEG_S,
        IArithmetic::FNEG_D,
        IArithmetic::FMUL_S,
        IArithmetic::FMUL_D,
        IArithmetic::FDIV_S,
        IArithmetic::FDIV_D,
        IArithmetic::FMOD_S,
        IArithmetic::FMOD_D,
        IArithmetic::FABS_S,
        IArithmetic::FABS_D,
        IArithmetic::FSQRT_S,
        IArithmetic::FSQRT_D,
        IArithmetic::FACOS_S,
        IArithmetic::FACOS_D,
        IArithmetic::FASIN_S,
        IArithmetic::FASIN_D,
        IArithmetic::FATAN_S,
        IArithmetic::FATAN_D,
        IArithmetic::FCOS_S,
        IArithmetic::FCOS_D,
        IArithmetic::FSIN_S,
        IArithmetic::FSIN_D,
        IArithmetic::FSINCOS_S,
        IArithmetic::FSINCOS_D,
        IArithmetic::FTAN_S,
        IArithmetic::FTAN_D,
        IArithmetic::FETOX_S,
        IArithmetic::FETOX_D,
        IArithmetic::FLOGN_S,
        IArithmetic::FLOGN_D,
        IArithmetic::FLOG2_S,
        IArithmetic::FLOG2_D,
        IArithmetic::FTWOTOX_S,
        IArithmetic::FTWOTOX_D,
        IArithmetic::FGETEXP_S,
        IArithmetic::FGETEXP_D,
        IArithmetic::FGETMAN_S,
        IArithmetic::FGETMAN_D,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new Parser\EffectiveAddress\AllFloatReadable();
        $this->oDstParser = new Parser\EffectiveAddress\AllFloatWriteable();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return self::OPCODES;
    }
}

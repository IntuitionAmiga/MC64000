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

namespace ABadCafe\MC64K\Defs\Mnemonic;

/**
 * IOperandSizes
 *
 * Defines the expected sizes of operands associated with each instruction.
 */
interface IOperandSizes {
    const MAP = [

        IControl::BMC << 8 | ICondition::IEQ_B => [1],
        IControl::BMC << 8 | ICondition::IEQ_W => [2],
        IControl::BMC << 8 | ICondition::IEQ_L => [4],
        IControl::BMC << 8 | ICondition::IEQ_Q => [8],
        IControl::BMC << 8 | ICondition::FEQ_S => [4],
        IControl::BMC << 8 | ICondition::FEQ_D => [8],

        // Branch if <ea> != 0
        IControl::BMC << 8 | ICondition::INE_B => [1],
        IControl::BMC << 8 | ICondition::INE_W => [2],
        IControl::BMC << 8 | ICondition::INE_L => [4],
        IControl::BMC << 8 | ICondition::INE_Q => [8],
        IControl::BMC << 8 | ICondition::FNE_S => [4],
        IControl::BMC << 8 | ICondition::FNE_D => [8],

        // Branch if <ea> < 0
        IControl::BMC << 8 | ICondition::ILT_B => [1],
        IControl::BMC << 8 | ICondition::ILT_W => [2],
        IControl::BMC << 8 | ICondition::ILT_L => [4],
        IControl::BMC << 8 | ICondition::ILT_Q => [8],
        IControl::BMC << 8 | ICondition::FLT_S => [4],
        IControl::BMC << 8 | ICondition::FLT_D => [8],

        // Branch if <ea> > 0
        IControl::BMC << 8 | ICondition::IGT_B => [1],
        IControl::BMC << 8 | ICondition::IGT_W => [2],
        IControl::BMC << 8 | ICondition::IGT_L => [4],
        IControl::BMC << 8 | ICondition::IGT_Q => [8],
        IControl::BMC << 8 | ICondition::FGT_S => [4],
        IControl::BMC << 8 | ICondition::FGT_D => [8],

        // Branch if <ea(s)> < <ea(d)>
        IControl::BDC << 8 | ICondition::ULT_B => [1, 1], // unsigned: Lower
        IControl::BDC << 8 | ICondition::ULT_W => [2, 2],
        IControl::BDC << 8 | ICondition::ULT_L => [4, 4],
        IControl::BDC << 8 | ICondition::ULT_Q => [8, 8],

        IControl::BDC << 8 | ICondition::ILT_B => [1, 1], // signed: Less Than
        IControl::BDC << 8 | ICondition::ILT_W => [2, 2],
        IControl::BDC << 8 | ICondition::ILT_L => [4, 4],
        IControl::BDC << 8 | ICondition::ILT_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FLT_S => [4, 4],
        IControl::BDC << 8 | ICondition::FLT_D => [8, 8],

        // Branch if <ea(s)> <= <ea(d)>
        IControl::BDC << 8 | ICondition::ULE_B => [1, 1], // unsigned: Lower or Same
        IControl::BDC << 8 | ICondition::ULE_W => [2, 2],
        IControl::BDC << 8 | ICondition::ULE_L => [4, 4],
        IControl::BDC << 8 | ICondition::ULE_Q => [8, 8],

        IControl::BDC << 8 | ICondition::ILE_B => [1, 1], // signed: Less or Equal
        IControl::BDC << 8 | ICondition::ILE_W => [2, 2],
        IControl::BDC << 8 | ICondition::ILE_L => [4, 4],
        IControl::BDC << 8 | ICondition::ILE_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FLE_S => [4, 4],
        IControl::BDC << 8 | ICondition::FLE_D => [8, 8],

        // Branch if <ea(s)> == <ea(d)>
        IControl::BDC << 8 | ICondition::IEQ_B => [1, 1],
        IControl::BDC << 8 | ICondition::IEQ_W => [2, 2],
        IControl::BDC << 8 | ICondition::IEQ_L => [4, 4],
        IControl::BDC << 8 | ICondition::IEQ_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FEQ_S => [4, 4],
        IControl::BDC << 8 | ICondition::FEQ_D => [8, 8],

        // Branch if <ea(s)> >= <ea(d)>
        IControl::BDC << 8 | ICondition::UGE_B => [1, 1], // unsigned: Higher or Same
        IControl::BDC << 8 | ICondition::UGE_W => [2, 2],
        IControl::BDC << 8 | ICondition::UGE_L => [4, 4],
        IControl::BDC << 8 | ICondition::UGE_Q => [8, 8],

        IControl::BDC << 8 | ICondition::IGE_B => [1, 1], // signed: Greater or Equal
        IControl::BDC << 8 | ICondition::IGE_W => [2, 2],
        IControl::BDC << 8 | ICondition::IGE_L => [4, 4],
        IControl::BDC << 8 | ICondition::IGE_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FGE_S => [4, 4],
        IControl::BDC << 8 | ICondition::FGE_D => [8, 8],

        // Branch if <ea(s)> > <ea(d)>
        IControl::BDC << 8 | ICondition::UGT_B => [1, 1], // unsigned: Higher
        IControl::BDC << 8 | ICondition::UGT_W => [2, 2],
        IControl::BDC << 8 | ICondition::UGT_L => [4, 4],
        IControl::BDC << 8 | ICondition::UGT_Q => [8, 8],

        IControl::BDC << 8 | ICondition::IGT_B => [1, 1], // signed: Greater Than
        IControl::BDC << 8 | ICondition::IGT_W => [2, 2],
        IControl::BDC << 8 | ICondition::IGT_L => [4, 4],
        IControl::BDC << 8 | ICondition::IGT_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FGT_S => [4, 4],
        IControl::BDC << 8 | ICondition::FGT_D => [8, 8],

        // Branch if <ea(s)> != <ea(d)>
        IControl::BDC << 8 | ICondition::INE_B => [1, 1],
        IControl::BDC << 8 | ICondition::INE_W => [2, 2],
        IControl::BDC << 8 | ICondition::INE_L => [4, 4],
        IControl::BDC << 8 | ICondition::INE_Q => [8, 8],
        IControl::BDC << 8 | ICondition::FNE_S => [4, 4],
        IControl::BDC << 8 | ICondition::FNE_D => [8, 8],

        // Branch if bit set
        IControl::BDC << 8 | ICondition::BPS_B => [1, 1],
        IControl::BDC << 8 | ICondition::BPS_W => [1, 2],
        IControl::BDC << 8 | ICondition::BPS_L => [1, 4],
        IControl::BDC << 8 | ICondition::BPS_Q => [1, 8],

        // Branch if bit clear
        IControl::BDC << 8 | ICondition::BPC_B => [1, 1],
        IControl::BDC << 8 | ICondition::BPC_W => [1, 2],
        IControl::BDC << 8 | ICondition::BPC_L => [1, 4],
        IControl::BDC << 8 | ICondition::BPC_B => [1, 8],

        // Go on, why not?
        IControl::DBNZ  => [4],

        // Data Movement
        IDataMove::MOVE_B => [1, 1],
        IDataMove::MOVE_W => [2, 2],
        IDataMove::MOVE_L => [4, 4],
        IDataMove::MOVE_Q => [8, 8],

        IDataMove::FMOVEB_S => [1, 4],
        IDataMove::FMOVEB_D => [1, 8],
        IDataMove::FMOVEW_S => [2, 4],
        IDataMove::FMOVEW_D => [2, 8],
        IDataMove::FMOVEL_S => [4, 4],
        IDataMove::FMOVEL_D => [4, 8],
        IDataMove::FMOVEQ_S => [8, 4],
        IDataMove::FMOVEQ_D => [8, 8],
        IDataMove::FMOVES_L => [4, 4],
        IDataMove::FMOVES_Q => [4, 8],
        IDataMove::FMOVES_D => [4, 8],
        IDataMove::FMOVED_L => [8, 4],
        IDataMove::FMOVED_Q => [8, 8],
        IDataMove::FMOVED_S => [8, 4],
        IDataMove::FMOVE_S  => [4, 4],
        IDataMove::FMOVE_D  => [8, 8],
        IDataMove::FINFO_S  => [4, 1],
        IDataMove::FINFO_D  => [8, 1],
        IDataMove::CLR_B    => [1, 1],
        IDataMove::CLR_W    => [2, 2],
        IDataMove::CLR_L    => [4, 4],
        IDataMove::CLR_Q    => [8, 8],
        IDataMove::LINK     => [8, 4],
        IDataMove::UNLK     => [8],

        // Logical
        ILogical::AND_B => [1, 1],
        ILogical::AND_W => [2, 2],
        ILogical::AND_L => [4, 4],
        ILogical::AND_Q => [8, 8],

        ILogical::OR_B  => [1, 1],
        ILogical::OR_W  => [2, 2],
        ILogical::OR_L  => [4, 4],
        ILogical::OR_Q  => [8, 8],
        ILogical::EOR_B => [1, 1],
        ILogical::EOR_W => [2, 2],
        ILogical::EOR_L => [4, 4],
        ILogical::EOR_Q => [8, 8],
        ILogical::NOT_B => [1, 1],
        ILogical::NOT_W => [2, 2],
        ILogical::NOT_L => [4, 4],
        ILogical::NOT_Q => [8, 8],

        ILogical::LSL_B => [1, 1],
        ILogical::LSL_W => [1, 2],
        ILogical::LSL_L => [1, 4],
        ILogical::LSL_Q => [1, 8],
        ILogical::LSR_B => [1, 1],
        ILogical::LSR_W => [1, 2],
        ILogical::LSR_L => [1, 4],
        ILogical::LSR_Q => [1, 8],
        ILogical::ROL_B => [1, 1],
        ILogical::ROL_W => [1, 2],
        ILogical::ROL_L => [1, 4],
        ILogical::ROL_Q => [1, 8],
        ILogical::ROR_B => [1, 1],
        ILogical::ROR_W => [1, 2],
        ILogical::ROR_L => [1, 4],
        ILogical::ROR_Q => [1, 8],

        ILogical::BCLR_B => [1, 1],
        ILogical::BCLR_W => [1, 2],
        ILogical::BCLR_L => [1, 4],
        ILogical::BCLR_Q => [1, 8],
        ILogical::BSET_B => [1, 1],
        ILogical::BSET_W => [1, 2],
        ILogical::BSET_L => [1, 4],
        ILogical::BSET_Q => [1, 8],

        // Sign extension
        IArithmetic::EXTB_W => [1, 2],
        IArithmetic::EXTB_L => [1, 4],
        IArithmetic::EXTB_Q => [1, 8],
        IArithmetic::EXTW_L => [2, 4],
        IArithmetic::EXTW_Q => [2, 8],
        IArithmetic::EXTL_Q => [4, 8],

        // Arithmetic shift
        IArithmetic::ASL_B  => [1, 1],
        IArithmetic::ASL_W  => [1, 2],
        IArithmetic::ASL_L  => [1, 4],
        IArithmetic::ASL_Q  => [1, 8],
        IArithmetic::ASR_B  => [1, 1],
        IArithmetic::ASR_W  => [1, 2],
        IArithmetic::ASR_L  => [1, 4],
        IArithmetic::ASR_Q  => [1, 8],

        // Addition
        IArithmetic::ADD_B  => [1, 1],
        IArithmetic::ADD_W  => [2, 2],
        IArithmetic::ADD_L  => [4, 4],
        IArithmetic::ADD_Q  => [8, 8],
        IArithmetic::FADD_S => [4, 4],
        IArithmetic::FADD_D => [8, 8],

        // Subtraction
        IArithmetic::SUB_B  => [1, 1],
        IArithmetic::SUB_W  => [2, 2],
        IArithmetic::SUB_L  => [4, 4],
        IArithmetic::SUB_Q  => [8, 8],
        IArithmetic::FSUB_S => [4, 4],
        IArithmetic::FSUB_D => [8, 8],

        // Negation
        IArithmetic::NEG_B  => [1, 1],
        IArithmetic::NEG_W  => [2, 2],
        IArithmetic::NEG_L  => [4, 4],
        IArithmetic::NEG_Q  => [8, 8],
        IArithmetic::FNEG_S => [4, 4],
        IArithmetic::FNEG_D => [8, 8],

        // Multiplication
        IArithmetic::MULS_B => [1, 1],
        IArithmetic::MULS_W => [2, 2],
        IArithmetic::MULS_L => [4, 4],
        IArithmetic::MULS_Q => [8, 8],
        IArithmetic::MULU_B => [1, 1],
        IArithmetic::MULU_W => [2, 2],
        IArithmetic::MULU_L => [4, 4],
        IArithmetic::MULU_Q => [8, 8],
        IArithmetic::FMUL_S => [4, 4],
        IArithmetic::FMUL_D => [8, 8],

        // Division
        IArithmetic::DIVS_L => [4, 4],
        IArithmetic::DIVS_Q => [8, 8],
        IArithmetic::MODS_L => [4, 4],
        IArithmetic::MODS_Q => [8, 8],
        IArithmetic::DIVU_L => [4, 4],
        IArithmetic::DIVU_Q => [8, 8],
        IArithmetic::MODU_L => [4, 4],
        IArithmetic::MODU_Q => [8, 8],
        IArithmetic::FDIV_S => [4, 4],
        IArithmetic::FDIV_D => [8, 8],
        IArithmetic::FMOD_S => [4, 4],
        IArithmetic::FMOD_D => [8, 8],

        //
        IArithmetic::FABS_S    => [4, 4],
        IArithmetic::FABS_D    => [8, 8],
        IArithmetic::FSQRT_S   => [4, 4],
        IArithmetic::FSQRT_D   => [8, 8],
        IArithmetic::FACOS_S   => [4, 4],
        IArithmetic::FACOS_D   => [8, 8],
        IArithmetic::FASIN_S   => [4, 4],
        IArithmetic::FASIN_D   => [8, 8],
        IArithmetic::FATAN_S   => [4, 4],
        IArithmetic::FATAN_D   => [8, 8],
        IArithmetic::FCOS_S    => [4, 4],
        IArithmetic::FCOS_D    => [8, 8],
        IArithmetic::FSIN_S    => [4, 4],
        IArithmetic::FSIN_D    => [8, 8],
        IArithmetic::FSINCOS_S => [4, 4],
        IArithmetic::FSINCOS_D => [8, 8],
        IArithmetic::FTAN_S    => [4, 4],
        IArithmetic::FTAN_D    => [8, 8],
        IArithmetic::FETOX_S   => [4, 4],
        IArithmetic::FETOX_D   => [8, 8],
        IArithmetic::FLOGN_S   => [4, 4],
        IArithmetic::FLOGN_D   => [8, 8],
        IArithmetic::FLOG2_S   => [4, 4],
        IArithmetic::FLOG2_D   => [8, 8],
        IArithmetic::FTWOTOX_S => [4, 4],
        IArithmetic::FTWOTOX_D => [8, 8],
    ];


}

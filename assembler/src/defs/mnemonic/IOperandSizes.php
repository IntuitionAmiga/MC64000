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

        IControl::BIZ_B => [1],
        IControl::BIZ_W => [2],
        IControl::BIZ_L => [4],
        IControl::BIZ_Q => [8],
        IControl::FBIZ_S => [4],
        IControl::FBIZ_D => [8],

        // Branch if <ea> != 0
        IControl::BNZ_B => [1],
        IControl::BNZ_W => [2],
        IControl::BNZ_L => [4],
        IControl::BNZ_Q => [8],
        IControl::FBNZ_S => [4],
        IControl::FBNZ_D => [8],

        // Branch if <ea> < 0
        IControl::BMI_B => [1],
        IControl::BMI_W => [2],
        IControl::BMI_L => [4],
        IControl::BMI_Q => [8],
        IControl::FBMI_S => [4],
        IControl::FBMI_D => [8],

        // Branch if <ea> > 0
        IControl::BPL_B => [1],
        IControl::BPL_W => [2],
        IControl::BPL_L => [4],
        IControl::BPL_Q => [8],
        IControl::FBPL_S => [4],
        IControl::FBPL_D => [8],

        // Branch if <ea(s)> < <ea(d)>
        IControl::BLO_B => [1, 1], // unsigned: Lower
        IControl::BLO_W => [2, 2],
        IControl::BLO_L => [4, 4],
        IControl::BLO_Q => [8, 8],

        IControl::BLT_B => [1, 1], // signed: Less Than
        IControl::BLT_W => [2, 2],
        IControl::BLT_L => [4, 4],
        IControl::BLT_Q => [8, 8],
        IControl::FBLT_S => [4, 4],
        IControl::FBLT_D => [8, 8],

        // Branch if <ea(s)> <= <ea(d)>
        IControl::BLS_B => [1, 1], // unsigned: Lower or Same
        IControl::BLS_W => [2, 2],
        IControl::BLS_L => [4, 4],
        IControl::BLS_Q => [8, 8],

        IControl::BLE_B => [1, 1], // signed: Less or Equal
        IControl::BLE_W => [2, 2],
        IControl::BLE_L => [4, 4],
        IControl::BLE_Q => [8, 8],
        IControl::FBLE_S => [4, 4],
        IControl::FBLE_D => [8, 8],

        // Branch if <ea(s)> == <ea(d)>
        IControl::BEQ_B => [1, 1],
        IControl::BEQ_W => [2, 2],
        IControl::BEQ_L => [4, 4],
        IControl::BEQ_Q => [8, 8],
        IControl::FBEQ_S => [4, 4],
        IControl::FBEQ_D => [8, 8],

        // Branch if <ea(s)> >= <ea(d)>
        IControl::BHS_B => [1, 1], // unsigned: Higher or Same
        IControl::BHS_W => [2, 2],
        IControl::BHS_L => [4, 4],
        IControl::BHS_Q => [8, 8],

        IControl::BGE_B => [1, 1], // signed: Greater or Equal
        IControl::BGE_W => [2, 2],
        IControl::BGE_L => [4, 4],
        IControl::BGE_Q => [8, 8],
        IControl::FBGE_S => [4, 4],
        IControl::FBGE_D => [8, 8],

        // Branch if <ea(s)> > <ea(d)>
        IControl::BHI_B => [1, 1], // unsigned: Higher
        IControl::BHI_W => [2, 2],
        IControl::BHI_L => [4, 4],
        IControl::BHI_Q => [8, 8],

        IControl::BGT_B => [1, 1], // signed: Greater Than
        IControl::BGT_W => [2, 2],
        IControl::BGT_L => [4, 4],
        IControl::BGT_Q => [8, 8],
        IControl::FBGT_S => [4, 4],
        IControl::FBGT_D => [8, 8],

        // Branch if <ea(s)> != <ea(d)>
        IControl::BNE_B => [1, 1],
        IControl::BNE_W => [2, 2],
        IControl::BNE_L => [4, 4],
        IControl::BNE_Q => [8, 8],
        IControl::FBNE_S => [4, 4],
        IControl::FBNE_D => [8, 8],

        // Branch if bit set
        IControl::BBS_B => [1, 1],
        IControl::BBS_W => [1, 2],
        IControl::BBS_L => [1, 4],
        IControl::BBS_Q => [1, 8],

        // Branch if bit clear
        IControl::BBC_B => [1, 1],
        IControl::BBC_W => [1, 2],
        IControl::BBC_L => [1, 4],
        IControl::BBC_B => [1, 8],

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

        // Set if <ea> == 0
        IDataMove::SIZ_B   => [1, 1],
        IDataMove::SIZ_W   => [2, 1],
        IDataMove::SIZ_L   => [4, 1],
        IDataMove::SIZ_Q   => [8, 1],
        IDataMove::FSIZ_S  => [4, 1],
        IDataMove::FSIZ_D  => [8, 1],

        // Set if <ea> != 0
        IDataMove::SNZ_B   => [1, 1],
        IDataMove::SNZ_W   => [2, 1],
        IDataMove::SNZ_L   => [4, 1],
        IDataMove::SNZ_Q   => [8, 1],
        IDataMove::FSNZ_S  => [4, 1],
        IDataMove::FSNZ_D  => [8, 1],

        // Set if <ea> < 0
        IDataMove::SMI_B   => [1, 1],
        IDataMove::SMI_W   => [2, 1],
        IDataMove::SMI_L   => [4, 1],
        IDataMove::SMI_Q   => [8, 1],
        IDataMove::FSMI_S  => [4, 1],
        IDataMove::FSMI_D  => [8, 1],

        // Set if <ea> > 0
        IDataMove::SPL_B   => [1, 1],
        IDataMove::SPL_W   => [2, 1],
        IDataMove::SPL_L   => [4, 1],
        IDataMove::SPL_Q   => [8, 1],
        IDataMove::FSPL_S  => [4, 1],
        IDataMove::FSPL_D  => [8, 1],


        // Set if <ea(s)> < <ea(d)>      // Triadic operations
        IDataMove::SLO_B   => [1, 1, 1], // unsigned: Lower
        IDataMove::SLO_W   => [2, 2, 1],
        IDataMove::SLO_L   => [4, 4, 1],
        IDataMove::SLO_Q   => [8, 8, 1],

        IDataMove::SLT_B   => [1, 1, 1], // signed: Less Than
        IDataMove::SLT_W   => [2, 2, 1],
        IDataMove::SLT_L   => [4, 4, 1],
        IDataMove::SLT_Q   => [8, 8, 1],
        IDataMove::FSLT_S  => [4, 4, 1],
        IDataMove::FSLT_D  => [8, 8, 1],

        // Set if <ea(s)> <= <ea(d)>
        IDataMove::SLS_B   => [1, 1, 1], // unsigned: Lower or Same
        IDataMove::SLS_W   => [2, 2, 1],
        IDataMove::SLS_L   => [4, 4, 1],
        IDataMove::SLS_Q   => [8, 8, 1],

        IDataMove::SLE_B   => [1, 1, 1], // signed: Less or Equal
        IDataMove::SLE_W   => [2, 2, 1],
        IDataMove::SLE_L   => [4, 4, 1],
        IDataMove::SLE_Q   => [8, 8, 1],
        IDataMove::FSLE_S  => [4, 4, 1],
        IDataMove::FSLE_D  => [8, 8, 1],

        // Set if <ea(s)> == <ea(d)>
        IDataMove::SEQ_B   => [1, 1, 1],
        IDataMove::SEQ_W   => [2, 2, 1],
        IDataMove::SEQ_L   => [4, 4, 1],
        IDataMove::SEQ_Q   => [8, 8, 1],
        IDataMove::FSEQ_S  => [4, 4, 1],
        IDataMove::FSEQ_D  => [8, 8, 1],

        // Set if <ea(s)> >= <ea(d)>
        IDataMove::SHS_B   => [1, 1, 1], // unsigned: Higher or Same
        IDataMove::SHS_W   => [2, 2, 1],
        IDataMove::SHS_L   => [4, 4, 1],
        IDataMove::SHS_Q   => [8, 8, 1],

        IDataMove::SGE_B   => [1, 1, 1], // signed: Greater or Equal
        IDataMove::SGE_W   => [2, 2, 1],
        IDataMove::SGE_L   => [4, 4, 1],
        IDataMove::SGE_Q   => [8, 8, 1],
        IDataMove::FSGE_S  => [4, 4, 1],
        IDataMove::FSGE_D  => [8, 8, 1],

        // Set if <ea(s)> > <ea(d)>
        IDataMove::SHI_B   => [1, 1, 1], // unsigned: Higher
        IDataMove::SHI_W   => [2, 2, 1],
        IDataMove::SHI_L   => [4, 4, 1],
        IDataMove::SHI_Q   => [8, 8, 1],

        IDataMove::SGT_B   => [1, 1, 1], // signed: Greater Than
        IDataMove::SGT_W   => [2, 2, 1],
        IDataMove::SGT_L   => [4, 4, 1],
        IDataMove::SGT_Q   => [8, 8, 1],
        IDataMove::FSGT_S  => [4, 4, 1],
        IDataMove::FSGT_D  => [8, 8, 1],

        // Set if <ea(s)> != <ea(d)>
        IDataMove::SNE_B   => [1, 1, 1],
        IDataMove::SNE_W   => [2, 2, 1],
        IDataMove::SNE_L   => [4, 4, 1],
        IDataMove::SNE_Q   => [8, 8, 1],
        IDataMove::FSNE_S  => [4, 4, 1],
        IDataMove::FSNE_D  => [8, 8, 1],

        // Set if bit set
        IDataMove::SBS_B   => [1, 1, 1],
        IDataMove::SBS_W   => [2, 2, 1],
        IDataMove::SBS_L   => [4, 4, 1],
        IDataMove::SBS_Q   => [8, 8, 1],

        // Set if bit clear
        IDataMove::SBC_B   => [1, 1, 1],
        IDataMove::SBC_W   => [2, 2, 1],
        IDataMove::SBC_L   => [4, 4, 1],
        IDataMove::SBC_Q   => [8, 8, 1],

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

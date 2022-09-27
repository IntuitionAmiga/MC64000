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
 * IMatches
 *
 * Maps mnemonic names to their corresponding bytecode values
 */
interface IMatches {

    // Direct opcode matches (one byte)
    const MATCHES = [
        'stop'    => IControl::STOP,

        // Flow control group
        'hcf'     => IControl::HOST,
        'bra.b'   => IControl::BRA_B,
        'bra'     => IControl::BRA,
        'bsr.b'   => IControl::BSR_B,
        'bsr'     => IControl::BSR,
        'jmp'     => IControl::JMP,
        'jsr'     => IControl::JSR,
        'rts'     => IControl::RTS,

        // Branch if <ea> == 0
        'biz.b'   => IControl::BMC << 8 | ICondition::IEQ_B,
        'biz.w'   => IControl::BMC << 8 | ICondition::IEQ_W,
        'biz.l'   => IControl::BMC << 8 | ICondition::IEQ_L,
        'biz.q'   => IControl::BMC << 8 | ICondition::IEQ_Q,
        'fbiz.s'  => IControl::BMC << 8 | ICondition::FEQ_S,
        'fbiz.d'  => IControl::BMC << 8 | ICondition::FEQ_D,

        // Branch if <ea> != 0
        'bnz.b'   => IControl::BMC << 8 | ICondition::INE_B,
        'bnz.w'   => IControl::BMC << 8 | ICondition::INE_W,
        'bnz.l'   => IControl::BMC << 8 | ICondition::INE_L,
        'bnz.q'   => IControl::BMC << 8 | ICondition::INE_Q,
        'fbnz.s'  => IControl::BMC << 8 | ICondition::FNE_S,
        'fbnz.d'  => IControl::BMC << 8 | ICondition::FNE_D,

        // Branch if <ea> < 0
        'bmi.b'   => IControl::BMC << 8 | ICondition::ILT_B,
        'bmi.w'   => IControl::BMC << 8 | ICondition::ILT_W,
        'bmi.l'   => IControl::BMC << 8 | ICondition::ILT_L,
        'bmi.q'   => IControl::BMC << 8 | ICondition::ILT_Q,
        'fbmi.s'  => IControl::BMC << 8 | ICondition::FLT_S,
        'fbmi.d'  => IControl::BMC << 8 | ICondition::FLT_D,

        // Branch if <ea> > 0
        'bpl.b'   => IControl::BMC << 8 | ICondition::IGT_B,
        'bpl.w'   => IControl::BMC << 8 | ICondition::IGT_W,
        'bpl.l'   => IControl::BMC << 8 | ICondition::IGT_L,
        'bpl.q'   => IControl::BMC << 8 | ICondition::IGT_Q,
        'fbpl.s'  => IControl::BMC << 8 | ICondition::FGT_S,
        'fbpl.d'  => IControl::BMC << 8 | ICondition::FGT_D,

        // Branch if <ea(s)> < <ea(d)>
        'blo.b'   => IControl::BDC << 8 | ICondition::ULT_B, // unsigned: Lower
        'blo.w'   => IControl::BDC << 8 | ICondition::ULT_W,
        'blo.l'   => IControl::BDC << 8 | ICondition::ULT_L,
        'blo.q'   => IControl::BDC << 8 | ICondition::ULT_Q,

        'blt.b'   => IControl::BDC << 8 | ICondition::ILT_B, // signed: Less Than
        'blt.w'   => IControl::BDC << 8 | ICondition::ILT_W,
        'blt.l'   => IControl::BDC << 8 | ICondition::ILT_L,
        'blt.q'   => IControl::BDC << 8 | ICondition::ILT_Q,
        'fblt.s'  => IControl::BDC << 8 | ICondition::FLT_S,
        'fblt.d'  => IControl::BDC << 8 | ICondition::FLT_D,

        // Branch if <ea(s)> <= <ea(d)>
        'bls.b'   => IControl::BDC << 8 | ICondition::ULE_B, // unsigned: Lower or Same
        'bls.w'   => IControl::BDC << 8 | ICondition::ULE_W,
        'bls.l'   => IControl::BDC << 8 | ICondition::ULE_L,
        'bls.q'   => IControl::BDC << 8 | ICondition::ULE_Q,

        'ble.b'   => IControl::BDC << 8 | ICondition::ILE_B, // signed: Less or Equal
        'ble.w'   => IControl::BDC << 8 | ICondition::ILE_W,
        'ble.l'   => IControl::BDC << 8 | ICondition::ILE_L,
        'ble.q'   => IControl::BDC << 8 | ICondition::ILE_Q,
        'fble.s'  => IControl::BDC << 8 | ICondition::FLE_S,
        'fble.d'  => IControl::BDC << 8 | ICondition::FLE_D,

        // Branch if <ea(s)> == <ea(d)>
        'beq.b'   => IControl::BDC << 8 | ICondition::IEQ_B,
        'beq.w'   => IControl::BDC << 8 | ICondition::IEQ_W,
        'beq.l'   => IControl::BDC << 8 | ICondition::IEQ_L,
        'beq.q'   => IControl::BDC << 8 | ICondition::IEQ_Q,
        'fbeq.s'  => IControl::BDC << 8 | ICondition::FEQ_S,
        'fbeq.d'  => IControl::BDC << 8 | ICondition::FEQ_D,

        // Branch if <ea(s)> >= <ea(d)>
        'bhs.b'   => IControl::BDC << 8 | ICondition::UGE_B, // unsigned: Higher or Same
        'bhs.w'   => IControl::BDC << 8 | ICondition::UGE_W,
        'bhs.l'   => IControl::BDC << 8 | ICondition::UGE_L,
        'bhs.q'   => IControl::BDC << 8 | ICondition::UGE_Q,

        'bge.b'   => IControl::BDC << 8 | ICondition::IGE_B, // signed: Greater or Equal
        'bge.w'   => IControl::BDC << 8 | ICondition::IGE_W,
        'bge.l'   => IControl::BDC << 8 | ICondition::IGE_L,
        'bge.q'   => IControl::BDC << 8 | ICondition::IGE_Q,
        'fbge.s'  => IControl::BDC << 8 | ICondition::FGE_S,
        'fbge.d'  => IControl::BDC << 8 | ICondition::FGE_D,

        // Branch if <ea(s)> > <ea(d)>
        'bhi.b'   => IControl::BDC << 8 | ICondition::UGT_B, // unsigned: Higher
        'bhi.w'   => IControl::BDC << 8 | ICondition::UGT_W,
        'bhi.l'   => IControl::BDC << 8 | ICondition::UGT_L,
        'bhi.q'   => IControl::BDC << 8 | ICondition::UGT_Q,

        'bgt.b'   => IControl::BDC << 8 | ICondition::IGT_B, // signed: Greater Than
        'bgt.w'   => IControl::BDC << 8 | ICondition::IGT_W,
        'bgt.l'   => IControl::BDC << 8 | ICondition::IGT_L,
        'bgt.q'   => IControl::BDC << 8 | ICondition::IGT_Q,
        'fbgt.s'  => IControl::BDC << 8 | ICondition::FGT_S,
        'fbgt.d'  => IControl::BDC << 8 | ICondition::FGT_D,

        // Branch if <ea(s)> != <ea(d)>
        'bne.b'   => IControl::BDC << 8 | ICondition::INE_B,
        'bne.w'   => IControl::BDC << 8 | ICondition::INE_W,
        'bne.l'   => IControl::BDC << 8 | ICondition::INE_L,
        'bne.q'   => IControl::BDC << 8 | ICondition::INE_Q,
        'fbne.s'  => IControl::BDC << 8 | ICondition::FNE_S,
        'fbne.d'  => IControl::BDC << 8 | ICondition::FNE_D,

        // Branch if bit set
        'bbs.b'   => IControl::BDC << 8 | ICondition::BPS_B,
        'bbs.w'   => IControl::BDC << 8 | ICondition::BPS_W,
        'bbs.l'   => IControl::BDC << 8 | ICondition::BPS_L,
        'bbs.q'   => IControl::BDC << 8 | ICondition::BPS_Q,

        // Branch if bit clear
        'bbc.b'   => IControl::BDC << 8 | ICondition::BPC_B,
        'bbc.w'   => IControl::BDC << 8 | ICondition::BPC_W,
        'bbc.l'   => IControl::BDC << 8 | ICondition::BPC_L,
        'bbc.q'   => IControl::BDC << 8 | ICondition::BPC_Q,


        // Go on, why not?
        'dbnz'    => IControl::DBNZ,

        // Data Movement
        'move.b'   => IDataMove::MOVE_B,
        'move.w'   => IDataMove::MOVE_W,
        'move.l'   => IDataMove::MOVE_L,
        'move.q'   => IDataMove::MOVE_Q,
        'savem'    => IDataMove::SAVEM,
        'loadm'    => IDataMove::LOADM,
        'fmoveb.s' => IDataMove::FMOVEB_S,
        'fmoveb.d' => IDataMove::FMOVEB_D,
        'fmovew.s' => IDataMove::FMOVEW_S,
        'fmovew.d' => IDataMove::FMOVEW_D,
        'fmovel.s' => IDataMove::FMOVEL_S,
        'fmovel.d' => IDataMove::FMOVEL_D,
        'fmoveq.s' => IDataMove::FMOVEQ_S,
        'fmoveq.d' => IDataMove::FMOVEQ_D,
        'fmoves.l' => IDataMove::FMOVES_L,
        'fmoves.q' => IDataMove::FMOVES_Q,
        'fmoves.d' => IDataMove::FMOVES_D,
        'fmoved.l' => IDataMove::FMOVED_L,
        'fmoved.q' => IDataMove::FMOVED_Q,
        'fmoved.s' => IDataMove::FMOVED_S,
        'fmove.s'  => IDataMove::FMOVE_S,
        'fmove.d'  => IDataMove::FMOVE_D,
        'clr.b'    => IDataMove::CLR_B,
        'clr.w'    => IDataMove::CLR_W,
        'clr.l'    => IDataMove::CLR_L,
        'clr.q'    => IDataMove::CLR_Q,
        'exg'      => IDataMove::R2R_EXG,
        'fexg'     => IDataMove::R2R_FEXG,
        'swap'     => IDataMove::R2R_SWAP,
        'swap.l'   => IDataMove::R2R_SWAP_L,
        'swap.q'   => IDataMove::R2R_SWAP_Q,
        'link'     => IDataMove::LINK,
        'unlk'     => IDataMove::UNLK,
        'lea'      => IDataMove::LEA,
        'pea'      => IDataMove::PEA,

        'finfo.s'  => IDataMove::FINFO_S,
        'finfo.d'  => IDataMove::FINFO_D,

        // Logical
        'and.b'    => ILogical::AND_B,
        'and.w'    => ILogical::AND_W,
        'and.l'    => ILogical::AND_L,
        'and.q'    => ILogical::AND_Q,

        'or.b'     => ILogical::OR_B,
        'or.w'     => ILogical::OR_W,
        'or.l'     => ILogical::OR_L,
        'or.q'     => ILogical::OR_Q,
        'eor.b'    => ILogical::EOR_B,
        'eor.w'    => ILogical::EOR_W,
        'eor.l'    => ILogical::EOR_L,
        'eor.q'    => ILogical::EOR_Q,
        'not.b'    => ILogical::NOT_B,
        'not.w'    => ILogical::NOT_W,
        'not.l'    => ILogical::NOT_L,
        'not.q'    => ILogical::NOT_Q,

        'lsl.b'    => ILogical::LSL_B,
        'lsl.w'    => ILogical::LSL_W,
        'lsl.l'    => ILogical::LSL_L,
        'lsl.q'    => ILogical::LSL_Q,
        'lsr.b'    => ILogical::LSR_B,
        'lsr.w'    => ILogical::LSR_W,
        'lsr.l'    => ILogical::LSR_L,
        'lsr.q'    => ILogical::LSR_Q,
        'rol.b'    => ILogical::ROL_B,
        'rol.w'    => ILogical::ROL_W,
        'rol.l'    => ILogical::ROL_L,
        'rol.q'    => ILogical::ROL_Q,
        'ror.b'    => ILogical::ROR_B,
        'ror.w'    => ILogical::ROR_W,
        'ror.l'    => ILogical::ROR_L,
        'ror.q'    => ILogical::ROR_Q,

        'bclr.b'   => ILogical::BCLR_B,
        'bclr.w'   => ILogical::BCLR_W,
        'bclr.l'   => ILogical::BCLR_L,
        'bclr.q'   => ILogical::BCLR_Q,
        'bset.b'   => ILogical::BSET_B,
        'bset.w'   => ILogical::BSET_W,
        'bset.l'   => ILogical::BSET_L,
        'bset.q'   => ILogical::BSET_Q,
        'bfffo'    => ILogical::BFFFO,
        'bfcnt'    => ILogical::BFCNT,

        // Arithmetic
        'extb.w'    => IArithmetic::EXTB_W,
        'extb.l'    => IArithmetic::EXTB_L,
        'extb.q'    => IArithmetic::EXTB_Q,
        'extw.l'    => IArithmetic::EXTW_L,
        'extw.q'    => IArithmetic::EXTW_Q,
        'extl.q'    => IArithmetic::EXTL_Q,
        'asl.b'     => IArithmetic::ASL_B,
        'asl.w'     => IArithmetic::ASL_W,
        'asl.l'     => IArithmetic::ASL_L,
        'asl.q'     => IArithmetic::ASL_Q,
        'asr.b'     => IArithmetic::ASR_B,
        'asr.w'     => IArithmetic::ASR_W,
        'asr.l'     => IArithmetic::ASR_L,
        'asr.q'     => IArithmetic::ASR_Q,
        'add.b'     => IArithmetic::ADD_B,
        'add.w'     => IArithmetic::ADD_W,
        'add.l'     => IArithmetic::ADD_L,
        'add.q'     => IArithmetic::ADD_Q,
        'fadd.s'    => IArithmetic::FADD_S,
        'fadd.d'    => IArithmetic::FADD_D,
        'sub.b'     => IArithmetic::SUB_B,
        'sub.w'     => IArithmetic::SUB_W,
        'sub.l'     => IArithmetic::SUB_L,
        'sub.q'     => IArithmetic::SUB_Q,
        'fsub.s'    => IArithmetic::FSUB_S,
        'fsub.d'    => IArithmetic::FSUB_D,
        'neg.b'     => IArithmetic::NEG_B,
        'neg.w'     => IArithmetic::NEG_W,
        'neg.l'     => IArithmetic::NEG_L,
        'neg.q'     => IArithmetic::NEG_Q,
        'fneg.s'    => IArithmetic::FNEG_S,
        'fneg.d'    => IArithmetic::FNEG_D,
        'muls.b'    => IArithmetic::MULS_B,
        'muls.w'    => IArithmetic::MULS_W,
        'muls.l'    => IArithmetic::MULS_L,
        'muls.q'    => IArithmetic::MULS_Q,
        'mulu.b'    => IArithmetic::MULU_B,
        'mulu.w'    => IArithmetic::MULU_W,
        'mulu.l'    => IArithmetic::MULU_L,
        'mulu.q'    => IArithmetic::MULU_Q,
        'fmul.s'    => IArithmetic::FMUL_S,
        'fmul.d'    => IArithmetic::FMUL_D,
        'divs.l'    => IArithmetic::DIVS_L,
        'divs.q'    => IArithmetic::DIVS_Q,
        'mods.l'    => IArithmetic::MODS_L,
        'mods.q'    => IArithmetic::MODS_Q,
        'divu.l'    => IArithmetic::DIVU_L,
        'divu.q'    => IArithmetic::DIVU_Q,
        'modu.l'    => IArithmetic::MODU_L,
        'modu.q'    => IArithmetic::MODU_Q,
        'fdiv.s'    => IArithmetic::FDIV_S,
        'fdiv.d'    => IArithmetic::FDIV_D,
        'fmod.s'    => IArithmetic::FMOD_S,
        'fmod.d'    => IArithmetic::FMOD_D,
        'fabs.s'    => IArithmetic::FABS_S,
        'fabs.d'    => IArithmetic::FABS_D,
        'fsqrt.s'   => IArithmetic::FSQRT_S,
        'fsqrt.d'   => IArithmetic::FSQRT_D,
        'facos.s'   => IArithmetic::FACOS_S,
        'facos.d'   => IArithmetic::FACOS_D,
        'fasin.s'   => IArithmetic::FASIN_S,
        'fasin.d'   => IArithmetic::FASIN_D,
        'fatan.s'   => IArithmetic::FATAN_S,
        'fatan.d'   => IArithmetic::FATAN_D,
        'fcos.s'    => IArithmetic::FCOS_S,
        'fcos.d'    => IArithmetic::FCOS_D,
        'fsin.s'    => IArithmetic::FSIN_S,
        'fsin.d'    => IArithmetic::FSIN_D,
        'fsincos.s' => IArithmetic::FSINCOS_S,
        'fsincos.d' => IArithmetic::FSINCOS_D,
        'ftan.s'    => IArithmetic::FTAN_S,
        'ftan.d'    => IArithmetic::FTAN_D,
        'fetox.s'   => IArithmetic::FETOX_S,
        'fetox.d'   => IArithmetic::FETOX_D,
        'flogn.s'   => IArithmetic::FLOGN_S,
        'flogn.d'   => IArithmetic::FLOGN_D,
        'flog2.s'   => IArithmetic::FLOG2_S,
        'flog2.d'   => IArithmetic::FLOG2_D,
        'ftwotox.s' => IArithmetic::FTWOTOX_S,
        'ftwotox.d' => IArithmetic::FTWOTOX_D,
/*
        'reserved0',
        'reserved1',
        'reserved2',
        'reserved3',
        'reserved4',
        'reserved5',
        'reserved6',
        'reserved7',
        'reserved8',
        'reserved9',
        'reserved10',
        'reserved11',
*/
    ];

    // Subopcode matches (2 bytes)
    const MATCHES_SUBOPCODE = [



    ];
}

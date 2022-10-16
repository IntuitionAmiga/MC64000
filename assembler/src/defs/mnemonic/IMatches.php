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
        'biz.b'   => IControl::BIZ_B,
        'biz.w'   => IControl::BIZ_W,
        'biz.l'   => IControl::BIZ_L,
        'biz.q'   => IControl::BIZ_Q,
        'fbiz.s'  => IControl::FBIZ_S,
        'fbiz.d'  => IControl::FBIZ_D,

        // Branch if <ea> != 0
        'bnz.b'   => IControl::BNZ_B,
        'bnz.w'   => IControl::BNZ_W,
        'bnz.l'   => IControl::BNZ_L,
        'bnz.q'   => IControl::BNZ_Q,
        'fbnz.s'  => IControl::FBNZ_S,
        'fbnz.d'  => IControl::FBNZ_D,

        // Branch if <ea> < 0
        'bmi.b'   => IControl::BMI_B,
        'bmi.w'   => IControl::BMI_W,
        'bmi.l'   => IControl::BMI_L,
        'bmi.q'   => IControl::BMI_Q,
        'fbmi.s'  => IControl::FBMI_S,
        'fbmi.d'  => IControl::FBMI_D,

        // Branch if <ea> > 0
        'bpl.b'   => IControl::BPL_B,
        'bpl.w'   => IControl::BPL_W,
        'bpl.l'   => IControl::BPL_L,
        'bpl.q'   => IControl::BPL_Q,
        'fbpl.s'  => IControl::FBPL_S,
        'fbpl.d'  => IControl::FBPL_D,

        // Branch if <ea(s)> < <ea(d)>
        'blo.b'   => IControl::BLO_B, // unsigned: Lower
        'blo.w'   => IControl::BLO_W,
        'blo.l'   => IControl::BLO_L,
        'blo.q'   => IControl::BLO_Q,

        'blt.b'   => IControl::BLT_B, // signed: Less Than
        'blt.w'   => IControl::BLT_W,
        'blt.l'   => IControl::BLT_L,
        'blt.q'   => IControl::BLT_Q,
        'fblt.s'  => IControl::FBLT_S,
        'fblt.d'  => IControl::FBLT_D,

        // Branch if <ea(s)> <= <ea(d)>
        'bls.b'   => IControl::BLS_B, // unsigned: Lower or Same
        'bls.w'   => IControl::BLS_W,
        'bls.l'   => IControl::BLS_L,
        'bls.q'   => IControl::BLS_Q,

        'ble.b'   => IControl::BLE_B, // signed: Less or Equal
        'ble.w'   => IControl::BLE_W,
        'ble.l'   => IControl::BLE_L,
        'ble.q'   => IControl::BLE_Q,
        'fble.s'  => IControl::FBLE_S,
        'fble.d'  => IControl::FBLE_D,

        // Branch if <ea(s)> == <ea(d)>
        'beq.b'   => IControl::BEQ_B,
        'beq.w'   => IControl::BEQ_W,
        'beq.l'   => IControl::BEQ_L,
        'beq.q'   => IControl::BEQ_Q,
        'fbeq.s'  => IControl::FBEQ_S,
        'fbeq.d'  => IControl::FBEQ_D,

        // Branch if <ea(s)> >= <ea(d)>
        'bhs.b'   => IControl::BHS_B, // unsigned: Higher or Same
        'bhs.w'   => IControl::BHS_W,
        'bhs.l'   => IControl::BHS_L,
        'bhs.q'   => IControl::BHS_Q,

        'bge.b'   => IControl::BGE_B, // signed: Greater or Equal
        'bge.w'   => IControl::BGE_W,
        'bge.l'   => IControl::BGE_L,
        'bge.q'   => IControl::BGE_Q,
        'fbge.s'  => IControl::FBGE_S,
        'fbge.d'  => IControl::FBGE_D,

        // Branch if <ea(s)> > <ea(d)>
        'bhi.b'   => IControl::BHI_B, // unsigned: Higher
        'bhi.w'   => IControl::BHI_W,
        'bhi.l'   => IControl::BHI_L,
        'bhi.q'   => IControl::BHI_Q,

        'bgt.b'   => IControl::BGT_B , // signed: Greater Than
        'bgt.w'   => IControl::BGT_W ,
        'bgt.l'   => IControl::BGT_L ,
        'bgt.q'   => IControl::BGT_Q ,
        'fbgt.s'  => IControl::FBGT_S,
        'fbgt.d'  => IControl::FBGT_D,

        // Branch if <ea(s)> != <ea(d)>
        'bne.b'   => IControl::BNE_B ,
        'bne.w'   => IControl::BNE_W ,
        'bne.l'   => IControl::BNE_L ,
        'bne.q'   => IControl::BNE_Q ,
        'fbne.s'  => IControl::FBNE_S,
        'fbne.d'  => IControl::FBNE_D,

        // Branch if bit set
        'bbs.b'   => IControl::BBS_B,
        'bbs.w'   => IControl::BBS_W,
        'bbs.l'   => IControl::BBS_L,
        'bbs.q'   => IControl::BBS_Q,

        // Branch if bit clear
        'bbc.b'   => IControl::BBC_B,
        'bbc.w'   => IControl::BBC_W,
        'bbc.l'   => IControl::BBC_L,
        'bbc.q'   => IControl::BBC_Q,

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

        // Set if <ea> == 0
        'siz.b'   => IDataMove::SIZ_B,
        'siz.w'   => IDataMove::SIZ_W,
        'siz.l'   => IDataMove::SIZ_L,
        'siz.q'   => IDataMove::SIZ_Q,
        'fsiz.s'  => IDataMove::FSIZ_S,
        'fsiz.d'  => IDataMove::FSIZ_D,

        // Set if <ea> != 0
        'snz.b'   => IDataMove::SNZ_B,
        'snz.w'   => IDataMove::SNZ_W,
        'snz.l'   => IDataMove::SNZ_L,
        'snz.q'   => IDataMove::SNZ_Q,
        'fsnz.s'  => IDataMove::FSNZ_S,
        'fsnz.d'  => IDataMove::FSNZ_D,

        // Set if <ea> < 0
        'smi.b'   => IDataMove::SMI_B,
        'smi.w'   => IDataMove::SMI_W,
        'smi.l'   => IDataMove::SMI_L,
        'smi.q'   => IDataMove::SMI_Q,
        'fsmi.s'  => IDataMove::FSMI_S,
        'fsmi.d'  => IDataMove::FSMI_D,

        // Set if <ea> > 0
        'spl.b'   => IDataMove::SPL_B,
        'spl.w'   => IDataMove::SPL_W,
        'spl.l'   => IDataMove::SPL_L,
        'spl.q'   => IDataMove::SPL_Q,
        'fspl.s'  => IDataMove::FSPL_S,
        'fspl.d'  => IDataMove::FSPL_D,

        // Set if <ea(s)> < <ea(d)>
        'slo.b'   => IDataMove::SLO_B, // unsigned: Lower
        'slo.w'   => IDataMove::SLO_W,
        'slo.l'   => IDataMove::SLO_L,
        'slo.q'   => IDataMove::SLO_Q,

        'slt.b'   => IDataMove::SLT_B, // signed: Less Than
        'slt.w'   => IDataMove::SLT_W,
        'slt.l'   => IDataMove::SLT_L,
        'slt.q'   => IDataMove::SLT_Q,
        'fslt.s'  => IDataMove::FSLT_S,
        'fslt.d'  => IDataMove::FSLT_D,

        // Set if <ea(s)> <= <ea(d)>
        'sls.b'   => IDataMove::SLS_B, // unsigned: Lower or Same
        'sls.w'   => IDataMove::SLS_W,
        'sls.l'   => IDataMove::SLS_L,
        'sls.q'   => IDataMove::SLS_Q,

        'sle.b'   => IDataMove::SLE_B, // signed: Less or Equal
        'sle.w'   => IDataMove::SLE_W,
        'sle.l'   => IDataMove::SLE_L,
        'sle.q'   => IDataMove::SLE_Q,
        'fsle.s'  => IDataMove::FSLE_S,
        'fsle.d'  => IDataMove::FSLE_D,

        // Set if <ea(s)> == <ea(d)>
        'seq.b'   => IDataMove::SEQ_B,
        'seq.w'   => IDataMove::SEQ_W,
        'seq.l'   => IDataMove::SEQ_L,
        'seq.q'   => IDataMove::SEQ_Q,
        'fseq.s'  => IDataMove::FSEQ_S,
        'fseq.d'  => IDataMove::FSEQ_D,

        // Set if <ea(s)> >= <ea(d)>
        'shs.b'   => IDataMove::SHS_B, // unsigned: Higher or Same
        'shs.w'   => IDataMove::SHS_W,
        'shs.l'   => IDataMove::SHS_L,
        'shs.q'   => IDataMove::SHS_Q,

        'sge.b'   => IDataMove::SGE_B, // signed: Greater or Equal
        'sge.w'   => IDataMove::SGE_W,
        'sge.l'   => IDataMove::SGE_L,
        'sge.q'   => IDataMove::SGE_Q,
        'fsge.s'  => IDataMove::FSGE_S,
        'fsge.d'  => IDataMove::FSGE_D,

        // Set if <ea(s)> > <ea(d)>
        'shi.b'   => IDataMove::SHI_B, // unsigned: Higher
        'shi.w'   => IDataMove::SHI_W,
        'shi.l'   => IDataMove::SHI_L,
        'shi.q'   => IDataMove::SHI_Q,

        'sgt.b'   => IDataMove::SGT_B , // signed: Greater Than
        'sgt.w'   => IDataMove::SGT_W ,
        'sgt.l'   => IDataMove::SGT_L ,
        'sgt.q'   => IDataMove::SGT_Q ,
        'fsgt.s'  => IDataMove::FSGT_S,
        'fsgt.d'  => IDataMove::FSGT_D,

        // Set if <ea(s)> != <ea(d)>
        'sne.b'   => IDataMove::SNE_B ,
        'sne.w'   => IDataMove::SNE_W ,
        'sne.l'   => IDataMove::SNE_L ,
        'sne.q'   => IDataMove::SNE_Q ,
        'fsne.s'  => IDataMove::FSNE_S,
        'fsne.d'  => IDataMove::FSNE_D,

        // Set if bit set
        'sbs.b'   => IDataMove::SBS_B,
        'sbs.w'   => IDataMove::SBS_W,
        'sbs.l'   => IDataMove::SBS_L,
        'sbs.q'   => IDataMove::SBS_Q,

        // Set if bit clear
        'sbc.b'   => IDataMove::SBC_B,
        'sbc.w'   => IDataMove::SBC_W,
        'sbc.l'   => IDataMove::SBC_L,
        'sbc.q'   => IDataMove::SBC_Q,

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
        'fmacc.s'   => IArithmetic::R2R_FMACC_S,
        'fmacc.d'   => IArithmetic::R2R_FMACC_D,
        'fmadd.s'   => IArithmetic::R2R_FMADD_S,
        'fmadd.d'   => IArithmetic::R2R_FMADD_D,
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
    ];

}

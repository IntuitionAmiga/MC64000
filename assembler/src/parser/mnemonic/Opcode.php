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

namespace ABadCafe\MC64K\Parser\Mnemonic;

use ABadCafe\MC64K\Defs\Mnemonic;

/**
 * INames
 */
class Opcode {
    const MATCHES = [

        // Flow control group
        'hcf'     => Mnemonic\IControl::HCF,
        'bra.b'   => Mnemonic\IControl::BRA_B,
        'bra'     => Mnemonic\IControl::BRA,
        'bsr.b'   => Mnemonic\IControl::BSR_B,
        'bsr'     => Mnemonic\IControl::BSR,
        'jmp'     => Mnemonic\IControl::JMP,
        'jsr'     => Mnemonic\IControl::JSR,
        'rts'     => Mnemonic\IControl::RTS,

        // Single operand conditional branches

        // Branch if <ea> == 0
        'biz.b'   => Mnemonic\IControl::BIZ_B,
        'biz.w'   => Mnemonic\IControl::BIZ_W,
        'biz.l'   => Mnemonic\IControl::BIZ_L,
        'biz.q'   => Mnemonic\IControl::BIZ_Q,
        'fbiz.s'  => Mnemonic\IControl::FBIZ_S,
        'fbiz.d'  => Mnemonic\IControl::FBIZ_D,

        // Branch if <ea> != 0
        'bnz.b'   => Mnemonic\IControl::BNZ_B,
        'bnz.w'   => Mnemonic\IControl::BNZ_W,
        'bnz.l'   => Mnemonic\IControl::BNZ_L,
        'bnz.q'   => Mnemonic\IControl::BNZ_Q,
        'fbnz.s'  => Mnemonic\IControl::FBNZ_S,
        'fbnz.d'  => Mnemonic\IControl::FBNZ_D,

        // Branch if <ea> < 0
        'bmi.b'   => Mnemonic\IControl::BMI_B,
        'bmi.w'   => Mnemonic\IControl::BMI_W,
        'bmi.l'   => Mnemonic\IControl::BMI_L,
        'bmi.q'   => Mnemonic\IControl::BMI_Q,
        'fbmi.s'  => Mnemonic\IControl::FBMI_S,
        'fbmi.d'  => Mnemonic\IControl::FBMI_D,

        // Branch if <ea> > 0
        'bpl.b'   => Mnemonic\IControl::BPL_B,
        'bpl.w'   => Mnemonic\IControl::BPL_W,
        'bpl.l'   => Mnemonic\IControl::BPL_L,
        'bpl.q'   => Mnemonic\IControl::BPL_Q,
        'fbpl.s'  => Mnemonic\IControl::FBPL_S,
        'fbpl.d'  => Mnemonic\IControl::FBPL_D,

        // Branch if <ea(s)> < <ea(d)>
        'blt.b'   => Mnemonic\IControl::BLT_B,
        'blt.w'   => Mnemonic\IControl::BLT_W,
        'blt.l'   => Mnemonic\IControl::BLT_L,
        'blt.q'   => Mnemonic\IControl::BLT_Q,
        'fblt.s'  => Mnemonic\IControl::FBLT_S,
        'fblt.d'  => Mnemonic\IControl::FBLT_D,

        // Branch if <ea(s)> <= <ea(d)>
        'ble.b'   => Mnemonic\IControl::BLE_B,
        'ble.w'   => Mnemonic\IControl::BLE_W,
        'ble.l'   => Mnemonic\IControl::BLE_L,
        'ble.q'   => Mnemonic\IControl::BLE_Q,
        'fble.s'  => Mnemonic\IControl::FBLE_S,
        'fble.d'  => Mnemonic\IControl::FBLE_D,

        // Branch if <ea(s)> == <ea(d)>
        'beq.b'   => Mnemonic\IControl::BEQ_B,
        'beq.w'   => Mnemonic\IControl::BEQ_W,
        'beq.l'   => Mnemonic\IControl::BEQ_L,
        'beq.q'   => Mnemonic\IControl::BEQ_Q,
        'fbeq.s'  => Mnemonic\IControl::FBEQ_S,
        'fbeq.d'  => Mnemonic\IControl::FBEQ_D,

        // Branch if <ea(s)> >= <ea(d)>
        'bge.b'   => Mnemonic\IControl::BGE_B,
        'bge.w'   => Mnemonic\IControl::BGE_W,
        'bge.l'   => Mnemonic\IControl::BGE_L,
        'bge.q'   => Mnemonic\IControl::BGE_Q,
        'fbge.s'  => Mnemonic\IControl::FBGE_S,
        'fbge.d'  => Mnemonic\IControl::FBGE_D,

        // Branch if <ea(s)> > <ea(d)>
        'bgt.b'   => Mnemonic\IControl::BGT_B,
        'bgt.w'   => Mnemonic\IControl::BGT_W,
        'bgt.l'   => Mnemonic\IControl::BGT_L,
        'bgt.q'   => Mnemonic\IControl::BGT_Q,
        'fbgt.s'  => Mnemonic\IControl::FBGT_S,
        'fbgt.d'  => Mnemonic\IControl::FBGT_D,

        // Branch if <ea(s)> != <ea(d)>
        'bne.b'   => Mnemonic\IControl::BNE_B,
        'bne.w'   => Mnemonic\IControl::BNE_W,
        'bne.l'   => Mnemonic\IControl::BNE_L,
        'bne.q'   => Mnemonic\IControl::BNE_Q,
        'fbne.s'  => Mnemonic\IControl::FBNE_S,
        'fbne.d'  => Mnemonic\IControl::FBNE_D,

        // Branch if bit set
        'bbs.b'   => Mnemonic\IControl::BBS_B,
        'bbs.w'   => Mnemonic\IControl::BBS_W,
        'bbs.l'   => Mnemonic\IControl::BBS_L,
        'bbs.q'   => Mnemonic\IControl::BBS_Q,

        // Branch if bit clear
        'bbc.b'   => Mnemonic\IControl::BBC_B,
        'bbc.w'   => Mnemonic\IControl::BBC_W,
        'bbc.l'   => Mnemonic\IControl::BBC_L,
        'bbc.q'   => Mnemonic\IControl::BBC_Q,

        // Go on, why not?
        'dbnz'    => Mnemonic\IControl::DBNZ,

        // Data Movement
        'move.b'   => Mnemonic\IDataMove::MOVE_B,
        'move.w'   => Mnemonic\IDataMove::MOVE_W,
        'move.l'   => Mnemonic\IDataMove::MOVE_L,
        'move.q'   => Mnemonic\IDataMove::MOVE_Q,
        'savem'    => Mnemonic\IDataMove::SAVEM,
        'loadm'    => Mnemonic\IDataMove::LOADM,
        'fmoveb.s' => Mnemonic\IDataMove::FMOVEB_S,
        'fmoveb.d' => Mnemonic\IDataMove::FMOVEB_D,
        'fmovew.s' => Mnemonic\IDataMove::FMOVEW_S,
        'fmovew.d' => Mnemonic\IDataMove::FMOVEW_D,
        'fmovel.s' => Mnemonic\IDataMove::FMOVEL_S,
        'fmovel.d' => Mnemonic\IDataMove::FMOVEL_D,
        'fmoveq.s' => Mnemonic\IDataMove::FMOVEQ_S,
        'fmoveq.d' => Mnemonic\IDataMove::FMOVEQ_D,
        'fmoves.l' => Mnemonic\IDataMove::FMOVES_L,
        'fmoves.q' => Mnemonic\IDataMove::FMOVES_Q,
        'fmoves.d' => Mnemonic\IDataMove::FMOVES_D,
        'fmoved.l' => Mnemonic\IDataMove::FMOVED_L,
        'fmoved.q' => Mnemonic\IDataMove::FMOVED_Q,
        'fmoved.s' => Mnemonic\IDataMove::FMOVED_S,
        'fsavem'   => Mnemonic\IDataMove::FSAVEM,
        'floadm'   => Mnemonic\IDataMove::FLOADM,
        'clr.b'    => Mnemonic\IDataMove::CLR_B,
        'clr.w'    => Mnemonic\IDataMove::CLR_W,
        'clr.l'    => Mnemonic\IDataMove::CLR_L,
        'clr.q'    => Mnemonic\IDataMove::CLR_Q,
        'exg'      => Mnemonic\IDataMove::EXG,
        'fexg'     => Mnemonic\IDataMove::FEXG,
        'swap'     => Mnemonic\IDataMove::SWAP,
        'swap.l'   => Mnemonic\IDataMove::SWAP_L,
        'swap.q'   => Mnemonic\IDataMove::SWAP_Q,
        'link'     => Mnemonic\IDataMove::LINK,
        'unlk'     => Mnemonic\IDataMove::UNLK,
        'lea'      => Mnemonic\IDataMove::LEA,
        'pea'      => Mnemonic\IDataMove::PEA,

// Logical
        'and.b'    => Mnemonic\ILogical::AND_B,
        'and.w'    => Mnemonic\ILogical::AND_W,
        'and.l'    => Mnemonic\ILogical::AND_L,
        'and.q'    => Mnemonic\ILogical::AND_Q,

        'or.b'     => Mnemonic\ILogical::OR_B,
        'or.w'     => Mnemonic\ILogical::OR_W,
        'or.l'     => Mnemonic\ILogical::OR_L,
        'or.q'     => Mnemonic\ILogical::OR_Q,
        'eor.b'    => Mnemonic\ILogical::EOR_B,
        'eor.w'    => Mnemonic\ILogical::EOR_W,
        'eor.l'    => Mnemonic\ILogical::EOR_L,
        'eor.q'    => Mnemonic\ILogical::EOR_Q,
        'not.b'    => Mnemonic\ILogical::NOT_B,
        'not.w'    => Mnemonic\ILogical::NOT_W,
        'not.l'    => Mnemonic\ILogical::NOT_L,
        'not.q'    => Mnemonic\ILogical::NOT_Q,

        'lsl.b'    => Mnemonic\ILogical::LSL_B,
        'lsl.w'    => Mnemonic\ILogical::LSL_W,
        'lsl.l'    => Mnemonic\ILogical::LSL_L,
        'lsl.q'    => Mnemonic\ILogical::LSL_Q,
        'lsr.b'    => Mnemonic\ILogical::LSR_B,
        'lsr.w'    => Mnemonic\ILogical::LSR_W,
        'lsr.l'    => Mnemonic\ILogical::LSR_L,
        'lsr.q'    => Mnemonic\ILogical::LSR_Q,
        'rol.b'    => Mnemonic\ILogical::ROL_B,
        'rol.w'    => Mnemonic\ILogical::ROL_W,
        'rol.l'    => Mnemonic\ILogical::ROL_L,
        'rol.q'    => Mnemonic\ILogical::ROL_Q,
        'ror.b'    => Mnemonic\ILogical::ROR_B,
        'ror.w'    => Mnemonic\ILogical::ROR_W,
        'ror.l'    => Mnemonic\ILogical::ROR_L,
        'ror.q'    => Mnemonic\ILogical::ROR_Q,

        'bclr.b'   => Mnemonic\ILogical::BCLR_B,
        'bclr.w'   => Mnemonic\ILogical::BCLR_W,
        'bclr.l'   => Mnemonic\ILogical::BCLR_L,
        'bclr.q'   => Mnemonic\ILogical::BCLR_Q,
        'bset.b'   => Mnemonic\ILogical::BSET_B,
        'bset.w'   => Mnemonic\ILogical::BSET_W,
        'bset.l'   => Mnemonic\ILogical::BSET_L,
        'bset.q'   => Mnemonic\ILogical::BSET_Q,
        'bfclr'    => Mnemonic\ILogical::BFCLR,
        'bfset'    => Mnemonic\ILogical::BFSET,
        'bfins'    => Mnemonic\ILogical::BFINS,
        'bfexts'   => Mnemonic\ILogical::BFEXTS,
        'bfextu'   => Mnemonic\ILogical::BFEXTU,
        'bfffo'    => Mnemonic\ILogical::BFFFO,
        'bfcnt'    => Mnemonic\ILogical::BFCNT,
        'bfxxx'    => Mnemonic\ILogical::BFXXX,

// Arithmetic
        'extb.w'    => Mnemonic\IArithmetic::EXTB_W,
        'extb.l'    => Mnemonic\IArithmetic::EXTB_L,
        'extb.q'    => Mnemonic\IArithmetic::EXTB_Q,
        'extw.l'    => Mnemonic\IArithmetic::EXTW_L,
        'extw.q'    => Mnemonic\IArithmetic::EXTW_Q,
        'extl.q'    => Mnemonic\IArithmetic::EXTL_Q,
        'asl.b'     => Mnemonic\IArithmetic::ASL_B,
        'asl.w'     => Mnemonic\IArithmetic::ASL_W,
        'asl.l'     => Mnemonic\IArithmetic::ASL_L,
        'asl.q'     => Mnemonic\IArithmetic::ASL_Q,
        'asr.b'     => Mnemonic\IArithmetic::ASR_B,
        'asr.w'     => Mnemonic\IArithmetic::ASR_W,
        'asr.l'     => Mnemonic\IArithmetic::ASR_L,
        'asr.q'     => Mnemonic\IArithmetic::ASR_Q,
        'add.b'     => Mnemonic\IArithmetic::ADD_B,
        'add.w'     => Mnemonic\IArithmetic::ADD_W,
        'add.l'     => Mnemonic\IArithmetic::ADD_L,
        'add.q'     => Mnemonic\IArithmetic::ADD_Q,
        'fadd.s'    => Mnemonic\IArithmetic::FADD_S,
        'fadd.d'    => Mnemonic\IArithmetic::FADD_D,
        'sub.b'     => Mnemonic\IArithmetic::SUB_B,
        'sub.w'     => Mnemonic\IArithmetic::SUB_W,
        'sub.l'     => Mnemonic\IArithmetic::SUB_L,
        'sub.q'     => Mnemonic\IArithmetic::SUB_Q,
        'fsub.s'    => Mnemonic\IArithmetic::FSUB_S,
        'fsub.d'    => Mnemonic\IArithmetic::FSUB_D,
        'neg.b'     => Mnemonic\IArithmetic::NEG_B,
        'neg.w'     => Mnemonic\IArithmetic::NEG_W,
        'neg.l'     => Mnemonic\IArithmetic::NEG_L,
        'neg.q'     => Mnemonic\IArithmetic::NEG_Q,
        'fneg.s'    => Mnemonic\IArithmetic::FNEG_S,
        'fneg.d'    => Mnemonic\IArithmetic::FNEG_D,
        'muls.b'    => Mnemonic\IArithmetic::MULS_B,
        'muls.w'    => Mnemonic\IArithmetic::MULS_W,
        'muls.l'    => Mnemonic\IArithmetic::MULS_L,
        'muls.q'    => Mnemonic\IArithmetic::MULS_Q,
        'mulu.b'    => Mnemonic\IArithmetic::MULU_B,
        'mulu.w'    => Mnemonic\IArithmetic::MULU_W,
        'mulu.l'    => Mnemonic\IArithmetic::MULU_L,
        'mulu.q'    => Mnemonic\IArithmetic::MULU_Q,
        'fmul.s'    => Mnemonic\IArithmetic::FMUL_S,
        'fmul.d'    => Mnemonic\IArithmetic::FMUL_D,
        'divs.b'    => Mnemonic\IArithmetic::DIVS_B,
        'divs.w'    => Mnemonic\IArithmetic::DIVS_W,
        'divs.l'    => Mnemonic\IArithmetic::DIVS_L,
        'divs.q'    => Mnemonic\IArithmetic::DIVS_Q,
        'divu.b'    => Mnemonic\IArithmetic::DIVU_B,
        'divu.w'    => Mnemonic\IArithmetic::DIVU_W,
        'divu.l'    => Mnemonic\IArithmetic::DIVU_L,
        'divu.q'    => Mnemonic\IArithmetic::DIVU_Q,
        'fdiv.s'    => Mnemonic\IArithmetic::FDIV_S,
        'fdiv.d'    => Mnemonic\IArithmetic::FDIV_D,
        'fmod.s'    => Mnemonic\IArithmetic::FMOD_S,
        'fmod.d'    => Mnemonic\IArithmetic::FMOD_D,
        'fabs.s'    => Mnemonic\IArithmetic::FABS_S,
        'fabs.d'    => Mnemonic\IArithmetic::FABS_D,
        'fsqrt.s'   => Mnemonic\IArithmetic::FSQRT_S,
        'frqrt.d'   => Mnemonic\IArithmetic::FSQRT_D,
        'facos.s'   => Mnemonic\IArithmetic::FACOS_S,
        'facos.d'   => Mnemonic\IArithmetic::FACOS_D,
        'fasin.s'   => Mnemonic\IArithmetic::FASIN_S,
        'fasin.d'   => Mnemonic\IArithmetic::FASIN_D,
        'fatan.s'   => Mnemonic\IArithmetic::FATAN_S,
        'fatan.d'   => Mnemonic\IArithmetic::FATAN_D,
        'fcos.s'    => Mnemonic\IArithmetic::FCOS_S,
        'fcos.d'    => Mnemonic\IArithmetic::FCOS_D,
        'fsin.s'    => Mnemonic\IArithmetic::FSIN_S,
        'fsin.d'    => Mnemonic\IArithmetic::FSIN_D,
        'fsincos.s' => Mnemonic\IArithmetic::FSINCOS_S,
        'fsincos.d' => Mnemonic\IArithmetic::FSINCOS_D,
        'ftan.s'    => Mnemonic\IArithmetic::FTAN_S,
        'ftan.d'    => Mnemonic\IArithmetic::FTAN_D,
        'fetox.s'   => Mnemonic\IArithmetic::FETOX_S,
        'fetox.d'   => Mnemonic\IArithmetic::FETOX_D,
        'flogn.s'   => Mnemonic\IArithmetic::FLOGN_S,
        'flogn.d'   => Mnemonic\IArithmetic::FLOGN_D,
        'flog2.s'   => Mnemonic\IArithmetic::FLOG2_S,
        'flog2.d'   => Mnemonic\IArithmetic::FLOG2_D,
        'ftwotox.s' => Mnemonic\IArithmetic::FTWOTOX_S,
        'ftwotox.d' => Mnemonic\IArithmetic::FTWOTOX_D,
        'fgetexp.s' => Mnemonic\IArithmetic::FGETEXP_S,
        'fgetexp.d' => Mnemonic\IArithmetic::FGETEXP_D,
        'fgetman.s' => Mnemonic\IArithmetic::FGETMAN_S,
        'fgetman.d' => Mnemonic\IArithmetic::FGETMAN_D,
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
}

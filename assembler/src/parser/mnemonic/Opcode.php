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
/*
        'or.b',
        'or.w',
        'or.l',
        'or.q',
        'eor.b',
        'eor.w',
        'eor.l',
        'eor.q',
        'not.b',
        'not.w',
        'not.l',
        'not.q',

        'lsl.b',
        'lsl.w',
        'lsl.l',
        'lsl.q',
        'lsr.b',
        'lsr.w',
        'lsr.l',
        'lsr.q',
        'rol.b',
        'rol.w',
        'rol.l',
        'rol.q',
        'ror.b',
        'ror.w',
        'ror.l',
        'ror.q',

        'bclr.b',
        'bclr.w',
        'bclr.l',
        'bclr.q',
        'bset.b',
        'bset.w',
        'bset.l',
        'bset.q',
        'bfclr',
        'bfset',
        'bfins',
        'bfexts',
        'bfextu',
        'bfffo',
        'bfcnt',
        'bfxxx',

// Arithmetic
        'extb.w',
        'extb.l',
        'extb.q',
        'extw.l',
        'extw.q',
        'extl.q',
        'asl.b',
        'asl.w',
        'asl.l',
        'asl.q',
        'asr.b',
        'asr.w',
        'asr.l',
        'asr.q',
        'add.b',
        'add.w',
        'add.l',
        'add.q',
        'fadd.s',
        'fadd.d',
        'sub.b',
        'sub.w',
        'sub.l',
        'sub.q',
        'fsub.s',
        'fsub.d',
        'neg.b',
        'neg.w',
        'neg.l',
        'neg.q',
        'fneg.s',
        'fneg.d',
        'muls.b',
        'muls.w',
        'muls.l',
        'muls.q',
        'mulu.b',
        'mulu.w',
        'mulu.l',
        'mulu.q',
        'fmul.s',
        'fmul.d',
        'divs.b',
        'divs.w',
        'divs.l',
        'divs.q',
        'divu.b',
        'divu.w',
        'divu.l',
        'divu.q',
        'fdiv.s',
        'fdiv.d',
        'fmod.s',
        'fmod.d',
        'fabs.s',
        'fabs.d',
        'fsqrt.s',
        'frqrt.d',
        'facos.s',
        'facos.d',
        'fasin.s',
        'fasin.d',
        'fatan.s',
        'fatan.d',
        'fcos.s',
        'fcos.d',
        'fsin.s',
        'fsin.d',
        'fsincos.s',
        'fsincos.d',
        'ftan.s',
        'ftan.d',
        'fetox.s',
        'fetox.d',
        'flogn.s',
        'flogn.d',
        'flog2.s',
        'flog2.d',
        'ftwotox.s',
        'ftwotox.d',
        'fgetexp.s',
        'fgetexp.d',
        'fgetman.s',
        'fgetman.d',

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

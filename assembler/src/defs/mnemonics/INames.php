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

namespace ABadCafe\MC64K\Defs\Mnemonic;

/**
 * INames
 */
interface INames {
    const MATCHES = [

// Flow control group
        'bkpt',

        'bra.b',
        'bra',
        'bsr.b',
        'bsr',
        'jmp',
        'jsr',
        'rts',

        // Single operand conditional branches

        // Branch if <ea> == 0
        'biz.b',
        'biz.w',
        'biz.l',
        'biz.q',
        'fbiz.s',
        'fbiz.d',

        // Branch if <ea> != 0
        'bnz.b',
        'bnz.w',
        'bnz.l',
        'bnz.q',
        'fbnz.s',
        'fbnz.d',

        // Branch if <ea> < 0
        'bmi.b',
        'bmi.w',
        'bmi.l',
        'bmi.q',
        'fbmi.s',
        'fbmi.d',

        // Branch if <ea> > 0
        'bpl.b',
        'bpl.w',
        'bpl.l',
        'bpl.q',
        'fbpl.s',
        'fbpl.d',

        // Branch if <ea(s)> < <ea(d)>
        'blt.b',
        'blt.w',
        'blt.l',
        'blt.q',
        'fblt.s',
        'fblt.d',

        // Branch if <ea(s)> <= <ea(d)>
        'ble.b',
        'ble.w',
        'ble.l',
        'ble.q',
        'fble.s',
        'fble.d',

        // Branch if <ea(s)> == <ea(d)>
        'beq.b',
        'beq.w',
        'beq.l',
        'beq.q',
        'fbeq.s',
        'fbeq.d',

        // Branch if <ea(s)> >= <ea(d)>
        'bge.b',
        'bge.w',
        'bge.l',
        'bge.q',
        'fbge.s',
        'fbge.d',

        // Branch if <ea(s)> > <ea(d)>
        'bgt.b',
        'bgt.w',
        'bgt.l',
        'bgt.q',
        'fbgt.s',
        'fbgt.d',

        // Branch if <ea(s)> != <ea(d)>
        'bne.b',
        'bne.w',
        'bne.l',
        'bne.q',
        'fbne.s',
        'fbne.d',

        // Branch if bit set
        'bbs.b',
        'bbs.w',
        'bbs.l',
        'bbs.q',

        // Branch if bit clear
        'bbc.b',
        'bbc.w',
        'bbc.l',
        'bbc.q',

        // Go on, why not?
        'dbnz',

// Data Movement
        'move.b',
        'move.w',
        'move.l',
        'move.q',
        'savem',
        'loadm',
        'fmoveb.s',
        'fmoveb.d',
        'fmovew.s',
        'fmovew.d',
        'fmovel.s',
        'fmovel.d',
        'fmoveq.s',
        'fmoveq.d',
        'fmoves.l',
        'fmoves.q',
        'fmoves.d',
        'fmoved.l',
        'fmoved.q',
        'fmoved.s',
        'fsavem',
        'floadm',
        'clr.b',
        'clr.w',
        'clr.l',
        'clr.q',
        'exg',
        'fexg',
        'swap',
        'link',
        'unlk',

        'add.b',
        'add.w',
        'add.l',
        'add.q',
        'fadd.s',
        'fadd.d',

        'and.b',
        'and.w',
        'and.l',
        'and.q',

        'asl.b',
        'asl.w',
        'asl.l',
        'asl.q',

        'asr.b',
        'asr.w',
        'asr.l',
        'asr.q',

        'bclr.b',
        'bclr.w',
        'bclr.l',
        'bclr.q',

        'bset.b',
        'bset.w',
        'bset.l',
        'bset.q',

        'bfclr',
        'bfexts',
        'bfextu',
        'bfffo',
        'bfins',
        'bfset',

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

        'eor.b',
        'eor.w',
        'eor.l',
        'eor.q',

        'extb.w',
        'extb.l',
        'extb.q',
        'extw.l',
        'extw.q',
        'extl.q',
        'fabs.s',
        'fabs.d',

        'lea',

        'lsl.b',
        'lsl.w',
        'lsl.l',
        'lsl.q',
        'lsr.b',
        'lsr.w',
        'lsr.l',
        'lsr.q',



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

        'neg.b',
        'neg.w',
        'neg.l',
        'neg.q',
        'fneg.s',
        'fneg.d',

        'fsqrt.s',
        'frqrt.d',

        'not.b',
        'not.w',
        'not.l',
        'not.q',

        'or.b',
        'or.w',
        'or.l',
        'or.q',

        'pea',

        'rol.b',
        'rol.w',
        'rol.l',
        'rol.q',

        'ror.b',
        'ror.w',
        'ror.l',
        'ror.q',



        'sub.b',
        'sub.w',
        'sub.l',
        'sub.q',
        'fsub.s',
        'fsub.d',




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
    ];
}

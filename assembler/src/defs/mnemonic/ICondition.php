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
 * ICondition
 *
 * Enumerates the condition sub-opcode. This is a parameter for instuctions that operate on a condition
 * being true.
 *
 */
interface ICondition {

    const
        // Integer Equal
        // Dyadic e.g. beq
        // Monadic e.g. biz
        IEQ_B = 0,
        IEQ_W = 1,
        IEQ_L = 2,
        IEQ_Q = 3,

        // Float Equal
        // Dyadic e.g. fbeq
        // Monadic e.g. fbiz
        FEQ_S = 4,
        FEQ_D = 5,

        // Integer Not Equal
        // Dyadic e.g. bne
        // Monadic e.g. bnz
        INE_B = 6,
        INE_W = 7,
        INE_L = 8,
        INE_Q = 9,

        // Float Not Equal
        // Dyadic e.g. fbne
        // Monadic e.g fbnz
        FNE_S = 10,
        FNE_D = 11,


        // Signed Integer Less Than
        // Dyadic e.g. blt
        // Monadic e.g bmi
        ILT_B  = 12,
        ILT_W  = 13,
        ILT_L  = 14,
        ILT_Q  = 15,

        // Unsigned Integer Less Than
        // Dyadic e.g. blo
        ULT_B  = 16,
        ULT_W  = 17,
        ULT_L  = 18,
        ULT_Q  = 19,

        // Float Less Than
        // Dyadic e.g. fblt
        FLT_S  = 20,
        FLT_D  = 21,

        // Signed Integer Less Or Equal
        // Dyadic e.g. ble
        ILE_B  = 22,
        ILE_W  = 23,
        ILE_L  = 24,
        ILE_Q  = 25,

        // Unigned Integer Less Or Equal
        // Dyadic e.g. bls
        ULE_B  = 26,
        ULE_W  = 27,
        ULE_L  = 28,
        ULE_Q  = 29,

        // Float less or Equal
        // Dyadic e.g. fble
        FLE_S  = 30,
        FLE_D  = 31,

        // Signed Integer Greater Or Equal
        // Dyadic e.g. bge
        IGE_B  = 32,
        IGE_W  = 33,
        IGE_L  = 34,
        IGE_Q  = 35,

        // Unsigned Integer Greater Or Equal
        // Dyadic e.g. bhs
        UGE_B  = 36,
        UGE_W  = 37,
        UGE_L  = 38,
        UGE_Q  = 39,

        // Float Greater Or Equal
        // Dyadic e.g. fbge
        FGE_S  = 40,
        FGE_D  = 41,

        // Signed Integer Greater Than
        // Dyadic e.g bgt
        // Monadic e.g bpl
        IGT_B  = 42,
        IGT_W  = 43,
        IGT_L  = 44,
        IGT_Q  = 45,

        // Unsigned Integer Greater Than
        // Dyadic e.g, bhi
        UGT_B  = 46,
        UGT_W  = 47,
        UGT_L  = 48,
        UGT_Q  = 49,

        // Float Greater Than
        // Dyadic e.g. fbgt
        FGT_S  = 50,
        FGT_D  = 51,

        // Integer Bit position set
        BPS_B  = 52,
        BPS_W  = 53,
        BPS_L  = 54,
        BPS_Q  = 55,

        // Integer Bit position clear
        BPC_B  = 56,
        BPC_W  = 57,
        BPC_L  = 58,
        BPC_Q  = 59
	;
}

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
 * IControl
 *
 * Enumerates flow control instruction opcodes.
 */
interface IControl extends IByteCodeGroups {
    const
        STOP    = self::OFS_CONTROL +  0, // Execution will halt on encountering 0x00
        HOST    = self::OFS_CONTROL +  1,
        BRA_B   = self::OFS_CONTROL +  2,
        BRA     = self::OFS_CONTROL +  3,
        BSR_B   = self::OFS_CONTROL +  4,
        BSR     = self::OFS_CONTROL +  5,
        JMP     = self::OFS_CONTROL +  6,
        JSR     = self::OFS_CONTROL +  7,
        RTS     = self::OFS_CONTROL +  8,

        // Branch on condition: monadic. Compares <ea> to zero.
        BMC	    = self::OFS_CONTROL +  9,

        // Branch on condition: dyadic. Compares <ea:src> to <ea:dst>
        BDC     = self::OFS_CONTROL + 10,

        // Decrement and branch if not zero
        DBNZ    = self::OFS_CONTROL + 11,

        // Branch on condition: monadic. Compares register to zero.
        R_BMC   = self::OFS_CONTROL + 12,

        // Branch on condition: dyadic. Compares register pair
        R2R_BDC = self::OFS_CONTROL + 13,

        // Decrement and branch if not zero, register only
        R_DBNZ  = self::OFS_CONTROL + 14,

        // Extended compound opcodes for monadic test and branch

        // Branch if <ea> == 0
        BIZ_B   = self::BMC << 8 | ICondition::IEQ_B,
        BIZ_W   = self::BMC << 8 | ICondition::IEQ_W,
        BIZ_L   = self::BMC << 8 | ICondition::IEQ_L,
        BIZ_Q   = self::BMC << 8 | ICondition::IEQ_Q,
        FBIZ_S  = self::BMC << 8 | ICondition::FEQ_S,
        FBIZ_D  = self::BMC << 8 | ICondition::FEQ_D,

        // Branch if <ea> != 0
        BNZ_B   = self::BMC << 8 | ICondition::INE_B,
        BNZ_W   = self::BMC << 8 | ICondition::INE_W,
        BNZ_L   = self::BMC << 8 | ICondition::INE_L,
        BNZ_Q   = self::BMC << 8 | ICondition::INE_Q,
        FBNZ_S  = self::BMC << 8 | ICondition::FNE_S,
        FBNZ_D  = self::BMC << 8 | ICondition::FNE_D,

        // Branch if <ea> < 0
        BMI_B   = self::BMC << 8 | ICondition::ILT_B,
        BMI_W   = self::BMC << 8 | ICondition::ILT_W,
        BMI_L   = self::BMC << 8 | ICondition::ILT_L,
        BMI_Q   = self::BMC << 8 | ICondition::ILT_Q,
        FBMI_S  = self::BMC << 8 | ICondition::FLT_S,
        FBMI_D  = self::BMC << 8 | ICondition::FLT_D,

        // Branch if <ea> > 0
        BPL_B   = self::BMC << 8 | ICondition::IGT_B,
        BPL_W   = self::BMC << 8 | ICondition::IGT_W,
        BPL_L   = self::BMC << 8 | ICondition::IGT_L,
        BPL_Q   = self::BMC << 8 | ICondition::IGT_Q,
        FBPL_S  = self::BMC << 8 | ICondition::FGT_S,
        FBPL_D  = self::BMC << 8 | ICondition::FGT_D,

        // Extended compound opcodes for dyadic compare and branch

        // Branch if <ea(s)> < <ea(d)>
        BLO_B   = self::BDC << 8 | ICondition::ULT_B, // unsigned: Lower
        BLO_W   = self::BDC << 8 | ICondition::ULT_W,
        BLO_L   = self::BDC << 8 | ICondition::ULT_L,
        BLO_Q   = self::BDC << 8 | ICondition::ULT_Q,

        BLT_B   = self::BDC << 8 | ICondition::ILT_B, // signed: Less Than
        BLT_W   = self::BDC << 8 | ICondition::ILT_W,
        BLT_L   = self::BDC << 8 | ICondition::ILT_L,
        BLT_Q   = self::BDC << 8 | ICondition::ILT_Q,
        FBLT_S  = self::BDC << 8 | ICondition::FLT_S,
        FBLT_D  = self::BDC << 8 | ICondition::FLT_D,

        // Branch if <ea(s)> <= <ea(d)>
        BLS_B   = self::BDC << 8 | ICondition::ULE_B, // unsigned: Lower or Same
        BLS_W   = self::BDC << 8 | ICondition::ULE_W,
        BLS_L   = self::BDC << 8 | ICondition::ULE_L,
        BLS_Q   = self::BDC << 8 | ICondition::ULE_Q,

        BLE_B   = self::BDC << 8 | ICondition::ILE_B, // signed: Less or Equal
        BLE_W   = self::BDC << 8 | ICondition::ILE_W,
        BLE_L   = self::BDC << 8 | ICondition::ILE_L,
        BLE_Q   = self::BDC << 8 | ICondition::ILE_Q,
        FBLE_S  = self::BDC << 8 | ICondition::FLE_S,
        FBLE_D  = self::BDC << 8 | ICondition::FLE_D,

        // Branch if <ea(s)> == <ea(d)>
        BEQ_B   = self::BDC << 8 | ICondition::IEQ_B,
        BEQ_W   = self::BDC << 8 | ICondition::IEQ_W,
        BEQ_L   = self::BDC << 8 | ICondition::IEQ_L,
        BEQ_Q   = self::BDC << 8 | ICondition::IEQ_Q,
        FBEQ_S  = self::BDC << 8 | ICondition::FEQ_S,
        FBEQ_D  = self::BDC << 8 | ICondition::FEQ_D,

        // Branch if <ea(s)> >= <ea(d)>
        BHS_B   = self::BDC << 8 | ICondition::UGE_B, // unsigned: Higher or Same
        BHS_W   = self::BDC << 8 | ICondition::UGE_W,
        BHS_L   = self::BDC << 8 | ICondition::UGE_L,
        BHS_Q   = self::BDC << 8 | ICondition::UGE_Q,

        BGE_B   = self::BDC << 8 | ICondition::IGE_B, // signed: Greater or Equal
        BGE_W   = self::BDC << 8 | ICondition::IGE_W,
        BGE_L   = self::BDC << 8 | ICondition::IGE_L,
        BGE_Q   = self::BDC << 8 | ICondition::IGE_Q,
        FBGE_S  = self::BDC << 8 | ICondition::FGE_S,
        FBGE_D  = self::BDC << 8 | ICondition::FGE_D,

        // Branch if <ea(s)> > <ea(d)>
        BHI_B   = self::BDC << 8 | ICondition::UGT_B, // unsigned: Higher
        BHI_W   = self::BDC << 8 | ICondition::UGT_W,
        BHI_L   = self::BDC << 8 | ICondition::UGT_L,
        BHI_Q   = self::BDC << 8 | ICondition::UGT_Q,

        BGT_B   = self::BDC << 8 | ICondition::IGT_B, // signed: Greater Than
        BGT_W   = self::BDC << 8 | ICondition::IGT_W,
        BGT_L   = self::BDC << 8 | ICondition::IGT_L,
        BGT_Q   = self::BDC << 8 | ICondition::IGT_Q,
        FBGT_S  = self::BDC << 8 | ICondition::FGT_S,
        FBGT_D  = self::BDC << 8 | ICondition::FGT_D,

        // Branch if <ea(s)> != <ea(d)>
        BNE_B   = self::BDC << 8 | ICondition::INE_B,
        BNE_W   = self::BDC << 8 | ICondition::INE_W,
        BNE_L   = self::BDC << 8 | ICondition::INE_L,
        BNE_Q   = self::BDC << 8 | ICondition::INE_Q,
        FBNE_S  = self::BDC << 8 | ICondition::FNE_S,
        FBNE_D  = self::BDC << 8 | ICondition::FNE_D,

        // Branch if bit set
        BBS_B   = self::BDC << 8 | ICondition::BPS_B,
        BBS_W   = self::BDC << 8 | ICondition::BPS_W,
        BBS_L   = self::BDC << 8 | ICondition::BPS_L,
        BBS_Q   = self::BDC << 8 | ICondition::BPS_Q,

        // Branch if bit clear
        BBC_B   = self::BDC << 8 | ICondition::BPC_B,
        BBC_W   = self::BDC << 8 | ICondition::BPC_W,
        BBC_L   = self::BDC << 8 | ICondition::BPC_L,
        BBC_Q   = self::BDC << 8 | ICondition::BPC_Q

    ;
}

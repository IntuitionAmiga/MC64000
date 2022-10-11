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
 * IDataMove
 *
 * Enumerates data movement instruction opcodes.
 */
interface IDataMove extends IByteCodeGroups {
    const
        // Fast path register-to-register versions
        R2R_MOVE_L   = self::OFS_DATA_MOVE +  0,
        R2R_MOVE_Q   = self::OFS_DATA_MOVE +  1,
        R2R_FMOVE_S  = self::OFS_DATA_MOVE +  2,
        R2R_FMOVE_D  = self::OFS_DATA_MOVE +  3,
        R2R_CLR_L    = self::OFS_DATA_MOVE +  4,
        R2R_CLR_Q    = self::OFS_DATA_MOVE +  5,

        // Register-to-register by default
        R2R_EXG      = self::OFS_DATA_MOVE +  6,
        R2R_FEXG     = self::OFS_DATA_MOVE +  7,
        R2R_SWAP     = self::OFS_DATA_MOVE +  8,
        R2R_SWAP_L   = self::OFS_DATA_MOVE +  9,
        R2R_SWAP_Q   = self::OFS_DATA_MOVE + 10,

        // Generalised <ea> versions
        MOVE_B       = self::OFS_DATA_MOVE + 11,
        MOVE_W       = self::OFS_DATA_MOVE + 12,
        MOVE_L       = self::OFS_DATA_MOVE + 13,
        MOVE_Q       = self::OFS_DATA_MOVE + 14,
        SAVEM        = self::OFS_DATA_MOVE + 15,
        LOADM        = self::OFS_DATA_MOVE + 16,
        FMOVEB_S     = self::OFS_DATA_MOVE + 17,
        FMOVEB_D     = self::OFS_DATA_MOVE + 18,
        FMOVEW_S     = self::OFS_DATA_MOVE + 19,

        FMOVEW_D     = self::OFS_DATA_MOVE + 20,
        FMOVEL_S     = self::OFS_DATA_MOVE + 21,
        FMOVEL_D     = self::OFS_DATA_MOVE + 22,
        FMOVEQ_S     = self::OFS_DATA_MOVE + 23,
        FMOVEQ_D     = self::OFS_DATA_MOVE + 24,
        FMOVES_L     = self::OFS_DATA_MOVE + 25,
        FMOVES_Q     = self::OFS_DATA_MOVE + 26,
        FMOVES_D     = self::OFS_DATA_MOVE + 27,
        FMOVED_L     = self::OFS_DATA_MOVE + 28,
        FMOVED_Q     = self::OFS_DATA_MOVE + 29,

        FMOVED_S     = self::OFS_DATA_MOVE + 30,
        FMOVE_S      = self::OFS_DATA_MOVE + 31,
        FMOVE_D      = self::OFS_DATA_MOVE + 32,
        FINFO_S      = self::OFS_DATA_MOVE + 33,
        FINFO_D      = self::OFS_DATA_MOVE + 34,
        CLR_B        = self::OFS_DATA_MOVE + 35,
        CLR_W        = self::OFS_DATA_MOVE + 36,
        CLR_L        = self::OFS_DATA_MOVE + 37,
        CLR_Q        = self::OFS_DATA_MOVE + 38,
        LINK         = self::OFS_DATA_MOVE + 39,

        UNLK         = self::OFS_DATA_MOVE + 40,
        LEA          = self::OFS_DATA_MOVE + 41,
        PEA          = self::OFS_DATA_MOVE + 42,
        SCM          = self::OFS_DATA_MOVE + 43, // set conditional (monadic compare)
        SCD          = self::OFS_DATA_MOVE + 44,  // set conditional (dyadic compare)

        // Set if <ea> == 0
        SIZ_B   = self::SCM << 8 | ICondition::IEQ_B,
        SIZ_W   = self::SCM << 8 | ICondition::IEQ_W,
        SIZ_L   = self::SCM << 8 | ICondition::IEQ_L,
        SIZ_Q   = self::SCM << 8 | ICondition::IEQ_Q,
        FSIZ_S  = self::SCM << 8 | ICondition::FEQ_S,
        FSIZ_D  = self::SCM << 8 | ICondition::FEQ_D,

        // Set if <ea> != 0
        SNZ_B   = self::SCM << 8 | ICondition::INE_B,
        SNZ_W   = self::SCM << 8 | ICondition::INE_W,
        SNZ_L   = self::SCM << 8 | ICondition::INE_L,
        SNZ_Q   = self::SCM << 8 | ICondition::INE_Q,
        FSNZ_S  = self::SCM << 8 | ICondition::FNE_S,
        FSNZ_D  = self::SCM << 8 | ICondition::FNE_D,

        // Set if <ea> < 0
        SMI_B   = self::SCM << 8 | ICondition::ILT_B,
        SMI_W   = self::SCM << 8 | ICondition::ILT_W,
        SMI_L   = self::SCM << 8 | ICondition::ILT_L,
        SMI_Q   = self::SCM << 8 | ICondition::ILT_Q,
        FSMI_S  = self::SCM << 8 | ICondition::FLT_S,
        FSMI_D  = self::SCM << 8 | ICondition::FLT_D,

        // Set if <ea> > 0
        SPL_B   = self::SCM << 8 | ICondition::IGT_B,
        SPL_W   = self::SCM << 8 | ICondition::IGT_W,
        SPL_L   = self::SCM << 8 | ICondition::IGT_L,
        SPL_Q   = self::SCM << 8 | ICondition::IGT_Q,
        FSPL_S  = self::SCM << 8 | ICondition::FGT_S,
        FSPL_D  = self::SCM << 8 | ICondition::FGT_D,


        // Set if <ea(s)> < <ea(d)>
        SLO_B   = self::SCD << 8 | ICondition::ULT_B, // unsigned: Lower
        SLO_W   = self::SCD << 8 | ICondition::ULT_W,
        SLO_L   = self::SCD << 8 | ICondition::ULT_L,
        SLO_Q   = self::SCD << 8 | ICondition::ULT_Q,

        SLT_B   = self::SCD << 8 | ICondition::ILT_B, // signed: Less Than
        SLT_W   = self::SCD << 8 | ICondition::ILT_W,
        SLT_L   = self::SCD << 8 | ICondition::ILT_L,
        SLT_Q   = self::SCD << 8 | ICondition::ILT_Q,
        FSLT_S  = self::SCD << 8 | ICondition::FLT_S,
        FSLT_D  = self::SCD << 8 | ICondition::FLT_D,

        // Set if <ea(s)> <= <ea(d)>
        SLS_B   = self::SCD << 8 | ICondition::ULE_B, // unsigned: Lower or Same
        SLS_W   = self::SCD << 8 | ICondition::ULE_W,
        SLS_L   = self::SCD << 8 | ICondition::ULE_L,
        SLS_Q   = self::SCD << 8 | ICondition::ULE_Q,

        SLE_B   = self::SCD << 8 | ICondition::ILE_B, // signed: Less or Equal
        SLE_W   = self::SCD << 8 | ICondition::ILE_W,
        SLE_L   = self::SCD << 8 | ICondition::ILE_L,
        SLE_Q   = self::SCD << 8 | ICondition::ILE_Q,
        FSLE_S  = self::SCD << 8 | ICondition::FLE_S,
        FSLE_D  = self::SCD << 8 | ICondition::FLE_D,

        // Set if <ea(s)> == <ea(d)>
        SEQ_B   = self::SCD << 8 | ICondition::IEQ_B,
        SEQ_W   = self::SCD << 8 | ICondition::IEQ_W,
        SEQ_L   = self::SCD << 8 | ICondition::IEQ_L,
        SEQ_Q   = self::SCD << 8 | ICondition::IEQ_Q,
        FSEQ_S  = self::SCD << 8 | ICondition::FEQ_S,
        FSEQ_D  = self::SCD << 8 | ICondition::FEQ_D,

        // Set if <ea(s)> >= <ea(d)>
        SHS_B   = self::SCD << 8 | ICondition::UGE_B, // unsigned: Higher or Same
        SHS_W   = self::SCD << 8 | ICondition::UGE_W,
        SHS_L   = self::SCD << 8 | ICondition::UGE_L,
        SHS_Q   = self::SCD << 8 | ICondition::UGE_Q,

        SGE_B   = self::SCD << 8 | ICondition::IGE_B, // signed: Greater or Equal
        SGE_W   = self::SCD << 8 | ICondition::IGE_W,
        SGE_L   = self::SCD << 8 | ICondition::IGE_L,
        SGE_Q   = self::SCD << 8 | ICondition::IGE_Q,
        FSGE_S  = self::SCD << 8 | ICondition::FGE_S,
        FSGE_D  = self::SCD << 8 | ICondition::FGE_D,

        // Set if <ea(s)> > <ea(d)>
        SHI_B   = self::SCD << 8 | ICondition::UGT_B, // unsigned: Higher
        SHI_W   = self::SCD << 8 | ICondition::UGT_W,
        SHI_L   = self::SCD << 8 | ICondition::UGT_L,
        SHI_Q   = self::SCD << 8 | ICondition::UGT_Q,

        SGT_B   = self::SCD << 8 | ICondition::IGT_B, // signed: Greater Than
        SGT_W   = self::SCD << 8 | ICondition::IGT_W,
        SGT_L   = self::SCD << 8 | ICondition::IGT_L,
        SGT_Q   = self::SCD << 8 | ICondition::IGT_Q,
        FSGT_S  = self::SCD << 8 | ICondition::FGT_S,
        FSGT_D  = self::SCD << 8 | ICondition::FGT_D,

        // Set if <ea(s)> != <ea(d)>
        SNE_B   = self::SCD << 8 | ICondition::INE_B,
        SNE_W   = self::SCD << 8 | ICondition::INE_W,
        SNE_L   = self::SCD << 8 | ICondition::INE_L,
        SNE_Q   = self::SCD << 8 | ICondition::INE_Q,
        FSNE_S  = self::SCD << 8 | ICondition::FNE_S,
        FSNE_D  = self::SCD << 8 | ICondition::FNE_D,

        // Set if bit set
        SBS_B   = self::SCD << 8 | ICondition::BPS_B,
        SBS_W   = self::SCD << 8 | ICondition::BPS_W,
        SBS_L   = self::SCD << 8 | ICondition::BPS_L,
        SBS_Q   = self::SCD << 8 | ICondition::BPS_Q,

        // Set if bit clear
        SBC_B   = self::SCD << 8 | ICondition::BPC_B,
        SBC_W   = self::SCD << 8 | ICondition::BPC_W,
        SBC_L   = self::SCD << 8 | ICondition::BPC_L,
        SBC_Q   = self::SCD << 8 | ICondition::BPC_Q

    ;

}

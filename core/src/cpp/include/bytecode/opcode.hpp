#ifndef __MC64K_BYTECODE_INSTRUCTION_HPP__
#   ifndef __MC64K_BYTECODE_HPP__
#       error "Header not included at correct scope"
#   endif
#   define __MC64K_BYTECODE_OPCODE_HPP__

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

/**
 * MC64K::ByteCode::Opcode
 *
 * Enumeration of instruction groups.
 *
 * @todo header generation from template definition shared with php assembler sources.
 */
namespace Opcode {

    enum Group {
        OFS_CONTROL    = 0,
        OFS_DATA_MOVE  = 77,
        OFS_LOGICAL    = 112,
        OFS_ARITHMETIC = 160,
        OFS_MAX        = 255
    };

    enum Control {
        HCF    = OFS_CONTROL +  0, // 0x00
        BRA_B  = OFS_CONTROL +  1,
        BRA    = OFS_CONTROL +  2,
        BSR_B  = OFS_CONTROL +  3,
        BSR    = OFS_CONTROL +  4,
        JMP    = OFS_CONTROL +  5,
        JSR    = OFS_CONTROL +  6,
        RTS    = OFS_CONTROL +  7,
        BIZ_B  = OFS_CONTROL +  8,
        BIZ_W  = OFS_CONTROL +  9,
        BIZ_L  = OFS_CONTROL + 10,
        BIZ_Q  = OFS_CONTROL + 11,
        FBIZ_S = OFS_CONTROL + 12,
        FBIZ_D = OFS_CONTROL + 13,
        BNZ_B  = OFS_CONTROL + 14,
        BNZ_W  = OFS_CONTROL + 15,

        BNZ_L  = OFS_CONTROL + 16, // 0x10
        BNZ_Q  = OFS_CONTROL + 17,
        FBNZ_S = OFS_CONTROL + 18,
        FBNZ_D = OFS_CONTROL + 19,
        BMI_B  = OFS_CONTROL + 20,
        BMI_W  = OFS_CONTROL + 21,
        BMI_L  = OFS_CONTROL + 22,
        BMI_Q  = OFS_CONTROL + 23,
        FBMI_S = OFS_CONTROL + 24,
        FBMI_D = OFS_CONTROL + 25,
        BPL_B  = OFS_CONTROL + 26,
        BPL_W  = OFS_CONTROL + 27,
        BPL_L  = OFS_CONTROL + 28,
        BPL_Q  = OFS_CONTROL + 29,
        FBPL_S = OFS_CONTROL + 30,
        FBPL_D = OFS_CONTROL + 31,

        BLT_B  = OFS_CONTROL + 32, // 0x20
        BLT_W  = OFS_CONTROL + 33,
        BLT_L  = OFS_CONTROL + 34,
        BLT_Q  = OFS_CONTROL + 35,
        FBLT_S = OFS_CONTROL + 36,
        FBLT_D = OFS_CONTROL + 37,
        BLE_B  = OFS_CONTROL + 38,
        BLE_W  = OFS_CONTROL + 39,
        BLE_L  = OFS_CONTROL + 40,
        BLE_Q  = OFS_CONTROL + 41,
        FBLE_S = OFS_CONTROL + 42,
        FBLE_D = OFS_CONTROL + 43,
        BEQ_B  = OFS_CONTROL + 44,
        BEQ_W  = OFS_CONTROL + 45,
        BEQ_L  = OFS_CONTROL + 46,
        BEQ_Q  = OFS_CONTROL + 47,

        FBEQ_S = OFS_CONTROL + 48, // 0x30
        FBEQ_D = OFS_CONTROL + 49,
        BGE_B  = OFS_CONTROL + 50,
        BGE_W  = OFS_CONTROL + 51,
        BGE_L  = OFS_CONTROL + 52,
        BGE_Q  = OFS_CONTROL + 53,
        FBGE_S = OFS_CONTROL + 54,
        FBGE_D = OFS_CONTROL + 55,
        BGT_B  = OFS_CONTROL + 56,
        BGT_W  = OFS_CONTROL + 57,
        BGT_L  = OFS_CONTROL + 58,
        BGT_Q  = OFS_CONTROL + 59,
        FBGT_S = OFS_CONTROL + 60,
        FBGT_D = OFS_CONTROL + 61,
        BNE_B  = OFS_CONTROL + 62,
        BNE_W  = OFS_CONTROL + 63,

        BNE_L  = OFS_CONTROL + 64, // 0x40
        BNE_Q  = OFS_CONTROL + 65,
        FBNE_S = OFS_CONTROL + 66,
        FBNE_D = OFS_CONTROL + 67,
        BBS_B  = OFS_CONTROL + 68,
        BBS_W  = OFS_CONTROL + 69,
        BBS_L  = OFS_CONTROL + 70,
        BBS_Q  = OFS_CONTROL + 71,
        BBC_B  = OFS_CONTROL + 72,
        BBC_W  = OFS_CONTROL + 73,
        BBC_L  = OFS_CONTROL + 74,
        BBC_Q  = OFS_CONTROL + 75,
        DBNZ   = OFS_CONTROL + 76
    };

    enum DataMove {
        MOVE_B   = OFS_DATA_MOVE +  0, // 0x4D
        MOVE_W   = OFS_DATA_MOVE +  1,
        MOVE_L   = OFS_DATA_MOVE +  2,

        MOVE_Q   = OFS_DATA_MOVE +  3, // 0x50
        SAVEM    = OFS_DATA_MOVE +  4,
        LOADM    = OFS_DATA_MOVE +  5,
        FMOVEB_S = OFS_DATA_MOVE +  6,
        FMOVEB_D = OFS_DATA_MOVE +  7,
        FMOVEW_S = OFS_DATA_MOVE +  8,
        FMOVEW_D = OFS_DATA_MOVE +  9,
        FMOVEL_S = OFS_DATA_MOVE + 10,
        FMOVEL_D = OFS_DATA_MOVE + 11,
        FMOVEQ_S = OFS_DATA_MOVE + 12,
        FMOVEQ_D = OFS_DATA_MOVE + 13,
        FMOVES_L = OFS_DATA_MOVE + 14,
        FMOVES_Q = OFS_DATA_MOVE + 15,
        FMOVES_D = OFS_DATA_MOVE + 16,
        FMOVED_L = OFS_DATA_MOVE + 17,
        FMOVED_Q = OFS_DATA_MOVE + 18,

        FMOVED_S = OFS_DATA_MOVE + 19, // 0x60
        FSAVEM   = OFS_DATA_MOVE + 20,
        FLOADM   = OFS_DATA_MOVE + 21,
        CLR_B    = OFS_DATA_MOVE + 22,
        CLR_W    = OFS_DATA_MOVE + 23,
        CLR_L    = OFS_DATA_MOVE + 24,
        CLR_Q    = OFS_DATA_MOVE + 25,
        EXG      = OFS_DATA_MOVE + 26,
        FEXG     = OFS_DATA_MOVE + 27,
        SWAP     = OFS_DATA_MOVE + 28,
        SWAP_L   = OFS_DATA_MOVE + 29,
        SWAP_Q   = OFS_DATA_MOVE + 30,
        LINK     = OFS_DATA_MOVE + 31,
        UNLK     = OFS_DATA_MOVE + 32,
        LEA      = OFS_DATA_MOVE + 33,
        PEA      = OFS_DATA_MOVE + 34 // 0x6F
    };

    enum Logical {
        AND_B    = OFS_LOGICAL +  0, // 0x70
        AND_W    = OFS_LOGICAL +  1,
        AND_L    = OFS_LOGICAL +  2,
        AND_Q    = OFS_LOGICAL +  3,
        OR_B     = OFS_LOGICAL +  4,
        OR_W     = OFS_LOGICAL +  5,
        OR_L     = OFS_LOGICAL +  6,
        OR_Q     = OFS_LOGICAL +  7,
        EOR_B    = OFS_LOGICAL +  8,
        EOR_W    = OFS_LOGICAL +  9,
        EOR_L    = OFS_LOGICAL + 10,
        EOR_Q    = OFS_LOGICAL + 11,
        NOT_B    = OFS_LOGICAL + 12,
        NOT_W    = OFS_LOGICAL + 13,
        NOT_L    = OFS_LOGICAL + 14,
        NOT_Q    = OFS_LOGICAL + 15,

        LSL_B    = OFS_LOGICAL + 16, // 0x80
        LSL_W    = OFS_LOGICAL + 17,
        LSL_L    = OFS_LOGICAL + 18,
        LSL_Q    = OFS_LOGICAL + 19,
        LSR_B    = OFS_LOGICAL + 20,
        LSR_W    = OFS_LOGICAL + 21,
        LSR_L    = OFS_LOGICAL + 22,
        LSR_Q    = OFS_LOGICAL + 23,
        ROL_B    = OFS_LOGICAL + 24,
        ROL_W    = OFS_LOGICAL + 25,
        ROL_L    = OFS_LOGICAL + 26,
        ROL_Q    = OFS_LOGICAL + 27,
        ROR_B    = OFS_LOGICAL + 28,
        ROR_W    = OFS_LOGICAL + 29,
        ROR_L    = OFS_LOGICAL + 30,
        ROR_Q    = OFS_LOGICAL + 31,

        BCLR_B   = OFS_LOGICAL + 32, // 0x90
        BCLR_W   = OFS_LOGICAL + 33,
        BCLR_L   = OFS_LOGICAL + 34,
        BCLR_Q   = OFS_LOGICAL + 35,
        BSET_B   = OFS_LOGICAL + 36,
        BSET_W   = OFS_LOGICAL + 37,
        BSET_L   = OFS_LOGICAL + 38,
        BSET_Q   = OFS_LOGICAL + 39,
        BFCLR    = OFS_LOGICAL + 40,
        BFSET    = OFS_LOGICAL + 41,
        BFINS    = OFS_LOGICAL + 42,
        BFEXTS   = OFS_LOGICAL + 43,
        BFEXTU   = OFS_LOGICAL + 44,
        BFFFO    = OFS_LOGICAL + 45,
        BFCNT    = OFS_LOGICAL + 46,
        BFXXX    = OFS_LOGICAL + 47
    };

    enum Arithmetic {
        EXTB_W    = OFS_ARITHMETIC +  0, // 0xA0
        EXTB_L    = OFS_ARITHMETIC +  1,
        EXTB_Q    = OFS_ARITHMETIC +  2,
        EXTW_L    = OFS_ARITHMETIC +  3,
        EXTW_Q    = OFS_ARITHMETIC +  4,
        EXTL_Q    = OFS_ARITHMETIC +  5,
        ASL_B     = OFS_ARITHMETIC +  6,
        ASL_W     = OFS_ARITHMETIC +  7,
        ASL_L     = OFS_ARITHMETIC +  8,
        ASL_Q     = OFS_ARITHMETIC +  9,
        ASR_B     = OFS_ARITHMETIC + 10,
        ASR_W     = OFS_ARITHMETIC + 11,
        ASR_L     = OFS_ARITHMETIC + 12,
        ASR_Q     = OFS_ARITHMETIC + 13,
        ADD_B     = OFS_ARITHMETIC + 14,
        ADD_W     = OFS_ARITHMETIC + 15,

        ADD_L     = OFS_ARITHMETIC + 16, // 0xB0
        ADD_Q     = OFS_ARITHMETIC + 17,
        FADD_S    = OFS_ARITHMETIC + 18,
        FADD_D    = OFS_ARITHMETIC + 19,
        SUB_B     = OFS_ARITHMETIC + 20,
        SUB_W     = OFS_ARITHMETIC + 21,
        SUB_L     = OFS_ARITHMETIC + 22,
        SUB_Q     = OFS_ARITHMETIC + 23,
        FSUB_S    = OFS_ARITHMETIC + 24,
        FSUB_D    = OFS_ARITHMETIC + 25,
        NEG_B     = OFS_ARITHMETIC + 26,
        NEG_W     = OFS_ARITHMETIC + 27,
        NEG_L     = OFS_ARITHMETIC + 28,
        NEG_Q     = OFS_ARITHMETIC + 29,
        FNEG_S    = OFS_ARITHMETIC + 30,
        FNEG_D    = OFS_ARITHMETIC + 31,

        MULS_B    = OFS_ARITHMETIC + 32, // 0xC0
        MULS_W    = OFS_ARITHMETIC + 33,
        MULS_L    = OFS_ARITHMETIC + 34,
        MULS_Q    = OFS_ARITHMETIC + 35,
        MULU_B    = OFS_ARITHMETIC + 36,
        MULU_W    = OFS_ARITHMETIC + 37,
        MULU_L    = OFS_ARITHMETIC + 38,
        MULU_Q    = OFS_ARITHMETIC + 39,
        FMUL_S    = OFS_ARITHMETIC + 40,
        FMUL_D    = OFS_ARITHMETIC + 41,
        DIVS_B    = OFS_ARITHMETIC + 42,
        DIVS_W    = OFS_ARITHMETIC + 43,
        DIVS_L    = OFS_ARITHMETIC + 44,
        DIVS_Q    = OFS_ARITHMETIC + 45,
        DIVU_B    = OFS_ARITHMETIC + 46,
        DIVU_W    = OFS_ARITHMETIC + 47,

        DIVU_L    = OFS_ARITHMETIC + 48, // 0xD0
        DIVU_Q    = OFS_ARITHMETIC + 49,
        FDIV_S    = OFS_ARITHMETIC + 50,
        FDIV_D    = OFS_ARITHMETIC + 51,
        FMOD_S    = OFS_ARITHMETIC + 52,
        FMOD_D    = OFS_ARITHMETIC + 53,
        FABS_S    = OFS_ARITHMETIC + 54,
        FABS_D    = OFS_ARITHMETIC + 55,
        FSQRT_S   = OFS_ARITHMETIC + 56,
        FSQRT_D   = OFS_ARITHMETIC + 57,
        FACOS_S   = OFS_ARITHMETIC + 58,
        FACOS_D   = OFS_ARITHMETIC + 59,
        FASIN_S   = OFS_ARITHMETIC + 60,
        FASIN_D   = OFS_ARITHMETIC + 61,
        FATAN_S   = OFS_ARITHMETIC + 62,
        FATAN_D   = OFS_ARITHMETIC + 63,

        FCOS_S    = OFS_ARITHMETIC + 64, // 0xE0
        FCOS_D    = OFS_ARITHMETIC + 65,
        FSIN_S    = OFS_ARITHMETIC + 66,
        FSIN_D    = OFS_ARITHMETIC + 67,
        FSINCOS_S = OFS_ARITHMETIC + 68,
        FSINCOS_D = OFS_ARITHMETIC + 69,
        FTAN_S    = OFS_ARITHMETIC + 70,
        FTAN_D    = OFS_ARITHMETIC + 71,
        FETOX_S   = OFS_ARITHMETIC + 72,
        FETOX_D   = OFS_ARITHMETIC + 73,
        FLOGN_S   = OFS_ARITHMETIC + 74,
        FLOGN_D   = OFS_ARITHMETIC + 75,
        FLOG2_S   = OFS_ARITHMETIC + 76,
        FLOG2_D   = OFS_ARITHMETIC + 77,
        FTWOTOX_S = OFS_ARITHMETIC + 78,
        FTWOTOX_D = OFS_ARITHMETIC + 79,

        FGETEXP_S = OFS_ARITHMETIC + 80, // 0xF0
        FGETEXP_D = OFS_ARITHMETIC + 81,
        FGETMAN_S = OFS_ARITHMETIC + 82,
        FGETMAN_D = OFS_ARITHMETIC + 83
    };
}

#endif

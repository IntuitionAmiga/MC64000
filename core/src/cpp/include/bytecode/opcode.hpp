#ifndef MC64K_BYTECODE_OPCODE_HPP
#   define MC64K_BYTECODE_OPCODE_HPP

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
namespace MC64K::ByteCode::Opcode {

/**
 * Group
 *
 * Enumerates the base offsets for each Operation Type
 */
enum Group {
    OFS_CONTROL    = 0,
    OFS_DATA_MOVE  = 13,
    OFS_LOGICAL    = OFS_DATA_MOVE + 45,
    OFS_ARITHMETIC = OFS_LOGICAL + 54,
    OFS_OTHER      = OFS_ARITHMETIC + 110,
    OFS_MAX        = 255
};

/**
 * Prefix
 *
 * Enumerates special prefix conditions that usually signify some alternative bytecode representation or
 * instruction set extension.
 */
enum Prefix {
    INT_R2R = 254,
    FLT_R2R = 255
};

/**
 * Control
 *
 * Enumerates control flow opcodes
 */
enum Control {
    STOP   = OFS_CONTROL + 0,
    HOST   = OFS_CONTROL +  1,
    BRA_B  = OFS_CONTROL +  2,
    BRA    = OFS_CONTROL +  3,
    BSR_B  = OFS_CONTROL +  4,
    BSR    = OFS_CONTROL +  5,
    JMP    = OFS_CONTROL +  6,
    JSR    = OFS_CONTROL +  7,
    RTS    = OFS_CONTROL +  8,

    // Branch on condition: monadic. Compares <ea> to zero.
    BMC	   = OFS_CONTROL +  9,

    // Branch on condition: dyadic. Compares <ea:src> to <ea:dst>
    BDC    = OFS_CONTROL + 10,

    // Decrement and branch if not zero (64-bit register counter only)
    DBNZ   = OFS_CONTROL + 11,

    // Register only variant
    DBNZ_R = OFS_CONTROL + 12
};

enum Condition {

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
};

/**
 * DataMove
 *
 * Enumerates data movement/conversion opcodes
 */
enum DataMove {
    // Fast path register-to-register versions
    R2R_MOVE_L   = OFS_DATA_MOVE +  0,
    R2R_MOVE_Q   = OFS_DATA_MOVE +  1,
    R2R_FMOVE_S  = OFS_DATA_MOVE +  2,
    R2R_FMOVE_D  = OFS_DATA_MOVE +  3,
    R2R_CLR_L    = OFS_DATA_MOVE +  4,
    R2R_CLR_Q    = OFS_DATA_MOVE +  5,

    // Register-to-register by default
    R2R_EXG      = OFS_DATA_MOVE +  6,
    R2R_FEXG     = OFS_DATA_MOVE +  7,
    R2R_SWAP     = OFS_DATA_MOVE +  8,
    R2R_SWAP_L   = OFS_DATA_MOVE +  9,
    R2R_SWAP_Q   = OFS_DATA_MOVE + 10,

    // Generalised <ea> versions
    MOVE_B       = OFS_DATA_MOVE + 11,
    MOVE_W       = OFS_DATA_MOVE + 12,
    MOVE_L       = OFS_DATA_MOVE + 13,
    MOVE_Q       = OFS_DATA_MOVE + 14,
    SAVEM        = OFS_DATA_MOVE + 15,
    LOADM        = OFS_DATA_MOVE + 16,
    FMOVEB_S     = OFS_DATA_MOVE + 17,
    FMOVEB_D     = OFS_DATA_MOVE + 18,
    FMOVEW_S     = OFS_DATA_MOVE + 19,

    FMOVEW_D     = OFS_DATA_MOVE + 20,
    FMOVEL_S     = OFS_DATA_MOVE + 21,
    FMOVEL_D     = OFS_DATA_MOVE + 22,
    FMOVEQ_S     = OFS_DATA_MOVE + 23,
    FMOVEQ_D     = OFS_DATA_MOVE + 24,
    FMOVES_L     = OFS_DATA_MOVE + 25,
    FMOVES_Q     = OFS_DATA_MOVE + 26,
    FMOVES_D     = OFS_DATA_MOVE + 27,
    FMOVED_L     = OFS_DATA_MOVE + 28,
    FMOVED_Q     = OFS_DATA_MOVE + 29,

    FMOVED_S     = OFS_DATA_MOVE + 30,
    FMOVE_S      = OFS_DATA_MOVE + 31,
    FMOVE_D      = OFS_DATA_MOVE + 32,
    FINFO_S      = OFS_DATA_MOVE + 33,
    FINFO_D      = OFS_DATA_MOVE + 34,
    CLR_B        = OFS_DATA_MOVE + 35,
    CLR_W        = OFS_DATA_MOVE + 36,
    CLR_L        = OFS_DATA_MOVE + 37,
    CLR_Q        = OFS_DATA_MOVE + 38,
    LINK         = OFS_DATA_MOVE + 39,

    UNLK         = OFS_DATA_MOVE + 40,
    LEA          = OFS_DATA_MOVE + 41,
    PEA          = OFS_DATA_MOVE + 42,
    SCM          = OFS_DATA_MOVE + 43, // set conditional (monadic compare)
    SCD          = OFS_DATA_MOVE + 44  // set conditional (dyadic compare)
};

/**
 * Logical
 *
 * Enumerates bitwise logical opcodes
 */
enum Logical {
    // Fast path register-to-register versions
    R2R_AND_L  = OFS_LOGICAL +  0,
    R2R_AND_Q  = OFS_LOGICAL +  1,
    R2R_OR_L   = OFS_LOGICAL +  2,
    R2R_OR_Q   = OFS_LOGICAL +  3,
    R2R_EOR_L  = OFS_LOGICAL +  4,
    R2R_EOR_Q  = OFS_LOGICAL +  5,
    R2R_NOT_L  = OFS_LOGICAL +  6,
    R2R_NOT_Q  = OFS_LOGICAL +  7,
    R2R_LSL_L  = OFS_LOGICAL +  8,
    R2R_LSL_Q  = OFS_LOGICAL +  9,
    R2R_LSR_L  = OFS_LOGICAL + 10,
    R2R_LSR_Q  = OFS_LOGICAL + 11,

    AND_B      = OFS_LOGICAL + 12,
    AND_W      = OFS_LOGICAL + 13,
    AND_L      = OFS_LOGICAL + 14,
    AND_Q      = OFS_LOGICAL + 15,

    OR_B       = OFS_LOGICAL + 16,
    OR_W       = OFS_LOGICAL + 17,
    OR_L       = OFS_LOGICAL + 18,
    OR_Q       = OFS_LOGICAL + 19,

    EOR_B      = OFS_LOGICAL + 20,
    EOR_W      = OFS_LOGICAL + 21,
    EOR_L      = OFS_LOGICAL + 22,
    EOR_Q      = OFS_LOGICAL + 23,

    NOT_B      = OFS_LOGICAL + 24,
    NOT_W      = OFS_LOGICAL + 25,
    NOT_L      = OFS_LOGICAL + 26,
    NOT_Q      = OFS_LOGICAL + 27,

    LSL_B      = OFS_LOGICAL + 28,
    LSL_W      = OFS_LOGICAL + 29,
    LSL_L      = OFS_LOGICAL + 30,
    LSL_Q      = OFS_LOGICAL + 31,

    LSR_B      = OFS_LOGICAL + 32,
    LSR_W      = OFS_LOGICAL + 33,
    LSR_L      = OFS_LOGICAL + 34,
    LSR_Q      = OFS_LOGICAL + 35,

    ROL_B      = OFS_LOGICAL + 36,
    ROL_W      = OFS_LOGICAL + 37,
    ROL_L      = OFS_LOGICAL + 38,
    ROL_Q      = OFS_LOGICAL + 39,

    ROR_B      = OFS_LOGICAL + 40,
    ROR_W      = OFS_LOGICAL + 41,
    ROR_L      = OFS_LOGICAL + 42,
    ROR_Q      = OFS_LOGICAL + 43,

    BCLR_B     = OFS_LOGICAL + 44,
    BCLR_W     = OFS_LOGICAL + 45,
    BCLR_L     = OFS_LOGICAL + 46,
    BCLR_Q     = OFS_LOGICAL + 47,

    BSET_B     = OFS_LOGICAL + 48,
    BSET_W     = OFS_LOGICAL + 49,
    BSET_L     = OFS_LOGICAL + 50,
    BSET_Q     = OFS_LOGICAL + 51,

    BFFFO      = OFS_LOGICAL + 52,
    BFCNT      = OFS_LOGICAL + 53
};

/**
 * Arithmetic
 *
 * Enumerates arithmetic (integer and floating point) operations.
 */
enum Arithmetic {
    // Fast path register to register
    R2R_EXTB_L  = OFS_ARITHMETIC +  0,
    R2R_EXTB_Q  = OFS_ARITHMETIC +  1,
    R2R_EXTW_L  = OFS_ARITHMETIC +  2,
    R2R_EXTW_Q  = OFS_ARITHMETIC +  3,
    R2R_EXTL_Q  = OFS_ARITHMETIC +  4,
    R2R_NEG_L   = OFS_ARITHMETIC +  5,
    R2R_NEG_Q   = OFS_ARITHMETIC +  6,
    R2R_FNEG_S  = OFS_ARITHMETIC +  7,
    R2R_FNEG_D  = OFS_ARITHMETIC +  8,
    R2R_FABS_S  = OFS_ARITHMETIC +  9,

    R2R_FABS_D  = OFS_ARITHMETIC + 10,
    R2R_ADD_L   = OFS_ARITHMETIC + 11,
    R2R_ADD_Q   = OFS_ARITHMETIC + 12,
    R2R_FADD_S  = OFS_ARITHMETIC + 13,
    R2R_FADD_D  = OFS_ARITHMETIC + 14,
    R2R_SUB_L   = OFS_ARITHMETIC + 15,
    R2R_SUB_Q   = OFS_ARITHMETIC + 16,
    R2R_FSUB_S  = OFS_ARITHMETIC + 17,
    R2R_FSUB_D  = OFS_ARITHMETIC + 18,
    R2R_MULS_L  = OFS_ARITHMETIC + 19,

    R2R_MULS_Q  = OFS_ARITHMETIC + 20,
    R2R_MULU_L  = OFS_ARITHMETIC + 21,
    R2R_MULU_Q  = OFS_ARITHMETIC + 22,
    R2R_FMUL_S  = OFS_ARITHMETIC + 23,
    R2R_FMUL_D  = OFS_ARITHMETIC + 24,
    R2R_FDIV_S  = OFS_ARITHMETIC + 25,
    R2R_FDIV_D  = OFS_ARITHMETIC + 26,
    R2R_FMOD_S  = OFS_ARITHMETIC + 27,
    R2R_FMOD_D  = OFS_ARITHMETIC + 28,
    R2R_FSQRT_S = OFS_ARITHMETIC + 29,

    R2R_FSQRT_D = OFS_ARITHMETIC + 30,
    EXTB_W      = OFS_ARITHMETIC + 31,
    EXTB_L      = OFS_ARITHMETIC + 32,
    EXTB_Q      = OFS_ARITHMETIC + 33,
    EXTW_L      = OFS_ARITHMETIC + 34,
    EXTW_Q      = OFS_ARITHMETIC + 35,
    EXTL_Q      = OFS_ARITHMETIC + 36,
    ASL_B       = OFS_ARITHMETIC + 37,
    ASL_W       = OFS_ARITHMETIC + 38,
    ASL_L       = OFS_ARITHMETIC + 39,

    ASL_Q       = OFS_ARITHMETIC + 40,
    ASR_B       = OFS_ARITHMETIC + 41,
    ASR_W       = OFS_ARITHMETIC + 42,
    ASR_L       = OFS_ARITHMETIC + 43,
    ASR_Q       = OFS_ARITHMETIC + 44,
    ADD_B       = OFS_ARITHMETIC + 45,
    ADD_W       = OFS_ARITHMETIC + 46,
    ADD_L       = OFS_ARITHMETIC + 47,
    ADD_Q       = OFS_ARITHMETIC + 48,
    FADD_S      = OFS_ARITHMETIC + 49,

    FADD_D      = OFS_ARITHMETIC + 50,
    SUB_B       = OFS_ARITHMETIC + 51,
    SUB_W       = OFS_ARITHMETIC + 52,
    SUB_L       = OFS_ARITHMETIC + 53,
    SUB_Q       = OFS_ARITHMETIC + 54,
    FSUB_S      = OFS_ARITHMETIC + 55,
    FSUB_D      = OFS_ARITHMETIC + 56,
    NEG_B       = OFS_ARITHMETIC + 57,
    NEG_W       = OFS_ARITHMETIC + 58,
    NEG_L       = OFS_ARITHMETIC + 59,

    NEG_Q       = OFS_ARITHMETIC + 60,
    FNEG_S      = OFS_ARITHMETIC + 61,
    FNEG_D      = OFS_ARITHMETIC + 62,
    MULS_B      = OFS_ARITHMETIC + 63,
    MULS_W      = OFS_ARITHMETIC + 64,
    MULS_L      = OFS_ARITHMETIC + 65,
    MULS_Q      = OFS_ARITHMETIC + 66,
    MULU_B      = OFS_ARITHMETIC + 67,
    MULU_W      = OFS_ARITHMETIC + 68,
    MULU_L      = OFS_ARITHMETIC + 69,

    MULU_Q      = OFS_ARITHMETIC + 70,
    FMUL_S      = OFS_ARITHMETIC + 71,
    FMUL_D      = OFS_ARITHMETIC + 72,
    DIVS_L      = OFS_ARITHMETIC + 73,
    DIVS_Q      = OFS_ARITHMETIC + 74,
    MODS_L      = OFS_ARITHMETIC + 75,
    MODS_Q      = OFS_ARITHMETIC + 76,
    DIVU_L      = OFS_ARITHMETIC + 77,
    DIVU_Q      = OFS_ARITHMETIC + 78,
    MODU_L      = OFS_ARITHMETIC + 79,

    MODU_Q      = OFS_ARITHMETIC + 80,
    FDIV_S      = OFS_ARITHMETIC + 81,
    FDIV_D      = OFS_ARITHMETIC + 82,
    FMOD_S      = OFS_ARITHMETIC + 83,
    FMOD_D      = OFS_ARITHMETIC + 84,
    FABS_S      = OFS_ARITHMETIC + 85,
    FABS_D      = OFS_ARITHMETIC + 86,
    FSQRT_S     = OFS_ARITHMETIC + 87,
    FSQRT_D     = OFS_ARITHMETIC + 88,
    FACOS_S     = OFS_ARITHMETIC + 89,

    FACOS_D     = OFS_ARITHMETIC + 90,
    FASIN_S     = OFS_ARITHMETIC + 91,
    FASIN_D     = OFS_ARITHMETIC + 92,
    FATAN_S     = OFS_ARITHMETIC + 93,
    FATAN_D     = OFS_ARITHMETIC + 94,
    FCOS_S      = OFS_ARITHMETIC + 95,
    FCOS_D      = OFS_ARITHMETIC + 96,
    FSIN_S      = OFS_ARITHMETIC + 97,
    FSIN_D      = OFS_ARITHMETIC + 98,
    FSINCOS_S   = OFS_ARITHMETIC + 99,

    FSINCOS_D   = OFS_ARITHMETIC + 100,
    FTAN_S      = OFS_ARITHMETIC + 101,
    FTAN_D      = OFS_ARITHMETIC + 102,
    FETOX_S     = OFS_ARITHMETIC + 103,
    FETOX_D     = OFS_ARITHMETIC + 104,
    FLOGN_S     = OFS_ARITHMETIC + 105,
    FLOGN_D     = OFS_ARITHMETIC + 106,
    FLOG2_S     = OFS_ARITHMETIC + 107,
    FLOG2_D     = OFS_ARITHMETIC + 108,
    FTWOTOX_S   = OFS_ARITHMETIC + 109,

    FTWOTOX_D   = OFS_ARITHMETIC + 110
};

} // namespace
#endif

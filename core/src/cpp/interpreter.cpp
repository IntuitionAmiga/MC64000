#include "include/mc64k.hpp"
#include <cstdio>

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

Register::GeneralPurpose StaticInterpreter::aGPR[Register::GeneralPurpose::MAX] = {};
Register::FloatingPoint  StaticInterpreter::aFPR[Register::FloatingPoint::MAX]  = {};

const uint8* StaticInterpreter::programCounter = 0;

/**
 *
 */
void StaticInterpreter::dumpState() {
    std::puts("GeneralPurpose");
    for (int i = 0; i < Register::GeneralPurpose::MAX; ++i) {
        std::printf(
            "\t%2d : 0x%016llX\n",
            i,
            aGPR[i].uQuad
        );
    }
    std::puts("FloatingPoint");
    for (int i = 0; i < Register::GeneralPurpose::MAX; ++i) {
        std::printf(
            "\t%2d : 0x%016llX %.15e %.7e\n",
            i,
            aFPR[i].uBinary,
            aFPR[i].fDouble,
            (float64)aFPR[i].fSingle
        );
    }
}

/**
 *
 */
void StaticInterpreter::run() {
    if (!programCounter) {
        return;
    }

    switch (*programCounter++) {
        // Control
        case Opcode::HCF:
        case Opcode::BRA_B:
        case Opcode::BRA:
        case Opcode::BSR_B:
        case Opcode::BSR:
        case Opcode::JMP:
        case Opcode::JSR:
        case Opcode::RTS:
        case Opcode::BIZ_B:
        case Opcode::BIZ_W:
        case Opcode::BIZ_L:
        case Opcode::BIZ_Q:
        case Opcode::FBIZ_S:
        case Opcode::FBIZ_D:
        case Opcode::BNZ_B:
        case Opcode::BNZ_W:
        case Opcode::BNZ_L:
        case Opcode::BNZ_Q:
        case Opcode::FBNZ_S:
        case Opcode::FBNZ_D:
        case Opcode::BMI_B:
        case Opcode::BMI_W:
        case Opcode::BMI_L:
        case Opcode::BMI_Q:
        case Opcode::FBMI_S:
        case Opcode::FBMI_D:
        case Opcode::BPL_B:
        case Opcode::BPL_W:
        case Opcode::BPL_L:
        case Opcode::BPL_Q:
        case Opcode::FBPL_S:
        case Opcode::FBPL_D:
        case Opcode::BLT_B:
        case Opcode::BLT_W:
        case Opcode::BLT_L:
        case Opcode::BLT_Q:
        case Opcode::FBLT_S:
        case Opcode::FBLT_D:
        case Opcode::BLE_B:
        case Opcode::BLE_W:
        case Opcode::BLE_L:
        case Opcode::BLE_Q:
        case Opcode::FBLE_S:
        case Opcode::FBLE_D:
        case Opcode::BEQ_B:
        case Opcode::BEQ_W:
        case Opcode::BEQ_L:
        case Opcode::BEQ_Q:
        case Opcode::FBEQ_S:
        case Opcode::FBEQ_D:
        case Opcode::BGE_B:
        case Opcode::BGE_W:
        case Opcode::BGE_L:
        case Opcode::BGE_Q:
        case Opcode::FBGE_S:
        case Opcode::FBGE_D:
        case Opcode::BGT_B:
        case Opcode::BGT_W:
        case Opcode::BGT_L:
        case Opcode::BGT_Q:
        case Opcode::FBGT_S:
        case Opcode::FBGT_D:
        case Opcode::BNE_B:
        case Opcode::BNE_W:
        case Opcode::BNE_L:
        case Opcode::BNE_Q:
        case Opcode::FBNE_S:
        case Opcode::FBNE_D:
        case Opcode::BBS_B:
        case Opcode::BBS_W:
        case Opcode::BBS_L:
        case Opcode::BBS_Q:
        case Opcode::BBC_B:
        case Opcode::BBC_W:
        case Opcode::BBC_L:
        case Opcode::BBC_Q:
        case Opcode::DBNZ:

        // DataMove
        case Opcode::MOVE_B:
        case Opcode::MOVE_W:
        case Opcode::MOVE_L:
        case Opcode::MOVE_Q:
        case Opcode::SAVEM:
        case Opcode::LOADM:
        case Opcode::FMOVEB_S:
        case Opcode::FMOVEB_D:
        case Opcode::FMOVEW_S:
        case Opcode::FMOVEW_D:
        case Opcode::FMOVEL_S:
        case Opcode::FMOVEL_D:
        case Opcode::FMOVEQ_S:
        case Opcode::FMOVEQ_D:
        case Opcode::FMOVES_L:
        case Opcode::FMOVES_Q:
        case Opcode::FMOVES_D:
        case Opcode::FMOVED_L:
        case Opcode::FMOVED_Q:
        case Opcode::FMOVED_S:
        case Opcode::FSAVEM:
        case Opcode::FLOADM:
        case Opcode::CLR_B:
        case Opcode::CLR_W:
        case Opcode::CLR_L:
        case Opcode::CLR_Q:
        case Opcode::EXG:
        case Opcode::FEXG:
        case Opcode::SWAP:
        case Opcode::SWAP_L:
        case Opcode::SWAP_Q:
        case Opcode::LINK:
        case Opcode::UNLK:
        case Opcode::LEA:
        case Opcode::PEA:

        // Logical
        case Opcode::AND_B:
        case Opcode::AND_W:
        case Opcode::AND_L:
        case Opcode::AND_Q:
        case Opcode::OR_B:
        case Opcode::OR_W:
        case Opcode::OR_L:
        case Opcode::OR_Q:
        case Opcode::EOR_B:
        case Opcode::EOR_W:
        case Opcode::EOR_L:
        case Opcode::EOR_Q:
        case Opcode::NOT_B:
        case Opcode::NOT_W:
        case Opcode::NOT_L:
        case Opcode::NOT_Q:
        case Opcode::LSL_B:
        case Opcode::LSL_W:
        case Opcode::LSL_L:
        case Opcode::LSL_Q:
        case Opcode::LSR_B:
        case Opcode::LSR_W:
        case Opcode::LSR_L:
        case Opcode::LSR_Q:
        case Opcode::ROL_B:
        case Opcode::ROL_W:
        case Opcode::ROL_L:
        case Opcode::ROL_Q:
        case Opcode::ROR_B:
        case Opcode::ROR_W:
        case Opcode::ROR_L:
        case Opcode::ROR_Q:
        case Opcode::BCLR_B:
        case Opcode::BCLR_W:
        case Opcode::BCLR_L:
        case Opcode::BCLR_Q:
        case Opcode::BSET_B:
        case Opcode::BSET_W:
        case Opcode::BSET_L:
        case Opcode::BSET_Q:
        case Opcode::BFCLR:
        case Opcode::BFSET:
        case Opcode::BFINS:
        case Opcode::BFEXTS:
        case Opcode::BFEXTU:
        case Opcode::BFFFO:
        case Opcode::BFCNT:
        case Opcode::BFXXX:

        // Arithmetic
        case Opcode::EXTB_W:
        case Opcode::EXTB_L:
        case Opcode::EXTB_Q:
        case Opcode::EXTW_L:
        case Opcode::EXTW_Q:
        case Opcode::EXTL_Q:
        case Opcode::ASL_B:
        case Opcode::ASL_W:
        case Opcode::ASL_L:
        case Opcode::ASL_Q:
        case Opcode::ASR_B:
        case Opcode::ASR_W:
        case Opcode::ASR_L:
        case Opcode::ASR_Q:
        case Opcode::ADD_B:
        case Opcode::ADD_W:
        case Opcode::ADD_L:
        case Opcode::ADD_Q:
        case Opcode::FADD_S:
        case Opcode::FADD_D:
        case Opcode::SUB_B:
        case Opcode::SUB_W:
        case Opcode::SUB_L:
        case Opcode::SUB_Q:
        case Opcode::FSUB_S:
        case Opcode::FSUB_D:
        case Opcode::NEG_B:
        case Opcode::NEG_W:
        case Opcode::NEG_L:
        case Opcode::NEG_Q:
        case Opcode::FNEG_S:
        case Opcode::FNEG_D:
        case Opcode::MULS_B:
        case Opcode::MULS_W:
        case Opcode::MULS_L:
        case Opcode::MULS_Q:
        case Opcode::MULU_B:
        case Opcode::MULU_W:
        case Opcode::MULU_L:
        case Opcode::MULU_Q:
        case Opcode::FMUL_S:
        case Opcode::FMUL_D:
        case Opcode::DIVS_B:
        case Opcode::DIVS_W:
        case Opcode::DIVS_L:
        case Opcode::DIVS_Q:
        case Opcode::DIVU_B:
        case Opcode::DIVU_W:
        case Opcode::DIVU_L:
        case Opcode::DIVU_Q:
        case Opcode::FDIV_S:
        case Opcode::FDIV_D:
        case Opcode::FMOD_S:
        case Opcode::FMOD_D:
        case Opcode::FABS_S:
        case Opcode::FABS_D:
        case Opcode::FSQRT_S:
        case Opcode::FSQRT_D:
        case Opcode::FACOS_S:
        case Opcode::FACOS_D:
        case Opcode::FASIN_S:
        case Opcode::FASIN_D:
        case Opcode::FATAN_S:
        case Opcode::FATAN_D:
        case Opcode::FCOS_S:
        case Opcode::FCOS_D:
        case Opcode::FSIN_S:
        case Opcode::FSIN_D:
        case Opcode::FSINCOS_S:
        case Opcode::FSINCOS_D:
        case Opcode::FTAN_S:
        case Opcode::FTAN_D:
        case Opcode::FETOX_S:
        case Opcode::FETOX_D:
        case Opcode::FLOGN_S:
        case Opcode::FLOGN_D:
        case Opcode::FLOG2_S:
        case Opcode::FLOG2_D:
        case Opcode::FTWOTOX_S:
        case Opcode::FTWOTOX_D:
        case Opcode::FGETEXP_S:
        case Opcode::FGETEXP_D:
        case Opcode::FGETMAN_S:
        case Opcode::FGETMAN_D:
        default:
            return;
    }
}

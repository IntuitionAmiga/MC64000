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
#include "include/mc64k.hpp"
#include <cstdio>

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

Register::GeneralPurpose StaticInterpreter::aGPR[Register::GeneralPurpose::MAX] = {};
Register::FloatingPoint  StaticInterpreter::aFPR[Register::FloatingPoint::MAX]  = {};

const uint8* StaticInterpreter::pProgramCounter = 0;

void* StaticInterpreter::pDstEA = 0;
void* StaticInterpreter::pSrcEA = 0;
void* StaticInterpreter::pTmpEA = 0;
int   StaticInterpreter::iCallDepth = 0;

StaticInterpreter::OperationSize StaticInterpreter::eOperationSize = StaticInterpreter::SIZE_BYTE;
StaticInterpreter::Status        StaticInterpreter::eStatus        = StaticInterpreter::UNINITIALISED;

const char* aStatusNames[] = {
    "Uninitialised",
    "Initialised",
    "Running",
    "Completed",
    "Unimplemented Opcode",
};

/**
 *
 */
Register::GeneralPurpose& StaticInterpreter::gpr(const unsigned int uReg) {
    return aGPR[uReg & Register::GeneralPurpose::MASK];
}

/**
 *
 */
Register::FloatingPoint& StaticInterpreter::fpr(const unsigned int uReg) {
    return aFPR[uReg & Register::FloatingPoint::MASK];
}

/**
 *
 */
void StaticInterpreter::setProgramCounter(const uint8* pNewProgramCounter) {
    pProgramCounter = pNewProgramCounter;
}

/**
 *
 */
void StaticInterpreter::dumpState(const int iFlags) {
    std::printf(
        "Machine State\n"
        "\tProgram Counter: %p\n"
        "\tCall Depth:      %d\n"
        "\tStatus:          %d [%s]\n",
        pProgramCounter,
        iCallDepth,
        eStatus,
        aStatusNames[eStatus]
    );
    if (iFlags & STATE_TMP) {
        std::printf(
            "Last Operation\n"
            "\tData Size: %d\n"
            "\tDst Addr:  %p\n"
            "\tSrc Addr:  %p\n"
            "\tTmp Addr:  %p\n",
            eOperationSize,
            pDstEA,
            pSrcEA,
            pTmpEA
        );
    }
    if (iFlags & STATE_GPR) {
        std::printf("GP Registers (%p)\n", aGPR);
        for (int i = 0; i < Register::GeneralPurpose::MAX; ++i) {
            std::printf(
                "\t%2d : 0x%016lX\n",
                i,
                aGPR[i].uQuad
            );
        }
    }
    if (iFlags & STATE_FPR) {
        std::printf("FP Registers (%p)\n", aFPR);
        for (int i = 0; i < Register::GeneralPurpose::MAX; ++i) {
            std::printf(
                "\t%2d : 0x%016lX %.15e %.7e\n",
                i,
                aFPR[i].uBinary,
                aFPR[i].fDouble,
                (float64)aFPR[i].fSingle
            );
        }
    }

}

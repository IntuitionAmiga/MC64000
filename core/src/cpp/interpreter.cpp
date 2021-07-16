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

#include <cstdio>
#include "machine/interpreter.hpp"

namespace Interpreter = MC64K::Machine::Interpreter;
namespace Register    = MC64K::Machine::Register;

Register::GeneralPurpose Interpreter::Static::aGPR[Register::GeneralPurpose::MAX] = {};
Register::FloatingPoint  Interpreter::Static::aFPR[Register::FloatingPoint::MAX]  = {};

const uint8* Interpreter::Static::pProgramCounter = 0;

void* Interpreter::Static::pDstEA = 0;
void* Interpreter::Static::pSrcEA = 0;
void* Interpreter::Static::pTmpEA = 0;
int   Interpreter::Static::iCallDepth = 0;

Interpreter::Static::OperationSize Interpreter::Static::eOperationSize = Interpreter::Static::SIZE_BYTE;
Interpreter::Static::Status        Interpreter::Static::eStatus        = Interpreter::Static::UNINITIALISED;

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
Register::GeneralPurpose& Interpreter::Static::gpr(const unsigned int uReg) {
    return aGPR[uReg & Register::GeneralPurpose::MASK];
}

/**
 *
 */
Register::FloatingPoint& Interpreter::Static::fpr(const unsigned int uReg) {
    return aFPR[uReg & Register::FloatingPoint::MASK];
}

/**
 *
 */
void Interpreter::Static::setProgramCounter(const uint8* pNewProgramCounter) {
    pProgramCounter = pNewProgramCounter;
}

/**
 *
 */
void Interpreter::Static::dumpState(const int iFlags) {
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

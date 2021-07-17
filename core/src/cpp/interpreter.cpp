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

namespace MC64K {
namespace Machine {

GPRegister   Interpreter::aGPR[GPRegister::MAX] = {};
FPRegister   Interpreter::aFPR[FPRegister::MAX] = {};
const uint8* Interpreter::pProgramCounter       = 0;
void*        Interpreter::pDstEA                = 0;
void*        Interpreter::pSrcEA                = 0;
void*        Interpreter::pTmpEA                = 0;
int          Interpreter::iCallDepth            = 0;

Interpreter::OperationSize Interpreter::eOperationSize = Interpreter::SIZE_BYTE;
Interpreter::Status        Interpreter::eStatus        = Interpreter::UNINITIALISED;

const char* aStatusNames[] = {
    "Uninitialised",
    "Initialised",
    "Running",
    "Completed",
    "Unimplemented Opcode",
};

/**
 * Get a general purpose register
 */
GPRegister& Interpreter::gpr(const unsigned int uReg) {
    return aGPR[uReg & GPRegister::MASK];
}

/**
 * Get a floating point register
 */
FPRegister& Interpreter::fpr(const unsigned int uReg) {
    return aFPR[uReg & FPRegister::MASK];
}

/**
 * Set the entry point
 */
void Interpreter::setProgramCounter(const uint8* pNewProgramCounter) {
    pProgramCounter = pNewProgramCounter;
}

/**
 * Debugging
 */
void Interpreter::dumpState(const int iFlags) {
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
        for (int i = 0; i < GPRegister::MAX; ++i) {
            std::printf(
                "\t%2d : 0x%016lX\n",
                i,
                aGPR[i].uQuad
            );
        }
    }
    if (iFlags & STATE_FPR) {
        std::printf("FP Registers (%p)\n", aFPR);
        for (int i = 0; i < FPRegister::MAX; ++i) {
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

}} // namespace

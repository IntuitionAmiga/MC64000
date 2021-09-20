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
#include <cstring>
#include <cstdlib>
#include "machine/error.hpp"
#include "machine/interpreter.hpp"
#include "loader/executable.hpp"

namespace MC64K {
namespace Machine {

GPRegister   Interpreter::aoGPR[GPRegister::MAX] = {};
FPRegister   Interpreter::aoFPR[FPRegister::MAX] = {};
const uint8* Interpreter::puProgramCounter       = 0;
void*        Interpreter::pDstEA                 = 0;
void*        Interpreter::pSrcEA                 = 0;
void*        Interpreter::pTmpEA                 = 0;
uint8*       Interpreter::puStackTop             = 0;
uint8*       Interpreter::puStackBase            = 0;
int32        Interpreter::iCallDepth             = 0;
uint32       Interpreter::uNumHCFVectors         = 0;

const Interpreter::HCFVector* Interpreter::pcHCFVectors   = 0;
Interpreter::OperationSize    Interpreter::eOperationSize = Interpreter::SIZE_BYTE;
Interpreter::Status           Interpreter::eStatus        = Interpreter::UNINITIALISED;

/**
 * Human readable names for Interpreter::eStatus
 */
const char* asStatusNames[] = {
    "Uninitialised",
    "Initialised",
    "Running",
    "Completed",
    "Caught Fire",
    "Unimplemented Opcode",
    "Unimplemented Effective Address",
    "Unimplemented Host Call",
    "Invalid Entrypoint"
};

void Interpreter::initHCFVectors(const Interpreter::HCFVector* pcHCFVectors, const uint32 uNumHCFVectors) {
    if (uNumHCFVectors > MAX_HCF_VECTOR) {
        throw MC64K::OutOfRangeException("HCF Vector List Too Large");
    }
    Interpreter::pcHCFVectors   = pcHCFVectors;
    Interpreter::uNumHCFVectors = uNumHCFVectors;
}

/**
 * @inheritDoc
 */
void Interpreter::allocateStack(uint32 uSize) {
    uSize += (STACK_ALIGN - 1);
    uSize &= ~(STACK_ALIGN - 1);
    puStackBase = (uint8*)std::calloc(uSize, 1);
    if (!puStackBase) {
        throw Error("Failed to allocate stack");
    }
    puStackTop = puStackBase + uSize;
    aoGPR[GPRegister::SP].puByte = puStackTop;
    std::fprintf(
        stderr,
        "Stack of %u allocated at %p ... %p\n",
        uSize,
        puStackBase, puStackTop
    );
}

/**
 * @inheritDoc
 */
void Interpreter::freeStack() {
    std::free(puStackBase);
    puStackTop = puStackBase = 0;
}

/**
 * @inheritDoc
 */
GPRegister& Interpreter::gpr(const unsigned int uReg) {
    return aoGPR[uReg & GPRegister::MASK];
}

/**
 * @inheritDoc
 */
FPRegister& Interpreter::fpr(const unsigned int uReg) {
    return aoFPR[uReg & FPRegister::MASK];
}

/**
 * @inheritDoc
 */
void Interpreter::setProgramCounter(const uint8* puNewProgramCounter) {
    puProgramCounter = puNewProgramCounter;
}

/**
 * @inheritDoc
 */
void Interpreter::dumpState(std::FILE* poStream, const unsigned uFlags) {
    if (puProgramCounter) {
        std::fprintf(
            poStream,
            "Machine State\n"
            "\tProgram Counter: %p [... 0x%02X > 0x%02X < 0x%02X ...]\n"
            "\tCall Depth:      %d\n"
            "\tStatus:          %d [%s]\n\n",
            puProgramCounter,
            (uint32) *(puProgramCounter - 1),
            (uint32) *(puProgramCounter),
            (uint32) *(puProgramCounter + 1),
            iCallDepth,
            eStatus,
            asStatusNames[eStatus]
        );
    } else {
        std::fprintf(
            poStream,
            "Machine State\n"
            "\tCall Depth:      %d\n"
            "\tStatus:          %d [%s]\n\n",
            iCallDepth,
            eStatus,
            asStatusNames[eStatus]
        );
    }
    if (uFlags & STATE_TMP) {
        std::fprintf(
            poStream,
            "Last Operation\n"
            "\tData Size: %d\n"
            "\tDst Addr:  %p\n"
            "\tSrc Addr:  %p\n"
            "\tTmp Addr:  %p\n\n",
            eOperationSize,
            pDstEA,
            pSrcEA,
            pTmpEA
        );
    }
    if (uFlags & STATE_HCF) {
        std::fprintf(poStream, "HCF Vectors\n");
        if (pcHCFVectors && uNumHCFVectors) {
            for (uint32 u = 0; u < uNumHCFVectors; ++u) {
                std::fprintf(
                    poStream,
                    "\t%02X : %p\n",
                    u,
                    pcHCFVectors[u]
                );
            }
            std::fprintf(poStream, "\n");
        }
    }
    if (uFlags & STATE_GPR) {
        std::fprintf(poStream, "GP Registers (%p)\n", aoGPR);
        for (unsigned u = 0; u < GPRegister::MAX; ++u) {
            std::fprintf(
                poStream,
                "\t%2u : 0x%016lX\n",
                u,
                aoGPR[u].uQuad
            );
        }
        std::fprintf(poStream, "\n");
    }
    if (uFlags & STATE_FPR) {
        std::fprintf(poStream, "FP Registers (%p)\n", aoFPR);
        for (unsigned u = 0; u < FPRegister::MAX; ++u) {
            std::fprintf(
                poStream,
                "\t%2u : 0x%016lX %.15e %.7e\n",
                u,
                aoFPR[u].uBinary,
                aoFPR[u].fDouble,
                (float64)aoFPR[u].fSingle
            );
        }
        std::fprintf(poStream, "\n");
    }
    if (uFlags & STATE_STACK) {
        std::fprintf(poStream, "Stack\n");
        if (puStackTop && puStackBase) {
            uint64* pu = (uint64*)puStackTop;
            while (--pu > (uint64*)puStackBase) {
                std::fprintf(
                    poStream,
                    "\t%p : 0x%016lX\n",
                    pu,
                    *pu
                );
            }
        }
        std::fprintf(poStream, "\n");
    }

}

}} // namespace

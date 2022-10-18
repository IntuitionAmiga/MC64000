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
#include <cassert>
#include <machine/error.hpp>
#include <machine/limits.hpp>
#include <machine/interpreter.hpp>
#include <loader/executable.hpp>

namespace MC64K::Machine {

#ifdef USE_GLOBAL_PC
register uint8 const* puProgramCounter __asm__(USE_GLOBAL_PC);
#else
uint8 const* puProgramCounter;
#endif

#ifdef USE_GLOBAL_DEA
register void* pDstEA __asm__(USE_GLOBAL_DEA);
#else
void* pDstEA;
#endif

GPRegister      Interpreter::aoGPR[GPRegister::MAX] = {};
FPRegister      Interpreter::aoFPR[FPRegister::MAX] = {};
void*           Interpreter::pSrcEA                 = 0;
void*           Interpreter::pTmpEA                 = 0;
uint8*          Interpreter::puStackTop             = 0;
uint8*          Interpreter::puStackBase            = 0;
Loader::Symbol* Interpreter::poImportSymbols        = 0;
uint32          Interpreter::uNumHCFVectors         = 0;
uint32          Interpreter::uNumImportSymbols      = 0;

Interpreter::HCFVector const* Interpreter::pcHCFVectors   = 0;
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

/**
 * @inheritDoc
 */
void Interpreter::initHCFVectors(Interpreter::HCFVector const* pcHCFVectors, uint32 const uNumHCFVectors) {
    assert(uNumHCFVectors <= Limits::MAX_HCF_VECTORS);
    Interpreter::pcHCFVectors   = pcHCFVectors;
    Interpreter::uNumHCFVectors = uNumHCFVectors;
}

/**
 * @inheritDoc
 */
void Interpreter::initImportSymbols(Loader::Symbol* poImportSymbols, uint32 const uNumImportSymbols) {
    Interpreter::poImportSymbols   = poImportSymbols;
    Interpreter::uNumImportSymbols = uNumImportSymbols;
}


/**
 * @inheritDoc
 */
void Interpreter::allocateStack(uint32 uSize) {
    uSize += (Limits::STACK_ALIGN - 1);
    uSize &= ~(Limits::STACK_ALIGN - 1);
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
void Interpreter::setProgramCounter(Interpreter::VMCodeEntryPoint pByteCode) {
    puProgramCounter = pByteCode;
}

/**
 * @inheritDoc
 */
void Interpreter::dumpState(std::FILE* poStream, unsigned const uFlags) {
    if (puProgramCounter) {
        std::fprintf(
            poStream,
            "Machine State\n"
            "\tProgram Counter: %p [... 0x%02X > 0x%02X < 0x%02X ...]\n"
            "\tStatus:          %d [%s]\n\n",
            puProgramCounter,
            (uint32) *(puProgramCounter - 1),
            (uint32) *(puProgramCounter),
            (uint32) *(puProgramCounter + 1),
            eStatus,
            asStatusNames[eStatus]
        );
    } else {
        std::fprintf(
            poStream,
            "Machine State\n"
            "\tStatus:          %d [%s]\n\n",
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
            std::fprintf(
                poStream,
                "\tRn : %18s | %20s | %12s | %6s | %4s\n",
                "Raw Data",
                "Quad",
                "Long",
                "Word",
                "Byte"
            );

        for (unsigned u = 0; u < GPRegister::MAX; ++u) {
            std::fprintf(
                poStream,
                "\t%2u : 0x%016lX | %20" PFS64
                " | %12" PFS32
                " | %6"  PFS32
                " | %4"  PFS32 "\n",
                u,
                aoGPR[u].uQuad,
                aoGPR[u].iQuad,
                aoGPR[u].iLong,
                (int)aoGPR[u].iWord,
                (int)aoGPR[u].iByte
            );
        }
        std::fprintf(poStream, "\n");
    }
    if (uFlags & STATE_FPR) {
        std::fprintf(poStream, "FP Registers (%p)\n", aoFPR);
        std::fprintf(
            poStream,
            "\tRn : %18s | %25s | %14s\n",
            "Raw Data",
            "Double",
            "Single"
        );

        for (unsigned u = 0; u < FPRegister::MAX; ++u) {
            std::fprintf(
                poStream,
                "\t%2u : 0x%016lX | %25.17e | %14.7e\n",
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

} // namespace

#include "interpreter_ea.cpp"
#include "interpreter_bmc.cpp"
#include "interpreter_bdc.cpp"
#include "interpreter_smc.cpp"
#include "interpreter_sdc.cpp"
#include "interpreter_run.cpp"

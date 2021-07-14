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

StaticInterpreter::OperationSize StaticInterpreter::eOperationSize = StaticInterpreter::SIZE_BYTE;

/**
 *
 */
void StaticInterpreter::dumpState(const int iFlags) {
    if (iFlags & STATE_GPR) {
        std::printf("GeneralPurpose (%p)\n", aGPR);
        for (int i = 0; i < Register::GeneralPurpose::MAX; ++i) {
            std::printf(
                "\t%2d : 0x%016llX\n",
                i,
                aGPR[i].uQuad
            );
        }
    }
    if (iFlags & STATE_FPR) {
        std::printf("FloatingPoint (%p)\n", aFPR);
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
    if (iFlags & STATE_TMP) {
        std::printf(
            "Program Counter: %p, Size: %d, Dst EA:%p, Src EA:%p, Tmp EA:%p\n",
            pProgramCounter,
            eOperationSize,
            pDstEA,
            pSrcEA,
            pTmpEA
        );
    }
}

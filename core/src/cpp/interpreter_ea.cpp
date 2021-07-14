#include "include/mc64k.hpp"
#include <cstdio>

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

// Nasty macro that is here to be inlined / improved
#define readDisplacement(p, a) \
    a[0] = *p++; \
    a[1] = *p++; \
    a[2] = *p++; \
    a[3] = *p++; \

/**
 *
 */
void* StaticInterpreter::decodeEffectiveAddress() {

    // Union used to read 32-bit values from the bytecode into a useable value.
    union {
        int32 iDisplacement;
        uint8 uBytes[4];
    };

    uint8 uEffectiveAddress = *pProgramCounter++;
    uint8 uEALower = uEffectiveAddress & 0x0F; // Lower nybble varies, usually a register.

    // Switch based on the mode
    switch (uEffectiveAddress & 0xF0) {

        // General Purpose Register Direct r<N>
        case EffectiveAddress::OFS_GPR_DIR:
            return &aGPR[uEALower];

        // Register Indirect (r<N>)
        case EffectiveAddress::OFS_GPR_IND:
            return aGPR[uEALower].pAny;

        // Register Indirect, Post Increment (r<N>)+
        case EffectiveAddress::OFS_GPR_IND_POST_INC: {
            void* p = aGPR[uEALower].pAny;
            aGPR[uEALower].pIByte += eOperationSize;
            return p;
        }

        // Register Indirect, Post Increment (r<N>)-
        case EffectiveAddress::OFS_GPR_IND_POST_DEC: {
            void* p = aGPR[uEALower].pAny;
            aGPR[uEALower].pIByte -= eOperationSize;
            return p;
        }

        // Register Indirect, Post Decrement (r<N>)-
        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            aGPR[uEALower].pIByte += eOperationSize;
            return aGPR[uEALower].pAny;

        // Register Indirect, Post Decrement -(r<N>)
        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            aGPR[uEALower].pIByte -= eOperationSize;
            return aGPR[uEALower].pAny;

        // Register Indirect with displacement <d32>(r<N>) / (<d32>, r<N>)
        case EffectiveAddress::OFS_GPR_IND_DSP:
            readDisplacement(pProgramCounter, uBytes);
            return aGPR[uEALower].pIByte + iDisplacement;

        // FPU Register Direct fp<N>
        case EffectiveAddress::OFS_FPR_DIR:
            return &aFPR[uEALower];

        // Register Indirect with scaled index
        case EffectiveAddress::OFS_GPR_IDX: {
            uint8 uIndexReg = *pProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            switch (uEALower & 3) {
                case 0: return aGPR[uBaseReg].pIByte + (aGPR[uIndexReg].iByte << uScale);
                case 1: return aGPR[uBaseReg].pIByte + (aGPR[uIndexReg].iWord << uScale);
                case 2: return aGPR[uBaseReg].pIByte + (aGPR[uIndexReg].iLong << uScale);
                case 3: return aGPR[uBaseReg].pIByte + (aGPR[uIndexReg].iQuad << uScale);
            }
            break;
        }

        // Register Indirect with scaled index and displacement
        case EffectiveAddress::OFS_GPR_IDX_DSP: {
            uint8 uIndexReg = *pProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            readDisplacement(pProgramCounter, uBytes);
            switch (uEALower & 3) {
                case 0: return aGPR[uBaseReg].pIByte + iDisplacement + (aGPR[uIndexReg].iByte << uScale);
                case 1: return aGPR[uBaseReg].pIByte + iDisplacement + (aGPR[uIndexReg].iWord << uScale);
                case 2: return aGPR[uBaseReg].pIByte + iDisplacement + (aGPR[uIndexReg].iLong << uScale);
                case 3: return aGPR[uBaseReg].pIByte + iDisplacement + (aGPR[uIndexReg].iQuad << uScale);
            }
            break;
        }

        // Program Counter Indirect with scaled index
        case EffectiveAddress::OFS_PC_IND_IDX:
            return 0;

        // Program Counter Indirect with 8-bit scaled index and displacement
        case EffectiveAddress::OFS_PC_IND_IDX_DSP:
            return 0;

        case EffectiveAddress::OFS_OTHER: {
            switch (uEALower) {
                default:
                    break;
            }
            break;
        }

        // Same as destination
        case EffectiveAddress::SAME_AS_DEST:
            return pDstEA;

        default:
            break;
    }
    return 0;
}

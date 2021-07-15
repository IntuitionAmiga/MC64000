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
#include "include/gnarly.hpp"
#include <cstdio>

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

/**
 *
 */
void* StaticInterpreter::decodeEffectiveAddress() {

    initDisplacement();

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
            readDisplacement();
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
            readDisplacement();
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
                    // TODO
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

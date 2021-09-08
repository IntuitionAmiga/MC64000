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

#include "machine/interpreter.hpp"
#include "bytecode/effective_address.hpp"
#include "machine/gnarly.hpp"
#include <cstdio>

namespace MC64K {
namespace Machine {

namespace {
    /**
     *
     */
    union {
        float64 fDouble;
        float32 fSingle;
        int64   iQuad;
        uint8   uBytes[8];
    } oImmediate;
}

/**
 * decodeEffectiveAddress()
 */
void* Interpreter::decodeEffectiveAddress() {

    using namespace MC64K::ByteCode;

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
            eStatus = UNIMPLEMENTED_EAMODE;
            return 0;

        // Program Counter Indirect with 8-bit scaled index and displacement
        case EffectiveAddress::OFS_PC_IND_IDX_DSP:
            eStatus = UNIMPLEMENTED_EAMODE;
            return 0;

        case EffectiveAddress::OFS_OTHER: {
            if (uEALower <= EffectiveAddress::Other::INT_SMALL_8) {
                oImmediate.iQuad = uEALower;
                return &oImmediate.iQuad;
            }

            switch (uEALower) {
                // For integer immediates, we always sign extend to 64-bits as the operation mayb be bigger
                // than the immediate size.
                case EffectiveAddress::Other::INT_IMM_BYTE:
                    oImmediate.iQuad = (int8)*pProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_WORD:
                    oImmediate.iQuad  = (int8)pProgramCounter[1];
                    oImmediate.uBytes[0] = *pProgramCounter++;
                    oImmediate.uBytes[1] = *pProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_LONG:
                case EffectiveAddress::Other::FLT_IMM_SINGLE:
                    oImmediate.iQuad  = (int8)pProgramCounter[3];
                    oImmediate.uBytes[0] = *pProgramCounter++;
                    oImmediate.uBytes[1] = *pProgramCounter++;
                    oImmediate.uBytes[2] = *pProgramCounter++;
                    oImmediate.uBytes[3] = *pProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_QUAD:
                case EffectiveAddress::Other::FLT_IMM_DOUBLE:
                    oImmediate.uBytes[0] = *pProgramCounter++;
                    oImmediate.uBytes[1] = *pProgramCounter++;
                    oImmediate.uBytes[2] = *pProgramCounter++;
                    oImmediate.uBytes[3] = *pProgramCounter++;
                    oImmediate.uBytes[4] = *pProgramCounter++;
                    oImmediate.uBytes[5] = *pProgramCounter++;
                    oImmediate.uBytes[6] = *pProgramCounter++;
                    oImmediate.uBytes[7] = *pProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::PC_IND_DSP:
                    readDisplacement();
                    return (uint8*)pProgramCounter + iDisplacement;
                default:
                    // TODO
                    eStatus = UNIMPLEMENTED_EAMODE;
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

/**
 * Save the registers indicated by the mask to the effective address
 *
 */
void Interpreter::saveRegisters(uint32 uMask, uint8 uEAMode) {

    using namespace MC64K::ByteCode;

    uint32  uHalfMask;
    uint64* pStack = aGPR[uEAMode & 0xF].pUQuad;

    switch (uEAMode & 0xF0) {

        case EffectiveAddress::OFS_GPR_IND_POST_INC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(pStack++) = aGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(pStack++) = aFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_POST_DEC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(pStack--) = aGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(pStack--) = aFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(++pStack) = aGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(++pStack) = aFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(--pStack) = aGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(--pStack) = aFPR[i].uBinary;
                    }
                }
            }
            break;

        default:
            eStatus = UNIMPLEMENTED_EAMODE;
            return;
            break;
    }
    aGPR[uEAMode & 0xF].pUQuad = pStack;
}

/**
 * Restore the registers indicated by the mask to the effective address
 *
 */
void Interpreter::restoreRegisters(uint32 uMask, uint8 uEAMode) {

    using namespace MC64K::ByteCode;

    uint32  uHalfMask;
    uint64* pStack = aGPR[uEAMode & 0xF].pUQuad;

    switch (uEAMode & 0xF0) {
        case EffectiveAddress::OFS_GPR_IND_POST_INC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aFPR[i].uBinary = *pStack++;
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aGPR[i].uQuad = *pStack++;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_POST_DEC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aFPR[i].uBinary = *pStack--;
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aGPR[i].uQuad = *pStack--;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aFPR[i].uBinary = *(++pStack);
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aGPR[i].uQuad = *(++pStack);
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aFPR[i].uBinary = *(--pStack);
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aGPR[i].uQuad = *(--pStack);
                    }
                }
            }
            break;

        default:
            eStatus = UNIMPLEMENTED_EAMODE;
            return;
            break;
    }
    aGPR[uEAMode & 0xF].pUQuad = pStack;
}


}} // namespace

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
     * Union used for reading bytestream immediate EA data into a machine aligned type.
     */
    union {
        float64 fDouble;
        float32 fSingle;
        int64   iQuad;
        uint8   auBytes[8];
    } oImmediate;
}

/**
 * decodeEffectiveAddress()
 *
 * return void*
 */
void* Interpreter::decodeEffectiveAddress() {

    using namespace MC64K::ByteCode;

    initDisplacement();

    uint8 uEffectiveAddress = *puProgramCounter++;
    uint8 uEALower = uEffectiveAddress & 0x0F; // Lower nybble varies, usually a register.

    // Switch based on the mode
    switch (uEffectiveAddress & 0xF0) {

        // General Purpose Register Direct r<N>
        case EffectiveAddress::OFS_GPR_DIR:
            return &aoGPR[uEALower];

        // Register Indirect (r<N>)
        case EffectiveAddress::OFS_GPR_IND:
            return aoGPR[uEALower].pAny;

        // Register Indirect, Post Increment (r<N>)+
        case EffectiveAddress::OFS_GPR_IND_POST_INC: {
            void* p = aoGPR[uEALower].pAny;
            aoGPR[uEALower].piByte += eOperationSize;
            return p;
        }

        // Register Indirect, Post Increment (r<N>)-
        case EffectiveAddress::OFS_GPR_IND_POST_DEC: {
            void* p = aoGPR[uEALower].pAny;
            aoGPR[uEALower].piByte -= eOperationSize;
            return p;
        }

        // Register Indirect, Post Decrement (r<N>)-
        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            aoGPR[uEALower].piByte += eOperationSize;
            return aoGPR[uEALower].pAny;

        // Register Indirect, Post Decrement -(r<N>)
        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            aoGPR[uEALower].piByte -= eOperationSize;
            return aoGPR[uEALower].pAny;

        // Register Indirect with displacement <d32>(r<N>) / (<d32>, r<N>)
        case EffectiveAddress::OFS_GPR_IND_DSP:
            readDisplacement();
            return aoGPR[uEALower].piByte + iDisplacement;

        // FPU Register Direct fp<N>
        case EffectiveAddress::OFS_FPR_DIR:
            return &aoFPR[uEALower];

        // Register Indirect with scaled index
        case EffectiveAddress::OFS_GPR_IDX: {
            uint8 uIndexReg = *puProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            switch (uEALower & 3) {
                case 0: return aoGPR[uBaseReg].piByte + (aoGPR[uIndexReg].iByte << uScale);
                case 1: return aoGPR[uBaseReg].piByte + (aoGPR[uIndexReg].iWord << uScale);
                case 2: return aoGPR[uBaseReg].piByte + (aoGPR[uIndexReg].iLong << uScale);
                case 3: return aoGPR[uBaseReg].piByte + (aoGPR[uIndexReg].iQuad << uScale);
            }
            break;
        }

        // Register Indirect with scaled index and displacement
        case EffectiveAddress::OFS_GPR_IDX_DSP: {
            uint8 uIndexReg = *puProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            readDisplacement();
            switch (uEALower & 3) {
                case 0: return aoGPR[uBaseReg].piByte + iDisplacement + (aoGPR[uIndexReg].iByte << uScale);
                case 1: return aoGPR[uBaseReg].piByte + iDisplacement + (aoGPR[uIndexReg].iWord << uScale);
                case 2: return aoGPR[uBaseReg].piByte + iDisplacement + (aoGPR[uIndexReg].iLong << uScale);
                case 3: return aoGPR[uBaseReg].piByte + iDisplacement + (aoGPR[uIndexReg].iQuad << uScale);
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
                    oImmediate.iQuad = (int8)*puProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_WORD:
                    oImmediate.iQuad = (int8)puProgramCounter[1];
                    oImmediate.auBytes[0] = *puProgramCounter++;
                    oImmediate.auBytes[1] = *puProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_LONG:
                case EffectiveAddress::Other::FLT_IMM_SINGLE:
                    oImmediate.iQuad = (int8)puProgramCounter[3];
                    oImmediate.auBytes[0] = *puProgramCounter++;
                    oImmediate.auBytes[1] = *puProgramCounter++;
                    oImmediate.auBytes[2] = *puProgramCounter++;
                    oImmediate.auBytes[3] = *puProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_QUAD:
                case EffectiveAddress::Other::FLT_IMM_DOUBLE:
                    oImmediate.auBytes[0] = *puProgramCounter++;
                    oImmediate.auBytes[1] = *puProgramCounter++;
                    oImmediate.auBytes[2] = *puProgramCounter++;
                    oImmediate.auBytes[3] = *puProgramCounter++;
                    oImmediate.auBytes[4] = *puProgramCounter++;
                    oImmediate.auBytes[5] = *puProgramCounter++;
                    oImmediate.auBytes[6] = *puProgramCounter++;
                    oImmediate.auBytes[7] = *puProgramCounter++;
                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::PC_IND_DSP:
                    readDisplacement();
                    return (uint8*)puProgramCounter + iDisplacement;
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
 * @param uint32 uMask
 * @param uint8  uEAMode
 */
void Interpreter::saveRegisters(uint32 uMask, uint8 uEAMode) {

    using namespace MC64K::ByteCode;

    uint32  uHalfMask;
    uint64* puStack = aoGPR[uEAMode & 0xF].puQuad;

    switch (uEAMode & 0xF0) {

        case EffectiveAddress::OFS_GPR_IND_POST_INC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(puStack++) = aoGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(puStack++) = aoFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_POST_DEC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(puStack--) = aoGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(puStack--) = aoFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(++puStack) = aoGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(++puStack) = aoFPR[i].uBinary;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(--puStack) = aoGPR[i].uQuad;
                    }
                }
            }
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 15; i >= 0; --i) {
                    if (uHalfMask & (1 << i)) {
                        *(--puStack) = aoFPR[i].uBinary;
                    }
                }
            }
            break;

        default:
            eStatus = UNIMPLEMENTED_EAMODE;
            return;
            break;
    }
    aoGPR[uEAMode & 0xF].puQuad = puStack;
}

/**
 * Restore the registers indicated by the mask to the effective address
 *
 */
void Interpreter::restoreRegisters(uint32 uMask, uint8 uEAMode) {

    using namespace MC64K::ByteCode;

    uint32  uHalfMask;
    uint64* puStack = aoGPR[uEAMode & 0xF].puQuad;

    switch (uEAMode & 0xF0) {
        case EffectiveAddress::OFS_GPR_IND_POST_INC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoFPR[i].uBinary = *puStack++;
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoGPR[i].uQuad = *puStack++;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_POST_DEC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoFPR[i].uBinary = *puStack--;
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoGPR[i].uQuad = *puStack--;
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_INC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoFPR[i].uBinary = *(++puStack);
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoGPR[i].uQuad = *(++puStack);
                    }
                }
            }
            break;

        case EffectiveAddress::OFS_GPR_IND_PRE_DEC:
            if ( (uHalfMask = (uMask >> 16)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoFPR[i].uBinary = *(--puStack);
                    }
                }
            }
            if ( (uHalfMask = (uMask & 0xFFFF)) ) {
                for (int i = 0; i < 16; ++i) {
                    if (uHalfMask & (1 << i)) {
                        aoGPR[i].uQuad = *(--puStack);
                    }
                }
            }
            break;

        default:
            eStatus = UNIMPLEMENTED_EAMODE;
            return;
            break;
    }
    aoGPR[uEAMode & 0xF].puQuad = puStack;
}


}} // namespace

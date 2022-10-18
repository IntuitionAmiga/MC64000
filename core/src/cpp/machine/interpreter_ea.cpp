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
#include "loader/symbol.hpp"
#include "machine/gnarly.hpp"
#include <cstdio>

namespace MC64K::Machine {

namespace {
    /**
     * Union used for reading bytestream immediate EA data into a machine aligned type.
     */
    union {
        float64 fDouble;
        float32 fSingle;
        int64   iQuad;
        int32   iLong;
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
    switch (uEffectiveAddress >> 4) {

        // General Purpose Register Direct r<N>
        case EffectiveAddress::OFS_GPR_DIR >> 4:
            return &aoGPR[uEALower];

        // Register Indirect (r<N>)
        case EffectiveAddress::OFS_GPR_IND >> 4:
            return aoGPR[uEALower].pAny;

        // Register Indirect, Post Increment (r<N>)+
        case EffectiveAddress::OFS_GPR_IND_POST_INC >> 4: {
            void* p = aoGPR[uEALower].pAny;
            aoGPR[uEALower].piByte += eOperationSize;
            return p;
        }

        // Register Indirect, Post Increment (r<N>)-
        case EffectiveAddress::OFS_GPR_IND_POST_DEC >> 4: {
            void* p = aoGPR[uEALower].pAny;
            aoGPR[uEALower].piByte -= eOperationSize;
            return p;
        }

        // Register Indirect, Pre Increment +(r<N>)
        case EffectiveAddress::OFS_GPR_IND_PRE_INC >> 4:
            aoGPR[uEALower].piByte += eOperationSize;
            return aoGPR[uEALower].pAny;

        // Register Indirect, Post Decrement -(r<N>)
        case EffectiveAddress::OFS_GPR_IND_PRE_DEC >> 4:
            aoGPR[uEALower].piByte -= eOperationSize;
            return aoGPR[uEALower].pAny;

        // Register Indirect with displacement <d8>(r<N>) / (<d8>, r<N>)
        case EffectiveAddress::OFS_GPR_IND_DSP8 >> 4:
            readByteDisplacement();
            return aoGPR[uEALower].piByte + iDisplacement;

        // Register Indirect with displacement <d32>(r<N>) / (<d32>, r<N>)
        case EffectiveAddress::OFS_GPR_IND_DSP >> 4:
            readDisplacement();
            return aoGPR[uEALower].piByte + iDisplacement;

        // FPU Register Direct fp<N>
        case EffectiveAddress::OFS_FPR_DIR >> 4:
            return &aoFPR[uEALower];

        // Register Indirect with scaled index
        case EffectiveAddress::OFS_GPR_IDX >> 4: {
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

        // Register Indirect with scaled index and 8-bit displacement
        case EffectiveAddress::OFS_GPR_IDX_DSP8 >> 4: {
            uint8 uIndexReg = *puProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            readByteDisplacement();

            int8* piResult = aoGPR[uBaseReg].piByte + iDisplacement;
            switch (uEALower & 3) {
                case 0: return piResult + (aoGPR[uIndexReg].iByte << uScale);
                case 1: return piResult + (aoGPR[uIndexReg].iWord << uScale);
                case 2: return piResult + (aoGPR[uIndexReg].iLong << uScale);
                case 3: return piResult + (aoGPR[uIndexReg].iQuad << uScale);
            }
            break;
        }

        // Register Indirect with scaled index and 32-bit displacement
        case EffectiveAddress::OFS_GPR_IDX_DSP >> 4: {
            uint8 uIndexReg = *puProgramCounter++;
            uint8 uBaseReg  = uIndexReg >> 4;
            uint8 uScale    = uEALower >> 2;
            uIndexReg      &= 0xF;
            readDisplacement();

            int8* piResult = aoGPR[uBaseReg].piByte + iDisplacement;
            switch (uEALower & 3) {
                case 0: return piResult + (aoGPR[uIndexReg].iByte << uScale);
                case 1: return piResult + (aoGPR[uIndexReg].iWord << uScale);
                case 2: return piResult + (aoGPR[uIndexReg].iLong << uScale);
                case 3: return piResult + (aoGPR[uIndexReg].iQuad << uScale);
            }
            break;
        }

        case EffectiveAddress::OFS_OTHER >> 4: {
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

                #ifdef ALLOW_MISALIGNED_IMMEDIATE
                    oImmediate.iLong = *(int32*)puProgramCounter;
                    puProgramCounter += 4;
                #else
                    oImmediate.auBytes[0] = *puProgramCounter++;
                    oImmediate.auBytes[1] = *puProgramCounter++;
                    oImmediate.auBytes[2] = *puProgramCounter++;
                    oImmediate.auBytes[3] = *puProgramCounter++;
                #endif

                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::INT_IMM_QUAD:
                case EffectiveAddress::Other::FLT_IMM_DOUBLE:

                #ifdef ALLOW_MISALIGNED_IMMEDIATE
                    oImmediate.iQuad = *(int64*)puProgramCounter;
                    puProgramCounter += 8;
                #else
                    oImmediate.auBytes[0] = *puProgramCounter++;
                    oImmediate.auBytes[1] = *puProgramCounter++;
                    oImmediate.auBytes[2] = *puProgramCounter++;
                    oImmediate.auBytes[3] = *puProgramCounter++;
                    oImmediate.auBytes[4] = *puProgramCounter++;
                    oImmediate.auBytes[5] = *puProgramCounter++;
                    oImmediate.auBytes[6] = *puProgramCounter++;
                    oImmediate.auBytes[7] = *puProgramCounter++;
                #endif

                    return &oImmediate.iQuad;

                case EffectiveAddress::Other::PC_IND_DSP:
                    readDisplacement();
                    return (uint8*)puProgramCounter + iDisplacement;

                default:
                    break;
            }
            break;
        }
        case  EffectiveAddress::OFS_OTHER_2 >> 4: {
            switch (uEALower) {
                case EffectiveAddress::SAME_AS_DEST:
                    return pDstEA;

                case EffectiveAddress::IMPORT_SYMBOL_ID:
                    readSymbolIndex();
                    return (uint8*)poImportSymbols[uIndex].pRawData;

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }
    std::fprintf(stderr, "\tCrashing with bad EA mode: %02X\n", uEffectiveAddress);
    eStatus = UNIMPLEMENTED_EAMODE;
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


} // namespace

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
#include "machine/timing.hpp"
#include "bytecode/opcode.hpp"
#include "machine/gnarly.hpp"
#include <cstdio>
#include <cmath>
#include <csetjmp>

namespace MC64K {
namespace Machine {


inline void rolByte(uint8* puValue, uint8 uSize) {
    uSize &= 7;
    uint8 val = *puValue;
    *puValue = val << uSize | val >> (8 - uSize);
}

inline void rolWord(uint16* puValue, uint8 uSize) {
    uSize &= 15;
    uint16 val = *puValue;
    *puValue = val << uSize | val >> (16 - uSize);
}

inline void rolLong(uint32* puValue, uint8 uSize) {
    uSize &= 31;
    uint32 val = *puValue;
    *puValue = val << uSize | val >> (32 - uSize);
}

inline void rolQuad(uint64* puValue, uint8 uSize) {
    uSize &= 63;
    uint64 val = *puValue;
    *puValue = val << uSize | val >> (64 - uSize);
}

inline void rorByte(uint8* puValue, uint8 uSize) {
    uSize &= 7;
    uint8 val = *puValue;
    *puValue = val >> uSize | val << (8 - uSize);
}

inline void rorWord(uint16* puValue, uint8 uSize) {
    uSize &= 15;
    uint16 val = *puValue;
    *puValue = val >> uSize | val << (16 - uSize);
}

inline void rorLong(uint32* puValue, uint8 uSize) {
    uSize &= 31;
    uint32 val = *puValue;
    *puValue = val >> uSize | val << (32 - uSize);
}

inline void rorQuad(uint64* puValue, uint8 uSize) {
    uSize &= 63;
    uint64 val = *puValue;
    *puValue = val >> uSize | val << (64 - uSize);
}

/**
 * @inheritDoc
 */
void Interpreter::run() {

    using namespace MC64K::ByteCode;

    if (!puProgramCounter) {
        return;
    }

    std::fprintf(stderr, "Beginning run at PC:%p...\n", puProgramCounter);

    eStatus    = RUNNING;
    iCallDepth = 1;

    initDisplacement();

    uint64 uInstructionCount = 0;
    Nanoseconds::Value uStart = Nanoseconds::mark();

    while (RUNNING == eStatus) {

        skipstatus:

        ++uInstructionCount;
        switch (*puProgramCounter++) {

            // Integer register to register fast path prefix
            case Opcode::INT_R2R: {
                unpackGPRPair();

                switch (*puProgramCounter++) {
                    case Opcode::DBNZ:   readDisplacement(); bcc(--asULong(pDstEA)); goto skipstatus;
                    case Opcode::MOVE_L: asULong(pDstEA) = asULong(pSrcEA);          goto skipstatus;
                    case Opcode::MOVE_Q: asUQuad(pDstEA) = asUQuad(pSrcEA);          goto skipstatus;
                    case Opcode::ADD_L:  asLong(pDstEA) += asLong(pSrcEA);           goto skipstatus;
                    case Opcode::ADD_Q:  asQuad(pDstEA) += asQuad(pSrcEA);           goto skipstatus;
                    case Opcode::SUB_L:  asLong(pDstEA) -= asLong(pSrcEA);           goto skipstatus;
                    case Opcode::SUB_Q:  asQuad(pDstEA) -= asQuad(pSrcEA);           goto skipstatus;

                    default:
                        todo();
                        break;
                }

                break;
            }

            // Float register to register fast path prefix
            case Opcode::FLT_R2R: {
                unpackFPRPair();

                switch (*puProgramCounter++) {
                    case Opcode::FMOVE_S:   asLong(pDstEA)    = asLong(pSrcEA);     goto skipstatus;
                    case Opcode::FMOVE_D:   asQuad(pDstEA)    = asQuad(pSrcEA);     goto skipstatus;
                    case Opcode::FNEG_S:    asSingle(pDstEA)  = -asSingle(pSrcEA);  goto skipstatus;
                    case Opcode::FNEG_D:    asDouble(pDstEA)  = -asDouble(pSrcEA);  goto skipstatus;
                    case Opcode::FADD_S:    asSingle(pDstEA) += asSingle(pSrcEA);   goto skipstatus;
                    case Opcode::FADD_D:    asDouble(pDstEA) += asDouble(pSrcEA);   goto skipstatus;
                    case Opcode::FSUB_S:    asSingle(pDstEA) -= asSingle(pSrcEA);   goto skipstatus;
                    case Opcode::FSUB_D:    asDouble(pDstEA) -= asDouble(pSrcEA);   goto skipstatus;
                    case Opcode::FMUL_S:    asSingle(pDstEA) *= asSingle(pSrcEA);   goto skipstatus;
                    case Opcode::FMUL_D:    asDouble(pDstEA) *= asDouble(pSrcEA);   goto skipstatus;
                    case Opcode::FDIV_S:    asSingle(pDstEA) /= asSingle(pSrcEA);   goto skipstatus;
                    case Opcode::FDIV_D:    asDouble(pDstEA) /= asDouble(pSrcEA);   goto skipstatus;
                    default:
                        todo();
                        break;
                }
                break;
            }

            // Control
            case Opcode::HCF: {
                // This opcode expects 0xFF followed by a byte indicating which function to call.
                // If the second byte is not 0xFF we assume we aren't in a valid bytecode stream
                // any more.
                uint8 uNext = *puProgramCounter++;
                if (uNext != 0xFF) {
                    eStatus = CAUGHT_FIRE;
                } else {
                    // Get the function ID and call it. The function is expected to return a valid
                    // status code we can set.
                    uNext = *puProgramCounter++;
                    if (uNext < uNumHCFVectors) {
                        eStatus = pcHCFVectors[uNext]();
                    } else {
                        eStatus = UNKNOWN_HOST_CALL;
                    }
                }
                break;
            }

            case Opcode::BRA_B: branchByte(); goto skipstatus;
            case Opcode::BRA:   branchLong(); goto skipstatus;

            case Opcode::BSR_B: {
                int8 iShortDisplacement = (int8)*puProgramCounter++;
                pushProgramCounter();
                puProgramCounter += iShortDisplacement;
                ++iCallDepth;
                goto skipstatus;
            }

            case Opcode::BSR: {
                readDisplacement();
                pushProgramCounter();
                puProgramCounter += iDisplacement;
                ++iCallDepth;
                goto skipstatus;
            }

            case Opcode::JMP: {
                monadic(SIZE_QUAD);
                puProgramCounter = (uint8 const*)pDstEA;
                break;
            }

            case Opcode::JSR: {
                monadic(SIZE_QUAD);
                pushProgramCounter();
                puProgramCounter = (uint8 const*)pDstEA;
                ++iCallDepth;
                break;
            }

            case Opcode::RTS: {
                if (0 == --iCallDepth) {
                    eStatus = COMPLETED;
                } else {
                    popProgramCounter();
                    goto skipstatus;
                }
                break;
            }

            // Branch if zero
            case Opcode::BIZ_B:  monadic(SIZE_BYTE); readDisplacement(); bcc(!asByte(pDstEA));       break;
            case Opcode::BIZ_W:  monadic(SIZE_WORD); readDisplacement(); bcc(!asWord(pDstEA));       break;
            case Opcode::BIZ_L:  monadic(SIZE_LONG); readDisplacement(); bcc(!asLong(pDstEA));       break;
            case Opcode::BIZ_Q:  monadic(SIZE_QUAD); readDisplacement(); bcc(!asQuad(pDstEA));       break;
            case Opcode::FBIZ_S: monadic(SIZE_LONG); readDisplacement(); bcc(!asSingle(pDstEA));     break;
            case Opcode::FBIZ_D: monadic(SIZE_QUAD); readDisplacement(); bcc(!asDouble(pDstEA));     break;

            // Branch if not zero
            case Opcode::BNZ_B:  monadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pDstEA));        break;
            case Opcode::BNZ_W:  monadic(SIZE_WORD); readDisplacement(); bcc(asWord(pDstEA));        break;
            case Opcode::BNZ_L:  monadic(SIZE_LONG); readDisplacement(); bcc(asLong(pDstEA));        break;
            case Opcode::BNZ_Q:  monadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pDstEA));        break;
            case Opcode::FBNZ_S: monadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pDstEA));      break;
            case Opcode::FBNZ_D: monadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pDstEA));      break;

            // Branch if minus
            case Opcode::BMI_B:  monadic(SIZE_BYTE); readDisplacement(); bcc(0 > asByte(pDstEA));    break;
            case Opcode::BMI_W:  monadic(SIZE_WORD); readDisplacement(); bcc(0 > asWord(pDstEA));    break;
            case Opcode::BMI_L:  monadic(SIZE_LONG); readDisplacement(); bcc(0 > asLong(pDstEA));    break;
            case Opcode::BMI_Q:  monadic(SIZE_QUAD); readDisplacement(); bcc(0 > asQuad(pDstEA));    break;
            case Opcode::FBMI_S: monadic(SIZE_LONG); readDisplacement(); bcc(0 > asSingle(pDstEA));  break;
            case Opcode::FBMI_D: monadic(SIZE_QUAD); readDisplacement(); bcc(0 > asDouble(pDstEA));  break;

            // Branch if plus
            case Opcode::BPL_B:  monadic(SIZE_BYTE); readDisplacement(); bcc(0 < asByte(pDstEA));    break;
            case Opcode::BPL_W:  monadic(SIZE_WORD); readDisplacement(); bcc(0 < asWord(pDstEA));    break;
            case Opcode::BPL_L:  monadic(SIZE_LONG); readDisplacement(); bcc(0 < asLong(pDstEA));    break;
            case Opcode::BPL_Q:  monadic(SIZE_QUAD); readDisplacement(); bcc(0 < asQuad(pDstEA));    break;
            case Opcode::FBPL_S: monadic(SIZE_LONG); readDisplacement(); bcc(0 < asSingle(pDstEA));  break;
            case Opcode::FBPL_D: monadic(SIZE_QUAD); readDisplacement(); bcc(0 < asDouble(pDstEA));  break;

            // Branch if source < dest
            case Opcode::BLT_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   <  asByte(pDstEA));   break;
            case Opcode::BLT_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   <  asWord(pDstEA));   break;
            case Opcode::BLT_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   <  asLong(pDstEA));   break;
            case Opcode::BLT_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   <  asQuad(pDstEA));   break;
            case Opcode::FBLT_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) <  asSingle(pDstEA)); break;
            case Opcode::FBLT_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) <  asDouble(pDstEA)); break;

            // Branch if source <= dest
            case Opcode::BLE_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   <= asByte(pDstEA));   break;
            case Opcode::BLE_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   <= asWord(pDstEA));   break;
            case Opcode::BLE_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   <= asLong(pDstEA));   break;
            case Opcode::BLE_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   <= asQuad(pDstEA));   break;
            case Opcode::FBLE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) <= asSingle(pDstEA)); break;
            case Opcode::FBLE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) <= asDouble(pDstEA)); break;

            // Branch if source == dest
            case Opcode::BEQ_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   == asByte(pDstEA));   break;
            case Opcode::BEQ_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   == asWord(pDstEA));   break;
            case Opcode::BEQ_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   == asLong(pDstEA));   break;
            case Opcode::BEQ_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   == asQuad(pDstEA));   break;
            case Opcode::FBEQ_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) == asSingle(pDstEA)); break;
            case Opcode::FBEQ_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) == asDouble(pDstEA)); break;

            // Branch if source >= dest
            case Opcode::BGE_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   >= asByte(pDstEA));   break;
            case Opcode::BGE_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   >= asWord(pDstEA));   break;
            case Opcode::BGE_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   >= asLong(pDstEA));   break;
            case Opcode::BGE_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   >= asQuad(pDstEA));   break;
            case Opcode::FBGE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) >= asSingle(pDstEA)); break;
            case Opcode::FBGE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) >= asDouble(pDstEA)); break;

            // Branch if source > dest
            case Opcode::BGT_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   >  asByte(pDstEA));   break;
            case Opcode::BGT_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   >  asWord(pDstEA));   break;
            case Opcode::BGT_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   >  asLong(pDstEA));   break;
            case Opcode::BGT_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   >  asQuad(pDstEA));   break;
            case Opcode::FBGT_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) >  asSingle(pDstEA)); break;
            case Opcode::FBGT_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) >  asDouble(pDstEA)); break;

            // Branch if source != dest
            case Opcode::BNE_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   != asByte(pDstEA));   break;
            case Opcode::BNE_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   != asWord(pDstEA));   break;
            case Opcode::BNE_L:  dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   != asLong(pDstEA));   break;
            case Opcode::BNE_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   != asQuad(pDstEA));   break;
            case Opcode::FBNE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) != asSingle(pDstEA)); break;
            case Opcode::FBNE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) != asDouble(pDstEA)); break;

            // Branch if bit set
            case Opcode::BBS_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pDstEA)   & asBitPos(pSrcEA, 7));   break;
            case Opcode::BBS_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pDstEA)   & asBitPos(pSrcEA, 15));  break;
            case Opcode::BBS_L:  dyadic(SIZE_WORD); readDisplacement(); bcc(asULong(pDstEA)   & asBitPos(pSrcEA, 31));  break;
            case Opcode::BBS_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pDstEA)   & asBitPos(pSrcEA, 63));  break;

            // Branch if bit clear
            case Opcode::BBC_B:  dyadic(SIZE_BYTE); readDisplacement(); bcc(!(asUByte(pDstEA) & asBitPos(pSrcEA, 7)));  break;
            case Opcode::BBC_W:  dyadic(SIZE_WORD); readDisplacement(); bcc(!(asUWord(pDstEA) & asBitPos(pSrcEA, 15))); break;
            case Opcode::BBC_L:  dyadic(SIZE_WORD); readDisplacement(); bcc(!(asULong(pDstEA) & asBitPos(pSrcEA, 31))); break;
            case Opcode::BBC_Q:  dyadic(SIZE_QUAD); readDisplacement(); bcc(!(asUQuad(pDstEA) & asBitPos(pSrcEA, 63))); break;

            // Decrement and branch if not zero
            case Opcode::DBNZ:   monadic(SIZE_WORD);readDisplacement(); bcc(--asULong(pDstEA)); break;

            // DataMove - move
            case Opcode::MOVE_B: dyadic(SIZE_BYTE); asUByte(pDstEA) = asUByte(pSrcEA); break;
            case Opcode::MOVE_W: dyadic(SIZE_WORD); asUWord(pDstEA) = asUWord(pSrcEA); break;
            case Opcode::MOVE_L: dyadic(SIZE_LONG); asULong(pDstEA) = asULong(pSrcEA); break;
            case Opcode::MOVE_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) = asUQuad(pSrcEA); break;

            // GPR save/restore
            case Opcode::SAVEM: {
                uint8 uEAMode = *puProgramCounter++;
                readMask();
                saveRegisters(uMask, uEAMode);
                break;
            }

            case Opcode::LOADM: {
                readMask();
                uint8 uEAMode = *puProgramCounter++;
                restoreRegisters(uMask, uEAMode);
                break;
            }

            // Integer/Float interconversion
            case Opcode::FMOVEB_S: dyadic(SIZE_LONG); asSingle(pDstEA) = (float32)asByte(pSrcEA);   break;
            case Opcode::FMOVEB_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = (float64)asByte(pSrcEA);   break;
            case Opcode::FMOVEW_S: dyadic(SIZE_LONG); asSingle(pDstEA) = (float32)asWord(pSrcEA);   break;
            case Opcode::FMOVEW_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = (float64)asWord(pSrcEA);   break;
            case Opcode::FMOVEL_S: dyadic(SIZE_LONG); asSingle(pDstEA) = (float32)asLong(pSrcEA);   break;
            case Opcode::FMOVEL_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = (float64)asLong(pSrcEA);   break;
            case Opcode::FMOVEQ_S: dyadic(SIZE_LONG); asSingle(pDstEA) = (float32)asQuad(pSrcEA);   break;
            case Opcode::FMOVEQ_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = (float64)asQuad(pSrcEA);   break;
            case Opcode::FMOVES_L: dyadic(SIZE_LONG); asLong(pDstEA)   = (int32)asSingle(pSrcEA);   break;
            case Opcode::FMOVES_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)   = (int64)asSingle(pSrcEA);   break;
            case Opcode::FMOVES_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = (float64)asSingle(pSrcEA); break;
            case Opcode::FMOVED_L: dyadic(SIZE_LONG); asLong(pDstEA)   = (int32)asDouble(pSrcEA);   break;
            case Opcode::FMOVED_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)   = (int64)asDouble(pSrcEA);   break;
            case Opcode::FMOVED_S: dyadic(SIZE_LONG); asSingle(pDstEA) = (float32)asDouble(pSrcEA); break;

            // FPR save/restore
            case Opcode::FMOVE_S:  dyadic(SIZE_LONG); asLong(pDstEA) = asLong(pSrcEA);  break;
            case Opcode::FMOVE_D:  dyadic(SIZE_QUAD); asQuad(pDstEA) = asQuad(pSrcEA);  break;

            // DataMove - clr
            case Opcode::CLR_B:    monadic(SIZE_BYTE); asUByte(pDstEA) = 0; break;
            case Opcode::CLR_W:    monadic(SIZE_WORD); asUWord(pDstEA) = 0; break;
            case Opcode::CLR_L:    monadic(SIZE_LONG); asULong(pDstEA) = 0; break;
            case Opcode::CLR_Q:    monadic(SIZE_QUAD); asUQuad(pDstEA) = 0; break;

            case Opcode::EXG: {
                unpackGPRPair();
                uint64 uTemp    = asUQuad(pSrcEA);
                asUQuad(pSrcEA) = asUQuad(pDstEA);
                asUQuad(pDstEA) = uTemp;
                break;
            }

            case Opcode::FEXG: {
                unpackFPRPair();
                uint64 uTemp    = asUQuad(pSrcEA);
                asUQuad(pSrcEA) = asUQuad(pDstEA);
                asUQuad(pDstEA) = uTemp;
                break;
            }

            case Opcode::SWAP: {
                unpackGPRPair();
                uint32 uTemp    = (uint32)asLong(pSrcEA);
                asULong(pDstEA) = uTemp >> 16 | uTemp << 16;
                break;
            }

            case Opcode::SWAP_L: {
                unpackGPRPair();
                asULong(pDstEA) = __builtin_bswap32(asULong(pSrcEA));
                break;
            }
            case Opcode::SWAP_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) = __builtin_bswap64(asUQuad(pSrcEA));
                break;
            }

            case Opcode::LINK:
            case Opcode::UNLK:   todo();
            case Opcode::LEA: {
                dyadic(SIZE_QUAD);
                asUQuad(pDstEA) = (uint64)pSrcEA;
                break;
            }
            case Opcode::PEA: {
                monadic(SIZE_QUAD);
                aoGPR[GPRegister::SP].puByte -= sizeof(uint64);
                *(aoGPR[GPRegister::SP].puQuad) = (uint64)pSrcEA;
                break;
            }

            // Logical
            case Opcode::AND_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) &=   asUByte(pSrcEA);       break;
            case Opcode::AND_W:  dyadic(SIZE_WORD); asUWord(pDstEA) &=   asUWord(pSrcEA);       break;
            case Opcode::AND_L:  dyadic(SIZE_LONG); asULong(pDstEA) &=   asULong(pSrcEA);       break;
            case Opcode::AND_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) &=   asUQuad(pSrcEA);       break;
            case Opcode::OR_B:   dyadic(SIZE_BYTE); asUByte(pDstEA) |=   asUByte(pSrcEA);       break;
            case Opcode::OR_W:   dyadic(SIZE_WORD); asUWord(pDstEA) |=   asUWord(pSrcEA);       break;
            case Opcode::OR_L:   dyadic(SIZE_LONG); asULong(pDstEA) |=   asULong(pSrcEA);       break;
            case Opcode::OR_Q:   dyadic(SIZE_QUAD); asUQuad(pDstEA) |=   asUQuad(pSrcEA);       break;
            case Opcode::EOR_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) ^=   asUByte(pSrcEA);       break;
            case Opcode::EOR_W:  dyadic(SIZE_WORD); asUWord(pDstEA) ^=   asUWord(pSrcEA);       break;
            case Opcode::EOR_L:  dyadic(SIZE_LONG); asULong(pDstEA) ^=   asULong(pSrcEA);       break;
            case Opcode::EOR_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) ^=   asUQuad(pSrcEA);       break;
            case Opcode::NOT_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) =   ~asUByte(pSrcEA);       break;
            case Opcode::NOT_W:  dyadic(SIZE_WORD); asUWord(pDstEA) =   ~asUWord(pSrcEA);       break;
            case Opcode::NOT_L:  dyadic(SIZE_LONG); asULong(pDstEA) =   ~asULong(pSrcEA);       break;
            case Opcode::NOT_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) =   ~asUQuad(pSrcEA);       break;
            case Opcode::LSL_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) <<= (asUByte(pSrcEA) & 7);  break;
            case Opcode::LSL_W:  dyadic(SIZE_WORD); asUWord(pDstEA) <<= (asUByte(pSrcEA) & 15); break;
            case Opcode::LSL_L:  dyadic(SIZE_LONG); asULong(pDstEA) <<= (asUByte(pSrcEA) & 31); break;
            case Opcode::LSL_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) <<= (asUByte(pSrcEA) & 63); break;
            case Opcode::LSR_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) >>= (asUByte(pSrcEA) & 7);  break;
            case Opcode::LSR_W:  dyadic(SIZE_WORD); asUWord(pDstEA) >>= (asUByte(pSrcEA) & 15); break;
            case Opcode::LSR_L:  dyadic(SIZE_LONG); asULong(pDstEA) >>= (asUByte(pSrcEA) & 31); break;
            case Opcode::LSR_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) >>= (asUByte(pSrcEA) & 63); break;
            case Opcode::ROL_B:  dyadic(SIZE_BYTE); rolByte((uint8*)pDstEA,  asUByte(pSrcEA));  break;
            case Opcode::ROL_W:  dyadic(SIZE_WORD); rolWord((uint16*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::ROL_L:  dyadic(SIZE_LONG); rolLong((uint32*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::ROL_Q:  dyadic(SIZE_QUAD); rolQuad((uint64*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::ROR_B:  dyadic(SIZE_BYTE); rorByte((uint8*)pDstEA,  asUByte(pSrcEA));  break;
            case Opcode::ROR_W:  dyadic(SIZE_WORD); rorWord((uint16*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::ROR_L:  dyadic(SIZE_LONG); rorLong((uint32*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::ROR_Q:  dyadic(SIZE_QUAD); rorQuad((uint64*)pDstEA, asUByte(pSrcEA));  break;
            case Opcode::BCLR_B: dyadic(SIZE_BYTE); asUByte(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BCLR_W: dyadic(SIZE_WORD); asUWord(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BCLR_L: dyadic(SIZE_LONG); asULong(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BCLR_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 63)); break;
            case Opcode::BSET_B: dyadic(SIZE_BYTE); asUByte(pDstEA) |=  (1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BSET_W: dyadic(SIZE_WORD); asUWord(pDstEA) |=  (1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BSET_L: dyadic(SIZE_LONG); asULong(pDstEA) |=  (1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BSET_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) |=  (1 << (asUByte(pSrcEA) & 63)); break;

            case Opcode::BFFFO: {
                unpackGPRPair();
                uint64 pTemp = asUQuad(pSrcEA);
                asUQuad(pDstEA) = pTemp ? (63 - __builtin_clzll(pTemp)) : ~pTemp;
                break;
            }
            case Opcode::BFCNT: {
                unpackGPRPair();
                asUQuad(pDstEA) = __builtin_popcountll(asUQuad(pSrcEA));
                break;
            }

            // Arithmetic
            case Opcode::EXTB_W: dyadic(SIZE_WORD); asWord(pDstEA)    = (int16)asByte(pSrcEA);   break;
            case Opcode::EXTB_L: dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asByte(pSrcEA);   break;
            case Opcode::EXTB_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asByte(pSrcEA);   break;
            case Opcode::EXTW_L: dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asByte(pSrcEA);   break;
            case Opcode::EXTW_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asWord(pSrcEA);   break;
            case Opcode::EXTL_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asLong(pSrcEA);   break;
            case Opcode::ASL_B:  dyadic(SIZE_BYTE); asByte(pDstEA)   <<= (asUByte(pSrcEA) & 7);  break;
            case Opcode::ASL_W:  dyadic(SIZE_WORD); asWord(pDstEA)   <<= (asUByte(pSrcEA) & 15); break;
            case Opcode::ASL_L:  dyadic(SIZE_LONG); asLong(pDstEA)   <<= (asUByte(pSrcEA) & 31); break;
            case Opcode::ASL_Q:  dyadic(SIZE_QUAD); asQuad(pDstEA)   <<= (asUByte(pSrcEA) & 63); break;
            case Opcode::ASR_B:  dyadic(SIZE_BYTE); asByte(pDstEA)   >>= (asUByte(pSrcEA) & 7);  break;
            case Opcode::ASR_W:  dyadic(SIZE_WORD); asWord(pDstEA)   >>= (asUByte(pSrcEA) & 15); break;
            case Opcode::ASR_L:  dyadic(SIZE_LONG); asLong(pDstEA)   >>= (asUByte(pSrcEA) & 31); break;
            case Opcode::ASR_Q:  dyadic(SIZE_QUAD); asQuad(pDstEA)   >>= (asUByte(pSrcEA) & 63); break;
            case Opcode::ADD_B:  dyadic(SIZE_BYTE); asByte(pDstEA)   +=   asByte(pSrcEA);        break;
            case Opcode::ADD_W:  dyadic(SIZE_WORD); asWord(pDstEA)   +=   asWord(pSrcEA);        break;
            case Opcode::ADD_L:  dyadic(SIZE_LONG); asLong(pDstEA)   +=   asLong(pSrcEA);        break;
            case Opcode::ADD_Q:  dyadic(SIZE_QUAD); asQuad(pDstEA)   +=   asQuad(pSrcEA);        break;
            case Opcode::FADD_S: dyadic(SIZE_LONG); asSingle(pDstEA) +=   asSingle(pSrcEA);      break;
            case Opcode::FADD_D: dyadic(SIZE_QUAD); asDouble(pDstEA) +=   asDouble(pSrcEA);      break;
            case Opcode::SUB_B:  dyadic(SIZE_BYTE); asByte(pDstEA)   -=   asByte(pSrcEA);        break;
            case Opcode::SUB_W:  dyadic(SIZE_WORD); asWord(pDstEA)   -=   asWord(pSrcEA);        break;
            case Opcode::SUB_L:  dyadic(SIZE_LONG); asLong(pDstEA)   -=   asLong(pSrcEA);        break;
            case Opcode::SUB_Q:  dyadic(SIZE_QUAD); asQuad(pDstEA)   -=   asQuad(pSrcEA);        break;
            case Opcode::FSUB_S: dyadic(SIZE_LONG); asSingle(pDstEA) -=   asSingle(pSrcEA);      break;
            case Opcode::FSUB_D: dyadic(SIZE_QUAD); asDouble(pDstEA) -=   asDouble(pSrcEA);      break;
            case Opcode::NEG_B:  dyadic(SIZE_BYTE); asByte(pDstEA)    =  -asByte(pSrcEA);        break;
            case Opcode::NEG_W:  dyadic(SIZE_WORD); asWord(pDstEA)    =  -asWord(pSrcEA);        break;
            case Opcode::NEG_L:  dyadic(SIZE_LONG); asLong(pDstEA)    =  -asLong(pSrcEA);        break;
            case Opcode::NEG_Q:  dyadic(SIZE_QUAD); asQuad(pDstEA)    =  -asQuad(pSrcEA);        break;
            case Opcode::FNEG_S: dyadic(SIZE_LONG); asSingle(pDstEA)  =  -asSingle(pSrcEA);      break;
            case Opcode::FNEG_D: dyadic(SIZE_QUAD); asDouble(pDstEA)  =  -asDouble(pSrcEA);      break;
            case Opcode::MULS_B: dyadic(SIZE_BYTE); asByte(pDstEA)   *=   asByte(pSrcEA);        break;
            case Opcode::MULS_W: dyadic(SIZE_WORD); asWord(pDstEA)   *=   asWord(pSrcEA);        break;
            case Opcode::MULS_L: dyadic(SIZE_LONG); asLong(pDstEA)   *=   asLong(pSrcEA);        break;
            case Opcode::MULS_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)   *=   asQuad(pSrcEA);        break;
            case Opcode::MULU_B: dyadic(SIZE_BYTE); asUByte(pDstEA)  *=   asUByte(pSrcEA);       break;
            case Opcode::MULU_W: dyadic(SIZE_WORD); asUWord(pDstEA)  *=   asUWord(pSrcEA);       break;
            case Opcode::MULU_L: dyadic(SIZE_LONG); asULong(pDstEA)  *=   asULong(pSrcEA);       break;
            case Opcode::MULU_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA)  *=   asUQuad(pSrcEA);       break;
            case Opcode::FMUL_S: dyadic(SIZE_LONG); asSingle(pDstEA) *=   asSingle(pSrcEA);      break;
            case Opcode::FMUL_D: dyadic(SIZE_QUAD); asDouble(pDstEA) *=   asDouble(pSrcEA);      break;
            case Opcode::DIVS_B: dyadic(SIZE_BYTE); asByte(pDstEA)   /=   asByte(pSrcEA);        break;
            case Opcode::DIVS_W: dyadic(SIZE_WORD); asWord(pDstEA)   /=   asWord(pSrcEA);        break;
            case Opcode::DIVS_L: dyadic(SIZE_LONG); asLong(pDstEA)   /=   asLong(pSrcEA);        break;
            case Opcode::DIVS_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)   /=   asQuad(pSrcEA);        break;
            case Opcode::DIVU_B: dyadic(SIZE_BYTE); asUByte(pDstEA)  /=   asUByte(pSrcEA);       break;
            case Opcode::DIVU_W: dyadic(SIZE_WORD); asUWord(pDstEA)  /=   asUWord(pSrcEA);       break;
            case Opcode::DIVU_L: dyadic(SIZE_LONG); asULong(pDstEA)  /=   asULong(pSrcEA);       break;
            case Opcode::DIVU_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA)  /=   asUQuad(pSrcEA);       break;
            case Opcode::FDIV_S: dyadic(SIZE_LONG); asSingle(pDstEA) /=   asSingle(pSrcEA);      break;
            case Opcode::FDIV_D: dyadic(SIZE_QUAD); asDouble(pDstEA) /=   asDouble(pSrcEA);      break;

            case Opcode::FMOD_S: {
                dyadic(SIZE_LONG);
                float32 f = asSingle(pDstEA);
                asSingle(pDstEA) = std::fmod(f, asSingle(pSrcEA));
                break;
            }
            case Opcode::FMOD_D: {
                dyadic(SIZE_QUAD);
                float64 f = asDouble(pDstEA);
                asDouble(pDstEA) = std::fmod(f, asDouble(pSrcEA));
                break;
            }

            case Opcode::FABS_S:    dyadic(SIZE_LONG); asSingle(pDstEA) = std::fabs(asSingle(pSrcEA)); break;
            case Opcode::FABS_D:    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::fabs(asDouble(pSrcEA)); break;
            case Opcode::FSQRT_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::sqrt(asSingle(pSrcEA)); break;
            case Opcode::FSQRT_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::sqrt(asDouble(pSrcEA)); break;
            case Opcode::FACOS_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::acos(asSingle(pSrcEA)); break;
            case Opcode::FACOS_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::acos(asDouble(pSrcEA)); break;
            case Opcode::FASIN_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::asin(asSingle(pSrcEA)); break;
            case Opcode::FASIN_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::asin(asDouble(pSrcEA)); break;
            case Opcode::FATAN_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::atan(asSingle(pSrcEA)); break;
            case Opcode::FATAN_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::atan(asDouble(pSrcEA)); break;
            case Opcode::FCOS_S:    dyadic(SIZE_LONG); asSingle(pDstEA) = std::cos(asSingle(pSrcEA));  break;
            case Opcode::FCOS_D:    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::cos(asDouble(pSrcEA));  break;
            case Opcode::FSIN_S:    dyadic(SIZE_LONG); asSingle(pDstEA) = std::sin(asSingle(pSrcEA));  break;
            case Opcode::FSIN_D:    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::sin(asDouble(pSrcEA));  break;
            case Opcode::FSINCOS_S:
            case Opcode::FSINCOS_D: todo();
            case Opcode::FTAN_S:    dyadic(SIZE_LONG); asSingle(pDstEA) = std::tan(asSingle(pSrcEA));  break;
            case Opcode::FTAN_D:    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::tan(asDouble(pSrcEA));  break;
            case Opcode::FETOX_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::exp(asSingle(pSrcEA));  break;
            case Opcode::FETOX_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::exp(asDouble(pSrcEA));  break;
            case Opcode::FLOGN_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::log(asSingle(pSrcEA));  break;
            case Opcode::FLOGN_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::log(asDouble(pSrcEA));  break;
            case Opcode::FLOG2_S:   dyadic(SIZE_LONG); asSingle(pDstEA) = std::log2(asSingle(pSrcEA)); break;
            case Opcode::FLOG2_D:   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::log2(asDouble(pSrcEA)); break;
            case Opcode::FTWOTOX_S: dyadic(SIZE_LONG); asSingle(pDstEA) = std::exp2(asSingle(pSrcEA)); break;
            case Opcode::FTWOTOX_D: dyadic(SIZE_QUAD); asDouble(pDstEA) = std::exp2(asDouble(pSrcEA)); break;

            case Opcode::FBIF_S:
            case Opcode::FBIF_D: // todo

            default:
                todo();
        }
    }

    Nanoseconds::Value uElapsed = Nanoseconds::mark() - uStart;

    float64 fMIPS = (1000.0 * uInstructionCount) / (float64)uElapsed;

    std::fprintf(
        stderr,
        "Total instructions %lu in %lu nanoseconds, %.2f MIPS\n",
        uInstructionCount,
        uElapsed,
        fMIPS
    );
}

}}

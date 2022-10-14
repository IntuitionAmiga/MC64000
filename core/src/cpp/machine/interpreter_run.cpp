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
#include <cmath>
#include <machine/interpreter.hpp>
#include <machine/timing.hpp>
#include <bytecode/opcode.hpp>
#include <machine/inline.hpp>
#include <machine/gnarly.hpp>

namespace MC64K::Machine {

/**
 * Deal with host call operations.
 */
void NOINLINE Interpreter::handleHost() {
    // Get the function ID and call it. The function is expected to return a valid
    // status code we can set.
    uint8 uNext = *puProgramCounter++;
    if (uNext < uNumHCFVectors) {
        uint8 const* volatile pNext = puProgramCounter + 1;
        eStatus = pcHCFVectors[uNext](*puProgramCounter++);
        if (eStatus == INITIALISED) {
            puProgramCounter = pNext;
            eStatus = RUNNING;
        }
    } else {
        eStatus = UNKNOWN_HOST_CALL;
    }
}

/**
 * @inheritDoc
 */
void Interpreter::run() {

    using namespace MC64K::ByteCode;

    if (!puProgramCounter) {
        return;
    }

    eStatus    = RUNNING;
    int32 iCallDepth = 1;

    initDisplacement();

    initMIPSReport();

    while (RUNNING == eStatus) {

        // Fast branch back location for operations that don't change the status. If you invoke monadic() or dyadic()
        // you have to assume the status could change due to a bad EA mode.
        skip_status_check:

        updateMIPS();
        switch (*puProgramCounter++) {

            case Opcode::STOP:  eStatus = CAUGHT_FIRE; break;
            case Opcode::HOST:  handleHost();          break;
            case Opcode::BRA_B: branchByte(); goto skip_status_check;
            case Opcode::BRA:   branchLong(); goto skip_status_check;

            case Opcode::BSR_B: {
                int8 iShortDisplacement = (int8)*puProgramCounter++;
                pushProgramCounter();
                puProgramCounter += iShortDisplacement;
                ++iCallDepth;
                goto skip_status_check;
            }

            case Opcode::BSR: {
                readDisplacement();
                pushProgramCounter();
                puProgramCounter += iDisplacement;
                ++iCallDepth;
                goto skip_status_check;
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
                    goto skip_status_check;
                }
                break;
            }

            case Opcode::BMC: handleBMC(); break;
            case Opcode::BDC: handleBDC(); break;

            case Opcode::DBNZ: {
                monadic(SIZE_WORD);
                readDisplacement();
                bcc(--asULong(pDstEA));
                break;
            }

            case Opcode::R_BMC:   handleRBMC();   goto skip_status_check;
            case Opcode::R2R_BDC: handleR2RBDC(); goto skip_status_check;

            case Opcode::R_DBNZ: {
                unpackGPR();
                readDisplacement();
                bcc(--asULong(pDstEA));
                goto skip_status_check;
            }

            // DataMove - move
            // Fast Path
            case Opcode::R2R_MOVE_L: {
                unpackGPRPair();
                asULong(pDstEA) = asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_MOVE_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) = asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FMOVE_S: {
                unpackFPRPair();
                asULong(pDstEA) = asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FMOVE_D: {
                unpackFPRPair();
                asUQuad(pDstEA) = asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_CLR_L: {
                unpackGPR();
                asULong(pDstEA) = 0;
                goto skip_status_check;
            }

            case Opcode::R2R_CLR_Q: {
                unpackGPR();
                asUQuad(pDstEA) = 0;
                goto skip_status_check;
            }

            case Opcode::R2R_EXG: {
                unpackGPRPair();
                uint64 uTemp    = asUQuad(pSrcEA);
                asUQuad(pSrcEA) = asUQuad(pDstEA);
                asUQuad(pDstEA) = uTemp;
                goto skip_status_check;
            }

            case Opcode::R2R_FEXG: {
                unpackFPRPair();
                uint64 uTemp    = asUQuad(pSrcEA);
                asUQuad(pSrcEA) = asUQuad(pDstEA);
                asUQuad(pDstEA) = uTemp;
                goto skip_status_check;
            }

            case Opcode::R2R_SWAP: {
                unpackGPRPair();
                uint32 uTemp    = (uint32)asLong(pSrcEA);
                asULong(pDstEA) = uTemp >> 16 | uTemp << 16;
                goto skip_status_check;
            }

            case Opcode::R2R_SWAP_L: {
                unpackGPRPair();
                asULong(pDstEA) = __builtin_bswap32(asULong(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_SWAP_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) = __builtin_bswap64(asUQuad(pSrcEA));
                goto skip_status_check;
            }


            case Opcode::R2R_AND_L: {
                unpackGPRPair();
                asULong(pDstEA) &= asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_AND_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) &= asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_OR_L: {
                unpackGPRPair();
                asULong(pDstEA) |= asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_OR_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) |= asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EOR_L: {
                unpackGPRPair();
                asULong(pDstEA) ^= asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EOR_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) ^= asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_NOT_L: {
                unpackGPRPair();
                asULong(pDstEA) = ~asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_NOT_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) = ~asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_LSL_L: {
                unpackGPRPair();
                asULong(pDstEA) <<= (asUByte(pSrcEA) & 31);
                goto skip_status_check;
            }

            case Opcode::R2R_LSL_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) <<= (asUByte(pSrcEA) & 63);
                goto skip_status_check;
            }

            case Opcode::R2R_LSR_L: {
                unpackGPRPair();
                asULong(pDstEA) >>= (asULong(pSrcEA) & 31);
                goto skip_status_check;
            }

            case Opcode::R2R_LSR_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) >>= (asUQuad(pSrcEA) & 63);
                goto skip_status_check;
            }


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

            case Opcode::R2R_EXTB_L: {
                unpackGPRPair();
                asLong(pDstEA) = (int32)asByte(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EXTB_Q: {
                unpackGPRPair();
                asQuad(pDstEA) = (int64)asByte(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EXTW_L: {
                unpackGPRPair();
                asLong(pDstEA) = (int32)asWord(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EXTW_Q: {
                unpackGPRPair();
                asQuad(pDstEA) = (int64)asWord(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_EXTL_Q: {
                unpackGPRPair();
                asQuad(pDstEA) = (int64)asLong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_NEG_L: {
                unpackGPRPair();
                asLong(pDstEA) = -asLong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_NEG_Q: {
                unpackGPRPair();
                asQuad(pDstEA) = -asQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FNEG_S: {
                unpackFPRPair();
                asSingle(pDstEA) = -asSingle(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FNEG_D: {
                unpackFPRPair();
                asDouble(pDstEA) = -asDouble(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FABS_S: {
                unpackFPRPair();
                asSingle(pDstEA) = std::fabs(asSingle(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_FABS_D: {
                unpackFPRPair();
                asDouble(pDstEA) = std::fabs(asDouble(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_ADD_L: {
                unpackGPRPair();
                asLong(pDstEA) += asLong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_ADD_Q: {
                unpackGPRPair();
                asQuad(pDstEA) += asQuad(pSrcEA);
                goto skip_status_check;
            }
            case Opcode::R2R_FADD_S: {
                unpackFPRPair();
                asSingle(pDstEA) += asSingle(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FADD_D: {
                unpackFPRPair();
                asDouble(pDstEA) += asDouble(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_SUB_L: {
                unpackGPRPair();
                asLong(pDstEA) -= asLong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_SUB_Q: {
                unpackGPRPair();
                asQuad(pDstEA) -= asQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FSUB_S: {
                unpackFPRPair();
                asSingle(pDstEA) -= asSingle(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FSUB_D: {
                unpackFPRPair();
                asDouble(pDstEA) -= asDouble(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_MULS_L: {
                unpackGPRPair();
                asLong(pDstEA) *= asLong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_MULS_Q: {
                unpackGPRPair();
                asQuad(pDstEA) *= asQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_MULU_L: {
                unpackGPRPair();
                asULong(pDstEA) *= asULong(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_MULU_Q: {
                unpackGPRPair();
                asUQuad(pDstEA) *= asUQuad(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FMUL_S: {
                unpackFPRPair();
                asSingle(pDstEA) *= asSingle(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FMUL_D: {
                unpackFPRPair();
                asDouble(pDstEA) *= asDouble(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FDIV_S: {
                unpackFPRPair();
                asSingle(pDstEA) /= asSingle(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FDIV_D: {
                unpackFPRPair();
                asDouble(pDstEA) /= asDouble(pSrcEA);
                goto skip_status_check;
            }

            case Opcode::R2R_FMOD_S: {
                unpackFPRPair();
                float32 f = asSingle(pDstEA);
                asSingle(pDstEA) = std::fmod(f, asSingle(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_FMOD_D: {
                unpackFPRPair();
                float64 f = asDouble(pDstEA);
                asDouble(pDstEA) = std::fmod(f, asDouble(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_FSQRT_S: {
                unpackFPRPair();
                asSingle(pDstEA) = std::sqrt(asSingle(pSrcEA));
                goto skip_status_check;
            }

            case Opcode::R2R_FSQRT_D: {
                unpackFPRPair();
                asDouble(pDstEA) = std::sqrt(asDouble(pSrcEA));
                goto skip_status_check;
            }

            // EA-based path

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
            case Opcode::FMOVEB_S:
                dyadic2(SIZE_LONG, SIZE_BYTE);
                asSingle(pDstEA) = (float32)asByte(pSrcEA);
                break;

            case Opcode::FMOVEB_D:
                dyadic2(SIZE_QUAD, SIZE_BYTE);
                asDouble(pDstEA) = (float64)asByte(pSrcEA);
                break;

            case Opcode::FMOVEW_S:
                dyadic2(SIZE_LONG, SIZE_WORD);
                asSingle(pDstEA) = (float32)asWord(pSrcEA);
                break;

            case Opcode::FMOVEW_D:
                dyadic2(SIZE_QUAD, SIZE_WORD);
                asDouble(pDstEA) = (float64)asWord(pSrcEA);
                break;

            case Opcode::FMOVEL_S:
                dyadic(SIZE_LONG);
                asSingle(pDstEA) = (float32)asLong(pSrcEA);
                break;

            case Opcode::FMOVEL_D:
                dyadic2(SIZE_QUAD, SIZE_LONG);
                asDouble(pDstEA) = (float64)asLong(pSrcEA);
                break;

            case Opcode::FMOVEQ_S:
                dyadic2(SIZE_LONG, SIZE_QUAD);
                asSingle(pDstEA) = (float32)asQuad(pSrcEA);
                break;

            case Opcode::FMOVEQ_D:
                dyadic(SIZE_QUAD);
                asDouble(pDstEA) = (float64)asQuad(pSrcEA);
                break;

            case Opcode::FMOVES_L:
                dyadic(SIZE_LONG);
                asLong(pDstEA)   = (int32)asSingle(pSrcEA);
                break;

            case Opcode::FMOVES_Q:
                dyadic2(SIZE_QUAD, SIZE_LONG);
                asQuad(pDstEA)   = (int64)asSingle(pSrcEA);
                break;

            case Opcode::FMOVES_D:
                dyadic(SIZE_QUAD);
                asDouble(pDstEA) = (float64)asSingle(pSrcEA);
                break;

            case Opcode::FMOVED_L:
                dyadic2(SIZE_LONG, SIZE_QUAD);
                asLong(pDstEA)   = (int32)asDouble(pSrcEA);
                break;

            case Opcode::FMOVED_Q:
                dyadic(SIZE_QUAD);
                asQuad(pDstEA)   = (int64)asDouble(pSrcEA);
                break;

            case Opcode::FMOVED_S:
                dyadic2(SIZE_LONG, SIZE_QUAD);
                asSingle(pDstEA) = (float32)asDouble(pSrcEA);
                break;

            // FPR save/restore
            case Opcode::FMOVE_S: {
                dyadic(SIZE_LONG);
                asULong(pDstEA) = asULong(pSrcEA);
                break;
            }
            case Opcode::FMOVE_D:
                dyadic(SIZE_QUAD);
                asQuad(pDstEA) = asQuad(pSrcEA);
                break;

            case Opcode::FINFO_S: {
                dyadic2(SIZE_BYTE, SIZE_LONG);
                asUByte(pDstEA) = classifyFloat(asSingle(pSrcEA));
                break;
            }

            case Opcode::FINFO_D: {
                dyadic2(SIZE_BYTE, SIZE_QUAD);
                asUByte(pDstEA) = classifyFloat(asDouble(pSrcEA));
                break;
            }

            // DataMove - clr
            case Opcode::CLR_B:    monadic(SIZE_BYTE); asUByte(pDstEA) = 0; break;
            case Opcode::CLR_W:    monadic(SIZE_WORD); asUWord(pDstEA) = 0; break;
            case Opcode::CLR_L:    monadic(SIZE_LONG); asULong(pDstEA) = 0; break;
            case Opcode::CLR_Q:    monadic(SIZE_QUAD); asUQuad(pDstEA) = 0; break;

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

            // Compare and set dyadic
            case Opcode::SCM: handleSMC(); break;

            // Compare and set dyadic
            case Opcode::SCD: handleSDC(); break;

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
            case Opcode::BCLR_B: dyadic(SIZE_BYTE); asUByte(pDstEA) &= (uint8)  ~(1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BCLR_W: dyadic(SIZE_WORD); asUWord(pDstEA) &= (uint16) ~(1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BCLR_L: dyadic(SIZE_LONG); asULong(pDstEA) &= (uint32) ~(1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BCLR_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) &= (uint64) ~(1 << (asUByte(pSrcEA) & 63)); break;
            case Opcode::BSET_B: dyadic(SIZE_BYTE); asUByte(pDstEA) |= (uint8)  (1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BSET_W: dyadic(SIZE_WORD); asUWord(pDstEA) |= (uint16) (1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BSET_L: dyadic(SIZE_LONG); asULong(pDstEA) |= (uint32) (1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BSET_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) |= (uint64) (1 << (asUByte(pSrcEA) & 63)); break;

            case Opcode::EXTB_W: dyadic(SIZE_WORD); asWord(pDstEA)    = (int16)asByte(pSrcEA);   break;
            case Opcode::EXTB_L: dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asByte(pSrcEA);   break;
            case Opcode::EXTB_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asByte(pSrcEA);   break;
            case Opcode::EXTW_L: dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asWord(pSrcEA);   break;
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
            case Opcode::DIVS_L: dyadic(SIZE_BYTE); asLong(pDstEA)   /=   asLong(pSrcEA);        break;
            case Opcode::DIVS_Q: dyadic(SIZE_WORD); asQuad(pDstEA)   /=   asQuad(pSrcEA);        break;
            case Opcode::MODS_L: dyadic(SIZE_LONG); asLong(pDstEA)   %=   asLong(pSrcEA);        break;
            case Opcode::MODS_Q: dyadic(SIZE_QUAD); asQuad(pDstEA)   %=   asQuad(pSrcEA);        break;
            case Opcode::DIVU_L: dyadic(SIZE_BYTE); asULong(pDstEA)  /=   asULong(pSrcEA);       break;
            case Opcode::DIVU_Q: dyadic(SIZE_WORD); asUQuad(pDstEA)  /=   asUQuad(pSrcEA);       break;
            case Opcode::MODU_L: dyadic(SIZE_LONG); asULong(pDstEA)  %=   asULong(pSrcEA);       break;
            case Opcode::MODU_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA)  %=   asUQuad(pSrcEA);       break;
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

            case Opcode::FSINCOS_S: {
                dyadic(SIZE_LONG);
                float32 f = asSingle(pSrcEA);
                asSingle(pDstEA) = std::sin(f);
                asSingle(pSrcEA) = std::cos(f);
                break;
            }
            case Opcode::FSINCOS_D: {
                dyadic(SIZE_QUAD);
                float64 f = asDouble(pSrcEA);
                asDouble(pDstEA) = std::sin(f);
                asDouble(pSrcEA) = std::cos(f);
                break;
            }

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

            case 0xF0:
                aoGPR[14].uQuad = Nanoseconds::mark();
                break;

            default:
                todo();
        }
    }

    outputMIPSReport();

}

}


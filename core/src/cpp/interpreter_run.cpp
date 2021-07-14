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
#include <cstdio>

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

#define monadic(size) \
    eOperationSize = size; \
    pDstEA = decodeEffectiveAddress(); \

#define dyadic(size) \
    eOperationSize = size; \
    pDstEA = decodeEffectiveAddress(); \
    pSrcEA = decodeEffectiveAddress(); \

#define asByte(ea) *((int8*)(ea))
#define asWord(ea) *((int16*)(ea))
#define asLong(ea) *((int32*)(ea))
#define asQuad(ea) *((int64*)(ea))
#define asUByte(ea) *((uint8*)(ea))
#define asUWord(ea) *((uint16*)(ea))
#define asULong(ea) *((uint32*)(ea))
#define asUQuad(ea) *((uint64*)(ea))
#define asSingle(ea) *((float32*)(ea))
#define asDouble(ea) *((float64*)(ea))

// Nasty macro that is here to be inlined / improved
#define readDisplacement() \
    uBytes[0] = *pProgramCounter++; \
    uBytes[1] = *pProgramCounter++; \
    uBytes[2] = *pProgramCounter++; \
    uBytes[3] = *pProgramCounter++; \

#define bcc(c) if (c) pProgramCounter += iDisplacement;

/**
 *
 */
void StaticInterpreter::run() {
    if (!pProgramCounter) {
        return;
    }

    eStatus    = RUNNING;
    iCallDepth = 1;

    // Union used to read 32-bit values from the bytecode into a useable value.
    union {
        int32 iDisplacement;
        uint8 uBytes[4];
    };

    uint64 uInstructionCount = 0;
    NanoTime::Value uStart = NanoTime::mark();

    while (RUNNING == eStatus) {
        ++uInstructionCount;
        switch (*pProgramCounter++) {
            // Control
            case Opcode::HCF:
            case Opcode::BRA_B:
            case Opcode::BRA:
            case Opcode::BSR_B:
                ++iCallDepth;
                break;
            case Opcode::BSR:
                ++iCallDepth;
                break;

            case Opcode::JMP:
                break;

            case Opcode::JSR:
                ++iCallDepth;
                break;

            case Opcode::RTS:
                if (0 == --iCallDepth) {
                    eStatus = COMPLETED;
                }
                break;

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

            case Opcode::BBS_B:
            case Opcode::BBS_W:
            case Opcode::BBS_L:
            case Opcode::BBS_Q:
            case Opcode::BBC_B:
            case Opcode::BBC_W:
            case Opcode::BBC_L:
            case Opcode::BBC_Q:
            case Opcode::DBNZ:

            // DataMove - move
            case Opcode::MOVE_B: dyadic(SIZE_BYTE); asUByte(pDstEA) = asUByte(pSrcEA); break;
            case Opcode::MOVE_W: dyadic(SIZE_WORD); asUWord(pDstEA) = asUWord(pSrcEA); break;
            case Opcode::MOVE_L: dyadic(SIZE_LONG); asULong(pDstEA) = asULong(pSrcEA); break;
            case Opcode::MOVE_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) = asUQuad(pSrcEA); break;

            case Opcode::SAVEM:
            case Opcode::LOADM:
            case Opcode::FMOVEB_S:
            case Opcode::FMOVEB_D:
            case Opcode::FMOVEW_S:
            case Opcode::FMOVEW_D:
            case Opcode::FMOVEL_S:
            case Opcode::FMOVEL_D:
            case Opcode::FMOVEQ_S:
            case Opcode::FMOVEQ_D:
            case Opcode::FMOVES_L:
            case Opcode::FMOVES_Q:
            case Opcode::FMOVES_D:
            case Opcode::FMOVED_L:
            case Opcode::FMOVED_Q:
            case Opcode::FMOVED_S:
            case Opcode::FSAVEM:
            case Opcode::FLOADM:

            // DataMove - clr
            case Opcode::CLR_B: monadic(SIZE_BYTE); asUByte(pDstEA) = 0; break;
            case Opcode::CLR_W: monadic(SIZE_WORD); asUWord(pDstEA) = 0; break;
            case Opcode::CLR_L: monadic(SIZE_LONG); asULong(pDstEA) = 0; break;
            case Opcode::CLR_Q: monadic(SIZE_QUAD); asUQuad(pDstEA) = 0; break;

            case Opcode::EXG:
            case Opcode::FEXG:
            case Opcode::SWAP:
            case Opcode::SWAP_L:
            case Opcode::SWAP_Q:
            case Opcode::LINK:
            case Opcode::UNLK:
            case Opcode::LEA:
            case Opcode::PEA:

            // Logical
            case Opcode::AND_B: dyadic(SIZE_BYTE); asUByte(pDstEA) &=   asUByte(pSrcEA);       break;
            case Opcode::AND_W: dyadic(SIZE_WORD); asUWord(pDstEA) &=   asUWord(pSrcEA);       break;
            case Opcode::AND_L: dyadic(SIZE_LONG); asULong(pDstEA) &=   asULong(pSrcEA);       break;
            case Opcode::AND_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) &=   asUQuad(pSrcEA);       break;
            case Opcode::OR_B:  dyadic(SIZE_BYTE); asUByte(pDstEA) |=   asUByte(pSrcEA);       break;
            case Opcode::OR_W:  dyadic(SIZE_WORD); asUWord(pDstEA) |=   asUWord(pSrcEA);       break;
            case Opcode::OR_L:  dyadic(SIZE_LONG); asULong(pDstEA) |=   asULong(pSrcEA);       break;
            case Opcode::OR_Q:  dyadic(SIZE_QUAD); asUQuad(pDstEA) |=   asUQuad(pSrcEA);       break;
            case Opcode::EOR_B: dyadic(SIZE_BYTE); asUByte(pDstEA) ^=   asUByte(pSrcEA);       break;
            case Opcode::EOR_W: dyadic(SIZE_WORD); asUWord(pDstEA) ^=   asUWord(pSrcEA);       break;
            case Opcode::EOR_L: dyadic(SIZE_LONG); asULong(pDstEA) ^=   asULong(pSrcEA);       break;
            case Opcode::EOR_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) ^=   asUQuad(pSrcEA);       break;
            case Opcode::NOT_B: dyadic(SIZE_BYTE); asUByte(pDstEA) =   ~asUByte(pSrcEA);       break;
            case Opcode::NOT_W: dyadic(SIZE_WORD); asUWord(pDstEA) =   ~asUWord(pSrcEA);       break;
            case Opcode::NOT_L: dyadic(SIZE_LONG); asULong(pDstEA) =   ~asULong(pSrcEA);       break;
            case Opcode::NOT_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) =   ~asUQuad(pSrcEA);       break;
            case Opcode::LSL_B: dyadic(SIZE_BYTE); asUByte(pDstEA) <<= (asUByte(pSrcEA) & 7);  break;
            case Opcode::LSL_W: dyadic(SIZE_WORD); asUWord(pDstEA) <<= (asUByte(pSrcEA) & 15); break;
            case Opcode::LSL_L: dyadic(SIZE_LONG); asULong(pDstEA) <<= (asUByte(pSrcEA) & 31); break;
            case Opcode::LSL_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) <<= (asUByte(pSrcEA) & 63); break;
            case Opcode::LSR_B: dyadic(SIZE_BYTE); asUByte(pDstEA) >>= (asUByte(pSrcEA) & 7);  break;
            case Opcode::LSR_W: dyadic(SIZE_WORD); asUWord(pDstEA) >>= (asUByte(pSrcEA) & 15); break;
            case Opcode::LSR_L: dyadic(SIZE_LONG); asULong(pDstEA) >>= (asUByte(pSrcEA) & 31); break;
            case Opcode::LSR_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) >>= (asUByte(pSrcEA) & 63); break;
            case Opcode::ROL_B:
            case Opcode::ROL_W:
            case Opcode::ROL_L:
            case Opcode::ROL_Q:
            case Opcode::ROR_B:
            case Opcode::ROR_W:
            case Opcode::ROR_L:
            case Opcode::ROR_Q:
            case Opcode::BCLR_B: dyadic(SIZE_BYTE); asUByte(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BCLR_W: dyadic(SIZE_WORD); asUWord(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BCLR_L: dyadic(SIZE_LONG); asULong(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BCLR_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) &= ~(1 << (asUByte(pSrcEA) & 63)); break;
            case Opcode::BSET_B: dyadic(SIZE_BYTE); asUByte(pDstEA) |=  (1 << (asUByte(pSrcEA) & 7));  break;
            case Opcode::BSET_W: dyadic(SIZE_WORD); asUWord(pDstEA) |=  (1 << (asUByte(pSrcEA) & 15)); break;
            case Opcode::BSET_L: dyadic(SIZE_LONG); asULong(pDstEA) |=  (1 << (asUByte(pSrcEA) & 31)); break;
            case Opcode::BSET_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) |=  (1 << (asUByte(pSrcEA) & 63)); break;
            case Opcode::BFCLR:
            case Opcode::BFSET:
            case Opcode::BFINS:
            case Opcode::BFEXTS:
            case Opcode::BFEXTU:
            case Opcode::BFFFO:
            case Opcode::BFCNT:
            case Opcode::BFXXX:

            // Arithmetic
            case Opcode::EXTB_W:
            case Opcode::EXTB_L:
            case Opcode::EXTB_Q:
            case Opcode::EXTW_L:
            case Opcode::EXTW_Q:
            case Opcode::EXTL_Q:
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
            case Opcode::MULS_B:
            case Opcode::MULS_W:
            case Opcode::MULS_L:
            case Opcode::MULS_Q:
            case Opcode::MULU_B:
            case Opcode::MULU_W:
            case Opcode::MULU_L:
            case Opcode::MULU_Q:
            case Opcode::FMUL_S:
            case Opcode::FMUL_D:
            case Opcode::DIVS_B:
            case Opcode::DIVS_W:
            case Opcode::DIVS_L:
            case Opcode::DIVS_Q:
            case Opcode::DIVU_B:
            case Opcode::DIVU_W:
            case Opcode::DIVU_L:
            case Opcode::DIVU_Q:
            case Opcode::FDIV_S:
            case Opcode::FDIV_D:
            case Opcode::FMOD_S:
            case Opcode::FMOD_D:
            case Opcode::FABS_S:
            case Opcode::FABS_D:
            case Opcode::FSQRT_S:
            case Opcode::FSQRT_D:
            case Opcode::FACOS_S:
            case Opcode::FACOS_D:
            case Opcode::FASIN_S:
            case Opcode::FASIN_D:
            case Opcode::FATAN_S:
            case Opcode::FATAN_D:
            case Opcode::FCOS_S:
            case Opcode::FCOS_D:
            case Opcode::FSIN_S:
            case Opcode::FSIN_D:
            case Opcode::FSINCOS_S:
            case Opcode::FSINCOS_D:
            case Opcode::FTAN_S:
            case Opcode::FTAN_D:
            case Opcode::FETOX_S:
            case Opcode::FETOX_D:
            case Opcode::FLOGN_S:
            case Opcode::FLOGN_D:
            case Opcode::FLOG2_S:
            case Opcode::FLOG2_D:
            case Opcode::FTWOTOX_S:
            case Opcode::FTWOTOX_D:
            case Opcode::FGETEXP_S:
            case Opcode::FGETEXP_D:
            case Opcode::FGETMAN_S:
            case Opcode::FGETMAN_D:
            default:
                return;
        }
    }

    NanoTime::Value uElapsed = NanoTime::mark() - uStart;

    float64 fMIPS = (1000.0 * uInstructionCount) / (float64)uElapsed;

    std::printf(
        "Total instructions %lu in %lu nanoseconds, %.2f MIPS\n",
        uInstructionCount,
        uElapsed,
        fMIPS
    );
}

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

namespace Interpreter = MC64K::Machine::Interpreter;
namespace Opcode      = MC64K::ByteCode::Opcode;

inline void rolByte(uint8* pVal, uint8 size) {
    size &= 7;
    uint8 val = *pVal;
    *pVal = val << size | val >> (8 - size);
}

inline void rolWord(uint16* pVal, uint8 size) {
    size &= 15;
    uint16 val = *pVal;
    *pVal = val << size | val >> (16 - size);
}

inline void rolLong(uint32* pVal, uint8 size) {
    size &= 31;
    uint32 val = *pVal;
    *pVal = val << size | val >> (32 - size);
}

inline void rolQuad(uint64* pVal, uint8 size) {
    size &= 63;
    uint64 val = *pVal;
    *pVal = val << size | val >> (64 - size);
}

inline void rorByte(uint8* pVal, uint8 size) {
    size &= 7;
    uint8 val = *pVal;
    *pVal = val >> size | val << (8 - size);
}

inline void rorWord(uint16* pVal, uint8 size) {
    size &= 15;
    uint16 val = *pVal;
    *pVal = val >> size | val << (16 - size);
}

inline void rorLong(uint32* pVal, uint8 size) {
    size &= 31;
    uint32 val = *pVal;
    *pVal = val >> size | val << (32 - size);
}

inline void rorQuad(uint64* pVal, uint8 size) {
    size &= 63;
    uint64 val = *pVal;
    *pVal = val >> size | val << (64 - size);
}

/**
 *
 */
void Interpreter::Static::run() {
    if (!pProgramCounter) {
        return;
    }

    eStatus    = RUNNING;
    iCallDepth = 1;

    initDisplacement();

    uint64 uInstructionCount = 0;
    NanoTime::Value uStart = NanoTime::mark();

    while (RUNNING == eStatus) {
        ++uInstructionCount;
        switch (*pProgramCounter++) {
            // Control
            case Opcode::HCF: todo();
            case Opcode::BRA_B: branchByte(); break;
            case Opcode::BRA:   branchLong(); break;

            case Opcode::BSR_B: {
                int8 iShortDisplacement = (int8)*pProgramCounter++;
                pushProgramCounter();
                pProgramCounter += iShortDisplacement;
                ++iCallDepth;
                break;
            }

            case Opcode::BSR:
                readDisplacement();
                pushProgramCounter();
                pProgramCounter += iDisplacement;
                ++iCallDepth;
                break;

            case Opcode::JMP:
                monadic(SIZE_QUAD);
                pProgramCounter = (const uint8*)pDstEA;
                break;

            case Opcode::JSR:
                monadic(SIZE_QUAD);
                pushProgramCounter();
                pProgramCounter = (const uint8*)pDstEA;
                ++iCallDepth;
                break;

            case Opcode::RTS:
                popProgramCounter();
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
            case Opcode::SAVEM: todo();
            case Opcode::LOADM: todo();

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
            case Opcode::FSAVEM:
            case Opcode::FLOADM: todo();

            // DataMove - clr
            case Opcode::CLR_B: monadic(SIZE_BYTE); asUByte(pDstEA) = 0; break;
            case Opcode::CLR_W: monadic(SIZE_WORD); asUWord(pDstEA) = 0; break;
            case Opcode::CLR_L: monadic(SIZE_LONG); asULong(pDstEA) = 0; break;
            case Opcode::CLR_Q: monadic(SIZE_QUAD); asUQuad(pDstEA) = 0; break;

            case Opcode::EXG:
            case Opcode::FEXG:
            case Opcode::SWAP:
            case Opcode::SWAP_L: dyadic(SIZE_LONG); asULong(pDstEA) = __builtin_bswap32(asULong(pSrcEA)); break;
            case Opcode::SWAP_Q: dyadic(SIZE_QUAD); asUQuad(pDstEA) = __builtin_bswap64(asUQuad(pSrcEA)); break;
            case Opcode::LINK:
            case Opcode::UNLK:
            case Opcode::LEA:
            case Opcode::PEA: todo();

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
            case Opcode::ROR_Q: todo();
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
            case Opcode::BFXXX: todo();

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
                todo();
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

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

void NOINLINE Interpreter::handleBDC() {
    using namespace MC64K::ByteCode;
    initDisplacement();
    switch (*puProgramCounter++) {
        // beq/fbeq
        case Opcode::IEQ_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   == asByte(pDstEA));   return;
        case Opcode::IEQ_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   == asWord(pDstEA));   return;
        case Opcode::IEQ_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   == asLong(pDstEA));   return;
        case Opcode::IEQ_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   == asQuad(pDstEA));   return;
        case Opcode::FEQ_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) == asSingle(pDstEA)); return;
        case Opcode::FEQ_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) == asDouble(pDstEA)); return;

        // bne/fbne
        case Opcode::INE_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   != asByte(pDstEA));   return;
        case Opcode::INE_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   != asWord(pDstEA));   return;
        case Opcode::INE_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   != asLong(pDstEA));   return;
        case Opcode::INE_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   != asQuad(pDstEA));   return;
        case Opcode::FNE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) != asSingle(pDstEA)); return;
        case Opcode::FNE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) != asDouble(pDstEA)); return;

        // blo/blt/fblt
        case Opcode::ILT_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   < asByte(pDstEA));    return;
        case Opcode::ILT_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   < asWord(pDstEA));    return;
        case Opcode::ILT_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   < asLong(pDstEA));    return;
        case Opcode::ILT_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   < asQuad(pDstEA));    return;
        case Opcode::ULT_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pSrcEA)  < asUByte(pDstEA));   return;
        case Opcode::ULT_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pSrcEA)  < asUWord(pDstEA));   return;
        case Opcode::ULT_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asULong(pSrcEA)  < asULong(pDstEA));   return;
        case Opcode::ULT_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pSrcEA)  < asUQuad(pDstEA));   return;
        case Opcode::FLT_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) < asSingle(pDstEA));  return;
        case Opcode::FLT_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) < asDouble(pDstEA));  return;

        // bls/ble/fble
        case Opcode::ILE_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   <= asByte(pDstEA));   return;
        case Opcode::ILE_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   <= asWord(pDstEA));   return;
        case Opcode::ILE_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   <= asLong(pDstEA));   return;
        case Opcode::ILE_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   <= asQuad(pDstEA));   return;
        case Opcode::ULE_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pSrcEA)  <= asUByte(pDstEA));  return;
        case Opcode::ULE_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pSrcEA)  <= asUWord(pDstEA));  return;
        case Opcode::ULE_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asULong(pSrcEA)  <= asULong(pDstEA));  return;
        case Opcode::ULE_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pSrcEA)  <= asUQuad(pDstEA));  return;
        case Opcode::FLE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) <= asSingle(pDstEA)); return;
        case Opcode::FLE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) <= asDouble(pDstEA)); return;

        // bhs/bge/fbge
        case Opcode::IGE_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   >= asByte(pDstEA));   return;
        case Opcode::IGE_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   >= asWord(pDstEA));   return;
        case Opcode::IGE_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   >= asLong(pDstEA));   return;
        case Opcode::IGE_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   >= asQuad(pDstEA));   return;
        case Opcode::UGE_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pSrcEA)  >= asUByte(pDstEA));  return;
        case Opcode::UGE_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pSrcEA)  >= asUWord(pDstEA));  return;
        case Opcode::UGE_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asULong(pSrcEA)  >= asULong(pDstEA));  return;
        case Opcode::UGE_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pSrcEA)  >= asUQuad(pDstEA));  return;
        case Opcode::FGE_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) >= asSingle(pDstEA)); return;
        case Opcode::FGE_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) >= asDouble(pDstEA)); return;

        // bhi/bgt/fbgt
        case Opcode::IGT_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pSrcEA)   > asByte(pDstEA));    return;
        case Opcode::IGT_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asWord(pSrcEA)   > asWord(pDstEA));    return;
        case Opcode::IGT_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asLong(pSrcEA)   > asLong(pDstEA));    return;
        case Opcode::IGT_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pSrcEA)   > asQuad(pDstEA));    return;
        case Opcode::UGT_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pSrcEA)  > asUByte(pDstEA));   return;
        case Opcode::UGT_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pSrcEA)  > asUWord(pDstEA));   return;
        case Opcode::UGT_L: dyadic(SIZE_LONG); readDisplacement(); bcc(asULong(pSrcEA)  > asULong(pDstEA));   return;
        case Opcode::UGT_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pSrcEA)  > asUQuad(pDstEA));   return;

        case Opcode::FGT_S: dyadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pSrcEA) > asSingle(pDstEA)); return;
        case Opcode::FGT_D: dyadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pSrcEA) > asDouble(pDstEA));  return;

        // Integer Bit position set
        case Opcode::BPS_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(asUByte(pDstEA)   & asBitPos(pSrcEA, 7));  return;
        case Opcode::BPS_W: dyadic(SIZE_WORD); readDisplacement(); bcc(asUWord(pDstEA)   & asBitPos(pSrcEA, 15)); return;
        case Opcode::BPS_L: dyadic(SIZE_WORD); readDisplacement(); bcc(asULong(pDstEA)   & asBitPos(pSrcEA, 31)); return;
        case Opcode::BPS_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(asUQuad(pDstEA)   & asBitPos(pSrcEA, 63)); return;

        // Integer Bit position clear
        case Opcode::BPC_B: dyadic(SIZE_BYTE); readDisplacement(); bcc(!(asUByte(pDstEA) & asBitPos(pSrcEA, 7)));  return;
        case Opcode::BPC_W: dyadic(SIZE_WORD); readDisplacement(); bcc(!(asUWord(pDstEA) & asBitPos(pSrcEA, 15))); return;
        case Opcode::BPC_L: dyadic(SIZE_WORD); readDisplacement(); bcc(!(asULong(pDstEA) & asBitPos(pSrcEA, 31))); return;
        case Opcode::BPC_Q: dyadic(SIZE_QUAD); readDisplacement(); bcc(!(asUQuad(pDstEA) & asBitPos(pSrcEA, 63))); return;

        default:
            todo(); // will trigger an unimplemented opcode
            break;
    }
}

void NOINLINE Interpreter::handleR2RBDC() {
    using namespace MC64K::ByteCode;
    initDisplacement();
    switch (*puProgramCounter++) {
        // beq/fbeq
        case Opcode::IEQ_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   == asByte(pDstEA));   return;
        case Opcode::IEQ_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   == asWord(pDstEA));   return;
        case Opcode::IEQ_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   == asLong(pDstEA));   return;
        case Opcode::IEQ_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   == asQuad(pDstEA));   return;
        case Opcode::FEQ_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) == asSingle(pDstEA)); return;
        case Opcode::FEQ_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) == asDouble(pDstEA)); return;

        // bne/fbne
        case Opcode::INE_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   != asByte(pDstEA));   return;
        case Opcode::INE_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   != asWord(pDstEA));   return;
        case Opcode::INE_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   != asLong(pDstEA));   return;
        case Opcode::INE_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   != asQuad(pDstEA));   return;
        case Opcode::FNE_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) != asSingle(pDstEA)); return;
        case Opcode::FNE_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) != asDouble(pDstEA)); return;

        // blo/blt/fblt
        case Opcode::ILT_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   < asByte(pDstEA));    return;
        case Opcode::ILT_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   < asWord(pDstEA));    return;
        case Opcode::ILT_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   < asLong(pDstEA));    return;
        case Opcode::ILT_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   < asQuad(pDstEA));    return;
        case Opcode::ULT_B: unpackGPRPair(); readDisplacement(); bcc(asUByte(pSrcEA)  < asUByte(pDstEA));   return;
        case Opcode::ULT_W: unpackGPRPair(); readDisplacement(); bcc(asUWord(pSrcEA)  < asUWord(pDstEA));   return;
        case Opcode::ULT_L: unpackGPRPair(); readDisplacement(); bcc(asULong(pSrcEA)  < asULong(pDstEA));   return;
        case Opcode::ULT_Q: unpackGPRPair(); readDisplacement(); bcc(asUQuad(pSrcEA)  < asUQuad(pDstEA));   return;
        case Opcode::FLT_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) < asSingle(pDstEA));  return;
        case Opcode::FLT_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) < asDouble(pDstEA));  return;

        // bls/ble/fble
        case Opcode::ILE_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   <= asByte(pDstEA));   return;
        case Opcode::ILE_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   <= asWord(pDstEA));   return;
        case Opcode::ILE_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   <= asLong(pDstEA));   return;
        case Opcode::ILE_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   <= asQuad(pDstEA));   return;
        case Opcode::ULE_B: unpackGPRPair(); readDisplacement(); bcc(asUByte(pSrcEA)  <= asUByte(pDstEA));  return;
        case Opcode::ULE_W: unpackGPRPair(); readDisplacement(); bcc(asUWord(pSrcEA)  <= asUWord(pDstEA));  return;
        case Opcode::ULE_L: unpackGPRPair(); readDisplacement(); bcc(asULong(pSrcEA)  <= asULong(pDstEA));  return;
        case Opcode::ULE_Q: unpackGPRPair(); readDisplacement(); bcc(asUQuad(pSrcEA)  <= asUQuad(pDstEA));  return;
        case Opcode::FLE_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) <= asSingle(pDstEA)); return;
        case Opcode::FLE_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) <= asDouble(pDstEA)); return;

        // bhs/bge/fbge
        case Opcode::IGE_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   >= asByte(pDstEA));   return;
        case Opcode::IGE_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   >= asWord(pDstEA));   return;
        case Opcode::IGE_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   >= asLong(pDstEA));   return;
        case Opcode::IGE_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   >= asQuad(pDstEA));   return;
        case Opcode::UGE_B: unpackGPRPair(); readDisplacement(); bcc(asUByte(pSrcEA)  >= asUByte(pDstEA));  return;
        case Opcode::UGE_W: unpackGPRPair(); readDisplacement(); bcc(asUWord(pSrcEA)  >= asUWord(pDstEA));  return;
        case Opcode::UGE_L: unpackGPRPair(); readDisplacement(); bcc(asULong(pSrcEA)  >= asULong(pDstEA));  return;
        case Opcode::UGE_Q: unpackGPRPair(); readDisplacement(); bcc(asUQuad(pSrcEA)  >= asUQuad(pDstEA));  return;
        case Opcode::FGE_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) >= asSingle(pDstEA)); return;
        case Opcode::FGE_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) >= asDouble(pDstEA)); return;

        // bhi/bgt/fbgt
        case Opcode::IGT_B: unpackGPRPair(); readDisplacement(); bcc(asByte(pSrcEA)   > asByte(pDstEA));    return;
        case Opcode::IGT_W: unpackGPRPair(); readDisplacement(); bcc(asWord(pSrcEA)   > asWord(pDstEA));    return;
        case Opcode::IGT_L: unpackGPRPair(); readDisplacement(); bcc(asLong(pSrcEA)   > asLong(pDstEA));    return;
        case Opcode::IGT_Q: unpackGPRPair(); readDisplacement(); bcc(asQuad(pSrcEA)   > asQuad(pDstEA));    return;
        case Opcode::UGT_B: unpackGPRPair(); readDisplacement(); bcc(asUByte(pSrcEA)  > asUByte(pDstEA));   return;
        case Opcode::UGT_W: unpackGPRPair(); readDisplacement(); bcc(asUWord(pSrcEA)  > asUWord(pDstEA));   return;
        case Opcode::UGT_L: unpackGPRPair(); readDisplacement(); bcc(asULong(pSrcEA)  > asULong(pDstEA));   return;
        case Opcode::UGT_Q: unpackGPRPair(); readDisplacement(); bcc(asUQuad(pSrcEA)  > asUQuad(pDstEA));   return;
        case Opcode::FGT_S: unpackFPRPair(); readDisplacement(); bcc(asSingle(pSrcEA) > asSingle(pDstEA));  return;
        case Opcode::FGT_D: unpackFPRPair(); readDisplacement(); bcc(asDouble(pSrcEA) > asDouble(pDstEA));  return;

        // Integer Bit position set
        case Opcode::BPS_B: unpackGPRPair(); readDisplacement(); bcc(asUByte(pDstEA)   & asBitPos(pSrcEA, 7));  return;
        case Opcode::BPS_W: unpackGPRPair(); readDisplacement(); bcc(asUWord(pDstEA)   & asBitPos(pSrcEA, 15)); return;
        case Opcode::BPS_L: unpackGPRPair(); readDisplacement(); bcc(asULong(pDstEA)   & asBitPos(pSrcEA, 31)); return;
        case Opcode::BPS_Q: unpackGPRPair(); readDisplacement(); bcc(asUQuad(pDstEA)   & asBitPos(pSrcEA, 63)); return;

        // Integer Bit position clear
        case Opcode::BPC_B: unpackGPRPair(); readDisplacement(); bcc(!(asUByte(pDstEA) & asBitPos(pSrcEA, 7)));  return;
        case Opcode::BPC_W: unpackGPRPair(); readDisplacement(); bcc(!(asUWord(pDstEA) & asBitPos(pSrcEA, 15))); return;
        case Opcode::BPC_L: unpackGPRPair(); readDisplacement(); bcc(!(asULong(pDstEA) & asBitPos(pSrcEA, 31))); return;
        case Opcode::BPC_Q: unpackGPRPair(); readDisplacement(); bcc(!(asUQuad(pDstEA) & asBitPos(pSrcEA, 63))); return;

        default:
            todo(); // will trigger an unimplemented opcode
            break;
    }
}

}


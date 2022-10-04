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
 * Deal with monadic compare and branch
 */
void NOINLINE Interpreter::handleBMC() {
    using namespace MC64K::ByteCode;
    initDisplacement();
    switch (*puProgramCounter++) {
        // biz/fbiz
        case Opcode::IEQ_B: monadic(SIZE_BYTE); readDisplacement(); bcc(!asByte(pDstEA)); return;
        case Opcode::IEQ_W: monadic(SIZE_WORD); readDisplacement(); bcc(!asWord(pDstEA)); return;
        case Opcode::IEQ_L: monadic(SIZE_LONG); readDisplacement(); bcc(!asLong(pDstEA)); return;
        case Opcode::IEQ_Q: monadic(SIZE_QUAD); readDisplacement(); bcc(!asQuad(pDstEA)); return;
        case Opcode::FEQ_S: monadic(SIZE_LONG); readDisplacement(); bcc(!asSingle(pDstEA)); return;
        case Opcode::FEQ_D: monadic(SIZE_QUAD); readDisplacement(); bcc(!asDouble(pDstEA)); return;

        // bnz/fbnz
        case Opcode::INE_B: monadic(SIZE_BYTE); readDisplacement(); bcc(asByte(pDstEA)); return;
        case Opcode::INE_W: monadic(SIZE_WORD); readDisplacement(); bcc(asWord(pDstEA)); return;
        case Opcode::INE_L: monadic(SIZE_LONG); readDisplacement(); bcc(asLong(pDstEA)); return;
        case Opcode::INE_Q: monadic(SIZE_QUAD); readDisplacement(); bcc(asQuad(pDstEA)); return;
        case Opcode::FNE_S: monadic(SIZE_LONG); readDisplacement(); bcc(asSingle(pDstEA)); return;
        case Opcode::FNE_D: monadic(SIZE_QUAD); readDisplacement(); bcc(asDouble(pDstEA)); return;

        // bmi/fbmi
        case Opcode::ILT_B: monadic(SIZE_BYTE); readDisplacement(); bcc(0 > asByte(pDstEA)); return;
        case Opcode::ILT_W: monadic(SIZE_WORD); readDisplacement(); bcc(0 > asWord(pDstEA)); return;
        case Opcode::ILT_L: monadic(SIZE_LONG); readDisplacement(); bcc(0 > asLong(pDstEA)); return;
        case Opcode::ILT_Q: monadic(SIZE_QUAD); readDisplacement(); bcc(0 > asQuad(pDstEA)); return;
        case Opcode::FLT_S: monadic(SIZE_LONG); readDisplacement(); bcc(0 > asSingle(pDstEA)); return;
        case Opcode::FLT_D: monadic(SIZE_QUAD); readDisplacement(); bcc(0 > asDouble(pDstEA)); return;

        // bpl/fbpl
        case Opcode::IGT_B: monadic(SIZE_BYTE); readDisplacement(); bcc(0 < asByte(pDstEA)); return;
        case Opcode::IGT_W: monadic(SIZE_WORD); readDisplacement(); bcc(0 < asWord(pDstEA)); return;
        case Opcode::IGT_L: monadic(SIZE_LONG); readDisplacement(); bcc(0 < asLong(pDstEA)); return;
        case Opcode::IGT_Q: monadic(SIZE_QUAD); readDisplacement(); bcc(0 < asQuad(pDstEA)); return;
        case Opcode::FGT_S: monadic(SIZE_LONG); readDisplacement(); bcc(0 < asSingle(pDstEA)); return;
        case Opcode::FGT_D: monadic(SIZE_QUAD); readDisplacement(); bcc(0 < asDouble(pDstEA)); return;

        default:
            todo(); // will trigger an unimplemented opcode
            break;
    }
}

/**
 * Deal with monadic compare and branch
 */
void NOINLINE Interpreter::handleRBMC() {
    using namespace MC64K::ByteCode;
    initDisplacement();

    switch (*puProgramCounter++) {
        // biz/fbiz
        case Opcode::IEQ_B: unpackGPR(); readDisplacement(); bcc(!asByte(pDstEA)); return;
        case Opcode::IEQ_W: unpackGPR(); readDisplacement(); bcc(!asWord(pDstEA)); return;
        case Opcode::IEQ_L: unpackGPR(); readDisplacement(); bcc(!asLong(pDstEA)); return;
        case Opcode::IEQ_Q: unpackGPR(); readDisplacement(); bcc(!asQuad(pDstEA)); return;
        case Opcode::FEQ_S: unpackFPR(); readDisplacement(); bcc(!asSingle(pDstEA)); return;
        case Opcode::FEQ_D: unpackFPR(); readDisplacement(); bcc(!asDouble(pDstEA)); return;

        // bnz/fbnz
        case Opcode::INE_B: unpackGPR(); readDisplacement(); bcc(asByte(pDstEA)); return;
        case Opcode::INE_W: unpackGPR(); readDisplacement(); bcc(asWord(pDstEA)); return;
        case Opcode::INE_L: unpackGPR(); readDisplacement(); bcc(asLong(pDstEA)); return;
        case Opcode::INE_Q: unpackGPR(); readDisplacement(); bcc(asQuad(pDstEA)); return;
        case Opcode::FNE_S: unpackFPR(); readDisplacement(); bcc(asSingle(pDstEA)); return;
        case Opcode::FNE_D: unpackFPR(); readDisplacement(); bcc(asDouble(pDstEA)); return;

        // bmi/fbmi
        case Opcode::ILT_B: unpackGPR(); readDisplacement(); bcc(0 > asByte(pDstEA)); return;
        case Opcode::ILT_W: unpackGPR(); readDisplacement(); bcc(0 > asWord(pDstEA)); return;
        case Opcode::ILT_L: unpackGPR(); readDisplacement(); bcc(0 > asLong(pDstEA)); return;
        case Opcode::ILT_Q: unpackGPR(); readDisplacement(); bcc(0 > asQuad(pDstEA)); return;
        case Opcode::FLT_S: unpackFPR(); readDisplacement(); bcc(0 > asSingle(pDstEA)); return;
        case Opcode::FLT_D: unpackFPR(); readDisplacement(); bcc(0 > asDouble(pDstEA)); return;

        // bpl/fbpl
        case Opcode::IGT_B: unpackGPR(); readDisplacement(); bcc(0 < asByte(pDstEA)); return;
        case Opcode::IGT_W: unpackGPR(); readDisplacement(); bcc(0 < asWord(pDstEA)); return;
        case Opcode::IGT_L: unpackGPR(); readDisplacement(); bcc(0 < asLong(pDstEA)); return;
        case Opcode::IGT_Q: unpackGPR(); readDisplacement(); bcc(0 < asQuad(pDstEA)); return;
        case Opcode::FGT_S: unpackFPR(); readDisplacement(); bcc(0 < asSingle(pDstEA)); return;
        case Opcode::FGT_D: unpackFPR(); readDisplacement(); bcc(0 < asDouble(pDstEA)); return;

        default:
            todo(); // will trigger an unimplemented opcode
            break;
    }
}

}


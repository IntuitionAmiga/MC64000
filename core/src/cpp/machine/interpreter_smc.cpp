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

#define smc(c) asUByte(pDstEA) = ((c)) ? 0xFF : 0;

/**
 * Deal with monadic compare and branch
 */
void NOINLINE Interpreter::handleSMC() {
    using namespace MC64K::ByteCode;

    // next byte is the condition we will test
    uint8 uCond = *puProgramCounter++;

    // Now get the destination byte that will be set or clear
    monadic(SIZE_BYTE);

    switch (uCond) {
        // biz/fbiz
        case Opcode::IEQ_B: monadic2(SIZE_BYTE); smc(!asByte(pDstEA)); return;
        case Opcode::IEQ_W: monadic2(SIZE_WORD); smc(!asWord(pDstEA)); return;
        case Opcode::IEQ_L: monadic2(SIZE_LONG); smc(!asLong(pDstEA)); return;
        case Opcode::IEQ_Q: monadic2(SIZE_QUAD); smc(!asQuad(pDstEA)); return;
        case Opcode::FEQ_S: monadic2(SIZE_LONG); smc(!asSingle(pDstEA)); return;
        case Opcode::FEQ_D: monadic2(SIZE_QUAD); smc(!asDouble(pDstEA)); return;

        // bnz/fbnz
        case Opcode::INE_B: monadic2(SIZE_BYTE); smc(asByte(pDstEA)); return;
        case Opcode::INE_W: monadic2(SIZE_WORD); smc(asWord(pDstEA)); return;
        case Opcode::INE_L: monadic2(SIZE_LONG); smc(asLong(pDstEA)); return;
        case Opcode::INE_Q: monadic2(SIZE_QUAD); smc(asQuad(pDstEA)); return;
        case Opcode::FNE_S: monadic2(SIZE_LONG); smc(asSingle(pDstEA)); return;
        case Opcode::FNE_D: monadic2(SIZE_QUAD); smc(asDouble(pDstEA)); return;

        // bmi/fbmi
        case Opcode::ILT_B: monadic2(SIZE_BYTE); smc(0 > asByte(pDstEA)); return;
        case Opcode::ILT_W: monadic2(SIZE_WORD); smc(0 > asWord(pDstEA)); return;
        case Opcode::ILT_L: monadic2(SIZE_LONG); smc(0 > asLong(pDstEA)); return;
        case Opcode::ILT_Q: monadic2(SIZE_QUAD); smc(0 > asQuad(pDstEA)); return;
        case Opcode::FLT_S: monadic2(SIZE_LONG); smc(0 > asSingle(pDstEA)); return;
        case Opcode::FLT_D: monadic2(SIZE_QUAD); smc(0 > asDouble(pDstEA)); return;

        // bpl/fbpl
        case Opcode::IGT_B: monadic2(SIZE_BYTE); smc(0 < asByte(pDstEA)); return;
        case Opcode::IGT_W: monadic2(SIZE_WORD); smc(0 < asWord(pDstEA)); return;
        case Opcode::IGT_L: monadic2(SIZE_LONG); smc(0 < asLong(pDstEA)); return;
        case Opcode::IGT_Q: monadic2(SIZE_QUAD); smc(0 < asQuad(pDstEA)); return;
        case Opcode::FGT_S: monadic2(SIZE_LONG); smc(0 < asSingle(pDstEA)); return;
        case Opcode::FGT_D: monadic2(SIZE_QUAD); smc(0 < asDouble(pDstEA)); return;

        default:
            todo(); // will trigger an unimplemented opcode
            break;
    }
}

}


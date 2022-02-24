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
#include <machine/register.hpp>
#include <machine/interpreter.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::AssemblyTest {

void setGPR() {
    Interpreter::gpr<0>().value<int8>()   = 0x11;
    Interpreter::gpr<1>().value<uint8>()  = 0x22;
    Interpreter::gpr<2>().value<int16>()  = 0x3333;
    Interpreter::gpr<3>().value<uint16>() = 0x4444;
    Interpreter::gpr<4>().value<int32>()  = 0x55555555;
    Interpreter::gpr<5>().value<uint32>() = 0x55555555;
    Interpreter::gpr<6>().value<int64>()  = 0x6666666666666666;
    Interpreter::gpr<7>().value<uint64>() = 0x7777777777777777;
}

void setFPR() {
    Interpreter::fpr<0>().value<float32>()  = 0.25;
    Interpreter::fpr<1>().value<float64>()  = 0.125;
}

} // namespace


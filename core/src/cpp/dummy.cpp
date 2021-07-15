#include <cstdio>
#include "include/mc64k.hpp"

using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

int main() {

    // Which state to dump out
    int iDumpState = StaticInterpreter::STATE_GPR|StaticInterpreter::STATE_TMP;

    // Some data
    uint32 aTest[] = {
        0xABADCAFE,
        0x31337DAD,
        1<<28,
        1
    };


    // Simple bytecode test
    uint8 aByteCode[] = {
        Opcode::MOVE_L,
        EffectiveAddress::R1_DIR,
        EffectiveAddress::R0_IND_DSP,
        8, 0, 0, 0,

        Opcode::MOVE_L,
        EffectiveAddress::R2_DIR,
        EffectiveAddress::R0_IND_DSP,
        12, 0, 0, 0,

        Opcode::BSR_B,
        7,

        Opcode::DBNZ,
        EffectiveAddress::R1_DIR,
        (uint8)-8, 0xFF, 0xFF, 0xFF,

        Opcode::RTS,

        Opcode::ADD_L,
        EffectiveAddress::R3_DIR,
        EffectiveAddress::R2_DIR,

        // done
        Opcode::RTS
    };

    uint8 aStack[32] = { 0 };

    // Set up the interpreter. r0 shall contain the address of the test data.
    StaticInterpreter::gpr(0).pULong  = aTest;
    StaticInterpreter::gpr(15).pUByte = aStack + 24;

    StaticInterpreter::setProgramCounter(aByteCode);
    StaticInterpreter::dumpState(iDumpState);

    StaticInterpreter::run();
    StaticInterpreter::dumpState(iDumpState);
    return 0;
}

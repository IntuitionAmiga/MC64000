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
        100000000,
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

        Opcode::SUB_L,
        EffectiveAddress::R1_DIR,
        EffectiveAddress::R2_DIR,

        Opcode::BNZ_L,
        EffectiveAddress::R1_DIR,
        (uint8)-9, 0xFF, 0xFF, 0xFF,

        // done
        Opcode::RTS
    };

    // Set up the interpreter. r0 shall contain the address of the test data.
    StaticInterpreter::gpr(0).pULong = aTest;
    StaticInterpreter::setProgramCounter(aByteCode);
    StaticInterpreter::dumpState(iDumpState);

    StaticInterpreter::run();
    StaticInterpreter::dumpState(iDumpState);
    return 0;
}

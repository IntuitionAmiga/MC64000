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
    };

    // A single bytecode operation to test: move.l (d0), d1
    uint8 aByteCode[] = {
        Opcode::MOVE_L,            // Opcode
        EffectiveAddress::R1_DIR,  // Destination EA first
        EffectiveAddress::R0_IND,  // Source EA second
    };

    // Set up the interpreter. r0 shall contain the address of the test data.
    StaticInterpreter::gpr(0).pULong = aTest;
    StaticInterpreter::setProgramCounter(aByteCode);
    StaticInterpreter::dumpState(iDumpState);

    StaticInterpreter::run();
    StaticInterpreter::dumpState(iDumpState);
    return 0;
}

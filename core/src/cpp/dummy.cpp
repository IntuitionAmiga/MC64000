#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "bytecode/opcode.hpp"
#include "bytecode/effective_address.hpp"

int main(/*int iArgN, const char** aArgV*/) {

//     if (iArgN < 2) {
//         std::puts("Missing required parameter");
//         std::exit(EXIT_FAILURE);
//     }
//
//     try {
//         Loader::Binary oMC64KExecutabe(aArgV[1]);
//
//     } catch (Loader::Error::FileIO oError) {
//         std::printf("Unable to load binary file \"%s\", %s.\n", oError.sFileName, oError.sIssue);
//         std::exit(EXIT_FAILURE);
//     }
//
//     return EXIT_SUCCESS;

    using namespace MC64K::Machine;
    using namespace MC64K::ByteCode;

    // Which state to dump out
    int iDumpState = Interpreter::STATE_GPR|Interpreter::STATE_TMP;

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
    Interpreter::gpr(0).pULong  = aTest;
    Interpreter::gpr(15).pUByte = aStack + 24;

    Interpreter::setProgramCounter(aByteCode);
    Interpreter::dumpState(iDumpState);

    Interpreter::run();
    Interpreter::dumpState(iDumpState);
    return 0;
}

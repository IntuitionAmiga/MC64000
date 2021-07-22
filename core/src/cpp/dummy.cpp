#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "bytecode/opcode.hpp"
#include "bytecode/effective_address.hpp"
#include "loader/binary.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;
using namespace MC64K::ByteCode;

Interpreter::Status nativeTest() {
    std::puts("Native Call");
    return Interpreter::RUNNING;
}

int main(int iArgN, const char** aArgV) {


    if (iArgN < 2) {
        std::puts("Missing required parameter");
        std::exit(EXIT_FAILURE);
    }

    try {
        const char* sExecutableName = aArgV[1];
        Binary oMC64KBinary(sExecutableName);
        const Executable* pExecutable = oMC64KBinary.load();

        std::printf(
            "Executable %s loaded at %p\n",
            sExecutableName,
            pExecutable
        );

        const Executable::EntryPoint* aEntryPoints = pExecutable->getEntryPoints();
        for (unsigned u = 0; u < pExecutable->getNumEntryPoints(); ++u) {
            std::printf(
                "\t%2u %p %s\n",
                u,
                aEntryPoints[u].pByteCode,
                aEntryPoints[u].sFunction
            );
        }

        const int iDumpState = Interpreter::STATE_FPR|Interpreter::STATE_GPR|Interpreter::STATE_TMP;
        uint8 aStack[32] = { 0 };

        Interpreter::setHostFunction(nativeTest, 0x69);
        Interpreter::gpr(15).pUByte = aStack + sizeof(aStack) - 8;
        Interpreter::setProgramCounter(aEntryPoints[0].pByteCode);
        Interpreter::dumpState(iDumpState);
        Interpreter::run();
        Interpreter::dumpState(iDumpState);

        delete pExecutable;
    } catch (Error& oError) {
        std::printf(
            "Unable to load binary file \"%s\", %s.\n",
            oError.sFileName,
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

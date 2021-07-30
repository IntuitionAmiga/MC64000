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

        const Executable::Symbol* aExports = 0;

        std::printf(
            "Executable %s loaded at %p\n",
            sExecutableName,
            pExecutable
        );

        uint32 uSymbolCount = 0;

        if ( (uSymbolCount = pExecutable->getNumExportedSymbols()) ) {
            std::printf(
                "Executable defines %u exported symbols:\n",
                uSymbolCount
            );

            aExports = pExecutable->getExportedSymbols();
            for (unsigned u = 0; u < uSymbolCount; ++u) {
                std::printf(
                    "\t%2u %p %s\n",
                    u,
                    aExports[u].pRawData,
                    aExports[u].sIdentifier
                );
            }
        }

        if ( (uSymbolCount = pExecutable->getNumImportedSymbols()) ) {
            std::printf(
                "Executable expects %u imported symbols:\n",
                uSymbolCount
            );

            const Executable::Symbol* aImports = pExecutable->getImportedSymbols();
            for (unsigned u = 0; u < uSymbolCount; ++u) {
                std::printf(
                    "\t%2u %p %s\n",
                    u,
                    aImports[u].pRawData,
                    aImports[u].sIdentifier
                );
            }
        }

        if (aExports) {
            const int iDumpState = Interpreter::STATE_FPR|Interpreter::STATE_GPR|Interpreter::STATE_TMP;
            Interpreter::allocateStack(256);
            Interpreter::setHostFunction(nativeTest, 0x69);
            Interpreter::setProgramCounter(aExports[0].pByteCode);
            Interpreter::dumpState(iDumpState);
            Interpreter::run();
            Interpreter::dumpState(iDumpState|Interpreter::STATE_STACK);
            Interpreter::freeStack();
        }
        delete pExecutable;
    } catch (MC64K::Loader::Error& oError) {
        std::printf(
            "Unable to load binary file \"%s\", %s.\n",
            oError.sFileName,
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    } catch (MC64K::Machine::Error& oError) {
        std::printf(
            "Machine error: %s.\n",
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

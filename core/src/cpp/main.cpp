#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/executable.hpp"
#include "host/definition.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;
using namespace MC64K::Host;


extern Interpreter::Status nativeTest();
extern Definition standardTestHost;

/**
 * Entry point
 */
int main(int iArgN, const char** aArgV) {

    if (iArgN < 2) {
        std::puts("Missing required parameter");
        std::exit(EXIT_FAILURE);
    }

    try {
        const char* sExecutableName = aArgV[1];

        // Initialise the binary loader
        Binary oMC64KBinary(standardTestHost);
        const Executable* pExecutable = oMC64KBinary.load(sExecutableName);
        //const LinkSymbol* aExports = 0;

        std::printf(
            "Executable %s loaded at %p\n",
            sExecutableName,
            pExecutable
        );

        size_t uSymbolCount = 0;

        const LinkSymbolSet* oExported = pExecutable->getExportedSymbolSet();
        if ( (uSymbolCount = oExported->getCount()) ) {
            std::printf(
                "Executable defines %u exported symbols:\n",
                (uint32)uSymbolCount
            );
            oExported->dump(stdout);
            //aExports = oExported->getSymbols();
        }
        const LinkSymbolSet* oImported = pExecutable->getImportedSymbolSet();
        if ( (uSymbolCount = oImported->getCount()) ) {
            std::printf(
                "Executable defines %u imported symbols:\n",
                (uint32)uSymbolCount
            );
            oImported->dump(stdout);
            //aExports = oExported->getSymbols();
        }


//         if ( (uSymbolCount = pExecutable->getNumImportedSymbols()) ) {
//             std::printf(
//                 "Executable expects %u imported symbols:\n",
//                 uSymbolCount
//             );
//
//             const LinkSymbol* aImports = pExecutable->getImportedSymbols();
//             for (unsigned u = 0; u < uSymbolCount; ++u) {
//                 std::printf(
//                     "\t%2u %p %s 0x%016lX\n",
//                     u,
//                     aImports[u].pRawData,
//                     aImports[u].sIdentifier,
//                     aExports[u].uFlags
//                 );
//             }
//         }
//
//         if (aExports) {
//             const int iDumpState = Interpreter::STATE_FPR|Interpreter::STATE_GPR|Interpreter::STATE_TMP;
//             Interpreter::allocateStack(256);
//             Interpreter::setHostFunction(nativeTest, 0x69);
//             Interpreter::setProgramCounter(aExports[0].pByteCode);
//             Interpreter::dumpState (iDumpState);
//             Interpreter::run();
//             Interpreter::dumpState (iDumpState|Interpreter::STATE_STACK);
//             Interpreter::freeStack();
//         }
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

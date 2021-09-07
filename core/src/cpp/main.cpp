#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/binary.hpp"
#include "host.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;

Interpreter::Status nativeTest() {
    std::puts( "Native Call" );
    return Interpreter::RUNNING;
}

uint64 testGlobal = 0xABADCAFE;

/**
 * Null terminated array of host vectors that are directly invokable via HCF
 */
Interpreter::HostCall aVectors[] = {
    nativeTest,
    0
};

/**
 * Symbols the host application requires from the binary. Terminated by a null name.
 */
LinkSymbol aImports[] = {
    { "main", {0}, LinkSymbol::EXECUTE },
    { 0, { 0 }, 0 }
};

/**
 * Symbols the host application makes available to the binary. Terminated by a null name.
 */
LinkSymbol aExports[] = {
    { "abadcafe", { &testGlobal }, LinkSymbol::READ|LinkSymbol::WRITE },
    { 0, { 0 }, 0 }
};

/**
 * Declare the host
 */
MC64K::Host hostApplication(
    "Standard Test Host",
    1, 0, 0,
    aVectors,
    aExports,
    aImports
);

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
        Binary oMC64KBinary(sExecutableName);
        const Executable* pExecutable = oMC64KBinary.load();
        const LinkSymbol* aExports = 0;

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
                    "\t%2u %p %s 0x%016lX\n",
                    u,
                    aExports[u].pRawData,
                    aExports[u].sIdentifier,
                    aExports[u].uFlags
                );
            }
        }

        if ( (uSymbolCount = pExecutable->getNumImportedSymbols()) ) {
            std::printf(
                "Executable expects %u imported symbols:\n",
                uSymbolCount
            );

            const LinkSymbol* aImports = pExecutable->getImportedSymbols();
            for (unsigned u = 0; u < uSymbolCount; ++u) {
                std::printf(
                    "\t%2u %p %s 0x%016lX\n",
                    u,
                    aImports[u].pRawData,
                    aImports[u].sIdentifier,
                    aExports[u].uFlags
                );
            }
        }

        if (aExports) {
            const int iDumpState = Interpreter::STATE_FPR|Interpreter::STATE_GPR|Interpreter::STATE_TMP;
            Interpreter::allocateStack(256);
            Interpreter::setHostFunction(nativeTest, 0x69);
            Interpreter::setProgramCounter(aExports[0].pByteCode);
            Interpreter::dumpState (iDumpState);
            Interpreter::run();
            Interpreter::dumpState (iDumpState|Interpreter::STATE_STACK);
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
